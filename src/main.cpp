#include <exception>
#include <iostream>
#include <memory>

#include "application/services/id_generator.h"
#include "application/use_cases/create_client_use_case.h"
#include "application/use_cases/deposit_money_use_case.h"
#include "application/use_cases/list_accounts_use_case.h"
#include "application/use_cases/list_clients_use_case.h"
#include "application/use_cases/open_account_use_case.h"
#include "application/use_cases/show_balance_use_case.h"
#include "application/use_cases/show_transaction_history_use_case.h"
#include "application/use_cases/transfer_money_use_case.h"
#include "application/use_cases/withdraw_money_use_case.h"
#include "domain/account/account_factory.h"
#include "domain/money/money.h"
#include "domain/policies/commission_policy.h"
#include "domain/policies/limit_policy.h"
#include "domain/services/transfer_service.h"
#include "infrastructure/repositories/in_memory_account_repository.h"
#include "infrastructure/repositories/in_memory_client_repository.h"
#include "infrastructure/repositories/in_memory_transaction_repository.h"

int main() {
  try {
    banking::infrastructure::InMemoryClientRepository client_repository;
    banking::infrastructure::InMemoryAccountRepository account_repository;
    banking::infrastructure::InMemoryTransactionRepository
        transaction_repository;

    banking::application::IdGenerator client_id_generator("");
    banking::application::IdGenerator account_id_generator("account");
    banking::application::IdGenerator transaction_id_generator("transaction");

    banking::domain::AccountFactory account_factory;
    auto commission_policy =
        std::make_shared<banking::domain::PercentCommissionPolicy>(100);
    auto limit_policy =
        std::make_shared<banking::domain::SingleOperationLimitPolicy>(
            banking::domain::Money::Rubles(1000000));
    banking::domain::TransferService transfer_service(commission_policy,
                                                      limit_policy);

    banking::application::CreateClientUseCase create_client_use_case(
        client_repository, client_id_generator);
    banking::application::OpenAccountUseCase open_account_use_case(
        client_repository, account_repository, account_id_generator,
        account_factory);
    banking::application::DepositMoneyUseCase deposit_money_use_case(
        account_repository, transaction_repository, transaction_id_generator);
    banking::application::WithdrawMoneyUseCase withdraw_money_use_case(
        account_repository, transaction_repository, transaction_id_generator);
    banking::application::TransferMoneyUseCase transfer_money_use_case(
        account_repository, transaction_repository, transaction_id_generator,
        transfer_service);
    banking::application::ShowBalanceUseCase show_balance_use_case(
        account_repository);
    banking::application::ShowTransactionHistoryUseCase
        show_transaction_history_use_case(transaction_repository);
    banking::application::ListClientsUseCase list_clients_use_case(
        client_repository);
    banking::application::ListAccountsUseCase list_accounts_use_case(
        account_repository);

    const auto client = create_client_use_case.Execute({"Ada Lovelace"});
    const auto account = open_account_use_case.Execute({client.id, "debit"});
    deposit_money_use_case.Execute({account.id, 250000});
    const auto balance = show_balance_use_case.Execute({account.id});

    std::cout << "Mini Banking System DDD" << "\n";
    std::cout << "Client: " << client.id << "\n";
    std::cout << "Account: " << account.id << "\n";
    std::cout << "Balance: " << balance.balance << "\n";
  } catch (const std::exception& exception) {
    std::cerr << "Fatal error: " << exception.what() << "\n";
    return 1;
  }

  return 0;
}
