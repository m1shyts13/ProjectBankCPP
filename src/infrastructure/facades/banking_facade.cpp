#include "infrastructure/facades/banking_facade.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "application/dto/commands.h"
#include "domain/money/money.h"
#include "infrastructure/repositories/in_memory_account_repository.h"
#include "infrastructure/repositories/in_memory_client_repository.h"
#include "infrastructure/repositories/in_memory_transaction_repository.h"
#include "infrastructure/repositories/json_account_repository.h"
#include "infrastructure/repositories/json_client_repository.h"
#include "infrastructure/repositories/json_transaction_repository.h"

namespace banking::infrastructure {
namespace {

unsigned long long NextNumber(const std::string& value,
                              const std::string& prefix) {
  std::string number = value;
  if (!prefix.empty()) {
    const std::string full_prefix = prefix + "-";
    if (value.rfind(full_prefix, 0) != 0) {
      return 0;
    }
    number = value.substr(full_prefix.size());
  }

  if (number.empty() ||
      !std::all_of(number.begin(), number.end(), [](unsigned char symbol) {
        return std::isdigit(symbol);
      })) {
    return 0;
  }
  return std::stoull(number) + 1;
}

unsigned long long NextClientId(application::IClientRepository& repository) {
  unsigned long long next_value = 1;
  for (const domain::Client& client : repository.FindAll()) {
    next_value = std::max(next_value, NextNumber(client.Id().Value(), ""));
  }
  return next_value;
}

unsigned long long NextAccountId(application::IAccountRepository& repository) {
  unsigned long long next_value = 1;
  for (const domain::BankAccount& account : repository.FindAll()) {
    next_value =
        std::max(next_value, NextNumber(account.Id().Value(), "account"));
  }
  return next_value;
}

unsigned long long NextTransactionId(
    application::ITransactionRepository& repository) {
  unsigned long long next_value = 1;
  for (const domain::Transaction& transaction : repository.FindAll()) {
    next_value = std::max(
        next_value, NextNumber(transaction.Id().Value(), "transaction"));
  }
  return next_value;
}

}

BankingFacade::BankingFacade()
    : BankingFacade(std::make_unique<InMemoryClientRepository>(),
                    std::make_unique<InMemoryAccountRepository>(),
                    std::make_unique<InMemoryTransactionRepository>()) {}

BankingFacade::BankingFacade(std::filesystem::path storage_directory)
    : BankingFacade(std::make_unique<JsonClientRepository>(storage_directory),
                    std::make_unique<JsonAccountRepository>(storage_directory),
                    std::make_unique<JsonTransactionRepository>(
                        storage_directory)) {}

BankingFacade::BankingFacade(
    std::unique_ptr<application::IClientRepository> client_repository,
    std::unique_ptr<application::IAccountRepository> account_repository,
    std::unique_ptr<application::ITransactionRepository> transaction_repository)
    : client_repository_(std::move(client_repository)),
      account_repository_(std::move(account_repository)),
      transaction_repository_(std::move(transaction_repository)),
      client_id_generator_("", NextClientId(*client_repository_)),
      account_id_generator_("account", NextAccountId(*account_repository_)),
      transaction_id_generator_("transaction",
                                NextTransactionId(*transaction_repository_)),
      commission_policy_(
          std::make_shared<domain::PercentCommissionPolicy>(100)),
      limit_policy_(std::make_shared<domain::SingleOperationLimitPolicy>(
          domain::Money::Rubles(1000000))),
      transfer_service_(commission_policy_, limit_policy_),
      create_client_use_case_(*client_repository_, client_id_generator_),
      open_account_use_case_(*client_repository_, *account_repository_,
                             account_id_generator_, account_factory_),
      deposit_money_use_case_(*account_repository_, *transaction_repository_,
                              transaction_id_generator_),
      withdraw_money_use_case_(*account_repository_, *transaction_repository_,
                               transaction_id_generator_),
      transfer_money_use_case_(*account_repository_, *transaction_repository_,
                               transaction_id_generator_, transfer_service_),
      show_balance_use_case_(*account_repository_),
      show_transaction_history_use_case_(*transaction_repository_),
      list_clients_use_case_(*client_repository_),
      list_accounts_use_case_(*account_repository_) {}

application::OperationResponse BankingFacade::CreateClient(
    const std::string& full_name) {
  return create_client_use_case_.Execute({full_name});
}

application::OperationResponse BankingFacade::OpenAccount(
    const std::string& client_id, const std::string& account_type) {
  return open_account_use_case_.Execute({client_id, account_type});
}

application::OperationResponse BankingFacade::Deposit(
    const std::string& account_id, long long kopecks) {
  return deposit_money_use_case_.Execute({account_id, kopecks});
}

application::OperationResponse BankingFacade::Withdraw(
    const std::string& account_id, long long kopecks) {
  return withdraw_money_use_case_.Execute({account_id, kopecks});
}

application::OperationResponse BankingFacade::Transfer(
    const std::string& from_account_id, const std::string& to_account_id,
    long long kopecks) {
  return transfer_money_use_case_.Execute(
      {from_account_id, to_account_id, kopecks});
}

application::BalanceResponse BankingFacade::ShowBalance(
    const std::string& account_id) {
  return show_balance_use_case_.Execute({account_id});
}

application::TransactionHistoryResponse BankingFacade::ShowTransactionHistory(
    const std::string& account_id) {
  return show_transaction_history_use_case_.Execute({account_id});
}

application::ListClientsResponse BankingFacade::ListClients() {
  return list_clients_use_case_.Execute();
}

application::ListAccountsResponse BankingFacade::ListAccounts() {
  return list_accounts_use_case_.Execute();
}

}
