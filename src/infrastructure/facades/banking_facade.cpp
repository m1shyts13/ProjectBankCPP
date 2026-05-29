#include "infrastructure/facades/banking_facade.h"

#include "application/dto/commands.h"
#include "domain/money/money.h"

namespace banking::infrastructure {

BankingFacade::BankingFacade()
    : client_id_generator_(""),
      account_id_generator_("account"),
      transaction_id_generator_("transaction"),
      commission_policy_(
          std::make_shared<domain::PercentCommissionPolicy>(100)),
      limit_policy_(std::make_shared<domain::SingleOperationLimitPolicy>(
          domain::Money::Rubles(1000000))),
      transfer_service_(commission_policy_, limit_policy_),
      create_client_use_case_(client_repository_, client_id_generator_),
      open_account_use_case_(client_repository_, account_repository_,
                             account_id_generator_, account_factory_),
      deposit_money_use_case_(account_repository_, transaction_repository_,
                              transaction_id_generator_),
      withdraw_money_use_case_(account_repository_, transaction_repository_,
                               transaction_id_generator_),
      transfer_money_use_case_(account_repository_, transaction_repository_,
                               transaction_id_generator_, transfer_service_),
      show_balance_use_case_(account_repository_),
      show_transaction_history_use_case_(transaction_repository_),
      list_clients_use_case_(client_repository_),
      list_accounts_use_case_(account_repository_) {}

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
