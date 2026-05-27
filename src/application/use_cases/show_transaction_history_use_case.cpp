#include "application/use_cases/show_transaction_history_use_case.h"

#include "domain/account/account_id.h"
#include "domain/transaction/transaction_type.h"

namespace banking::application {
namespace {

std::string OptionalAccountIdToString(
    const std::optional<domain::AccountId>& account_id) {
  if (!account_id.has_value()) {
    return "-";
  }
  return account_id->Value();
}

std::string TransactionTypeToString(domain::TransactionType type) {
  switch (type) {
    case domain::TransactionType::Deposit:
      return "Deposit";
    case domain::TransactionType::Withdraw:
      return "Withdraw";
    case domain::TransactionType::Transfer:
      return "Transfer";
  }
  return "Unknown";
}

}  // namespace

ShowTransactionHistoryUseCase::ShowTransactionHistoryUseCase(
    ITransactionRepository& transaction_repository)
    : transaction_repository_(transaction_repository) {}

TransactionHistoryResponse ShowTransactionHistoryUseCase::Execute(
    const ShowTransactionHistoryCommand& command) {
  domain::AccountId account_id(command.account_id);
  TransactionHistoryResponse response;

  for (const domain::Transaction& transaction :
       transaction_repository_.FindByAccountId(account_id)) {
    response.transactions.push_back(TransactionDto{
        .id = transaction.Id().Value(),
        .from_account_id = OptionalAccountIdToString(
            transaction.FromAccountId()),
        .to_account_id = OptionalAccountIdToString(transaction.ToAccountId()),
        .amount = transaction.Amount().ToString(),
        .commission = transaction.Commission().ToString(),
        .type = TransactionTypeToString(transaction.Type()),
        .created_at = transaction.CreatedAt(),
        .description = transaction.Description(),
    });
  }

  return response;
}

}  // namespace banking::application
