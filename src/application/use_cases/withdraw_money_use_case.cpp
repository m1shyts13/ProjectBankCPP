#include "application/use_cases/withdraw_money_use_case.h"

#include <optional>
#include <stdexcept>

#include "application/services/timestamp.h"
#include "domain/account/account_id.h"
#include "domain/money/money.h"
#include "domain/transaction/transaction.h"
#include "domain/transaction/transaction_id.h"
#include "domain/transaction/transaction_type.h"

namespace banking::application {

WithdrawMoneyUseCase::WithdrawMoneyUseCase(
    IAccountRepository& account_repository,
    ITransactionRepository& transaction_repository,
    IdGenerator& transaction_id_generator)
    : account_repository_(account_repository),
      transaction_repository_(transaction_repository),
      transaction_id_generator_(transaction_id_generator) {}

OperationResponse WithdrawMoneyUseCase::Execute(
    const WithdrawMoneyCommand& command) {
  domain::AccountId account_id(command.account_id);
  std::optional<domain::BankAccount> found =
      account_repository_.FindById(account_id);
  if (!found.has_value()) {
    throw std::runtime_error("account not found");
  }

  domain::Money amount(command.kopecks);
  domain::BankAccount account = found.value();
  account.Withdraw(amount);

  domain::Transaction transaction(
      domain::TransactionId(transaction_id_generator_.Next()), account.Id(),
      std::nullopt, amount, domain::Money::Zero(),
      domain::TransactionType::Withdraw, CurrentTimestamp(), "Withdraw");

  account_repository_.Save(account);
  transaction_repository_.Save(transaction);

  return OperationResponse{
      .success = true,
      .message = "Money withdrawn",
      .id = transaction.Id().Value(),
  };
}

}  // namespace banking::application
