#pragma once

#include <memory>
#include <string>

#include "application/dto/responses.h"
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
#include "domain/policies/commission_policy.h"
#include "domain/policies/limit_policy.h"
#include "domain/services/transfer_service.h"
#include "infrastructure/repositories/in_memory_account_repository.h"
#include "infrastructure/repositories/in_memory_client_repository.h"
#include "infrastructure/repositories/in_memory_transaction_repository.h"

namespace banking::infrastructure {

class BankingFacade {
 public:
  BankingFacade();

  application::OperationResponse CreateClient(const std::string& full_name);
  application::OperationResponse OpenAccount(const std::string& client_id,
                                             const std::string& account_type);
  application::OperationResponse Deposit(const std::string& account_id,
                                         long long kopecks);
  application::OperationResponse Withdraw(const std::string& account_id,
                                          long long kopecks);
  application::OperationResponse Transfer(const std::string& from_account_id,
                                          const std::string& to_account_id,
                                          long long kopecks);
  application::BalanceResponse ShowBalance(const std::string& account_id);
  application::TransactionHistoryResponse ShowTransactionHistory(
      const std::string& account_id);
  application::ListClientsResponse ListClients();
  application::ListAccountsResponse ListAccounts();

 private:
  InMemoryClientRepository client_repository_;
  InMemoryAccountRepository account_repository_;
  InMemoryTransactionRepository transaction_repository_;

  application::IdGenerator client_id_generator_;
  application::IdGenerator account_id_generator_;
  application::IdGenerator transaction_id_generator_;

  domain::AccountFactory account_factory_;
  std::shared_ptr<const domain::ICommissionPolicy> commission_policy_;
  std::shared_ptr<const domain::ILimitPolicy> limit_policy_;
  domain::TransferService transfer_service_;

  application::CreateClientUseCase create_client_use_case_;
  application::OpenAccountUseCase open_account_use_case_;
  application::DepositMoneyUseCase deposit_money_use_case_;
  application::WithdrawMoneyUseCase withdraw_money_use_case_;
  application::TransferMoneyUseCase transfer_money_use_case_;
  application::ShowBalanceUseCase show_balance_use_case_;
  application::ShowTransactionHistoryUseCase show_transaction_history_use_case_;
  application::ListClientsUseCase list_clients_use_case_;
  application::ListAccountsUseCase list_accounts_use_case_;
};

}
