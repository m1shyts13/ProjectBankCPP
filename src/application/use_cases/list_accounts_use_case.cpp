#include "application/use_cases/list_accounts_use_case.h"

namespace banking::application {
namespace {

std::string AccountTypeToString(domain::AccountType type) {
  switch (type) {
    case domain::AccountType::Debit:
      return "Debit";
    case domain::AccountType::Savings:
      return "Savings";
  }
  return "Unknown";
}

}

ListAccountsUseCase::ListAccountsUseCase(
    IAccountRepository& account_repository)
    : account_repository_(account_repository) {}

ListAccountsResponse ListAccountsUseCase::Execute() {
  ListAccountsResponse response;
  for (const domain::BankAccount& account : account_repository_.FindAll()) {
    response.accounts.push_back(AccountSummaryResponse{
        .id = account.Id().Value(),
        .owner_id = account.OwnerId().Value(),
        .balance = account.Balance().ToString(),
        .type = AccountTypeToString(account.Type()),
        .status = account.IsClosed() ? "Closed" : "Open",
    });
  }
  return response;
}

}
