#include "application/use_cases/show_balance_use_case.h"

#include <optional>
#include <stdexcept>

#include "domain/account/account_id.h"

namespace banking::application {

ShowBalanceUseCase::ShowBalanceUseCase(IAccountRepository& account_repository)
    : account_repository_(account_repository) {}

BalanceResponse ShowBalanceUseCase::Execute(const ShowBalanceCommand& command) {
  domain::AccountId account_id(command.account_id);
  std::optional<domain::BankAccount> found =
      account_repository_.FindById(account_id);
  if (!found.has_value()) {
    throw std::runtime_error("account not found");
  }

  const domain::BankAccount& account = found.value();
  return BalanceResponse{
      .account_id = account.Id().Value(),
      .balance = account.Balance().ToString(),
      .kopecks = account.Balance().Kopecks(),
  };
}

}  // namespace banking::application
