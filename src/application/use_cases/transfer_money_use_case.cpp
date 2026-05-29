#include "application/use_cases/transfer_money_use_case.h"

#include <optional>
#include <stdexcept>

#include "application/services/timestamp.h"
#include "domain/account/account_id.h"
#include "domain/money/money.h"
#include "domain/transaction/transaction.h"
#include "domain/transaction/transaction_id.h"
#include "domain/transaction/transaction_type.h"

namespace banking::application {

TransferMoneyUseCase::TransferMoneyUseCase(
    IAccountRepository& account_repository,
    ITransactionRepository& transaction_repository,
    IdGenerator& transaction_id_generator,
    const domain::TransferService& transfer_service)
    : account_repository_(account_repository),
      transaction_repository_(transaction_repository),
      transaction_id_generator_(transaction_id_generator),
      transfer_service_(transfer_service) {}

OperationResponse TransferMoneyUseCase::Execute(
    const TransferMoneyCommand& command) {
  if (command.from_account_id == command.to_account_id) {
    throw std::runtime_error("cannot transfer to the same account");
  }

  domain::AccountId from_account_id(command.from_account_id);
  domain::AccountId to_account_id(command.to_account_id);
  std::optional<domain::BankAccount> from_found =
      account_repository_.FindById(from_account_id);
  std::optional<domain::BankAccount> to_found =
      account_repository_.FindById(to_account_id);

  if (!from_found.has_value()) {
    throw std::runtime_error("source account not found");
  }
  if (!to_found.has_value()) {
    throw std::runtime_error("target account not found");
  }

  domain::Money amount(command.kopecks);
  domain::BankAccount from_account = from_found.value();
  domain::BankAccount to_account = to_found.value();
  domain::TransferResult result =
      transfer_service_.Transfer(from_account, to_account, amount);

  domain::Transaction transaction(
      domain::TransactionId(transaction_id_generator_.Next()),
      from_account.Id(), to_account.Id(), result.amount, result.commission,
      domain::TransactionType::Transfer, CurrentTimestamp(), "Transfer");

  account_repository_.Save(from_account);
  account_repository_.Save(to_account);
  transaction_repository_.Save(transaction);

  return OperationResponse{
      .success = true,
      .message = "Money transferred",
      .id = transaction.Id().Value(),
  };
}

}
