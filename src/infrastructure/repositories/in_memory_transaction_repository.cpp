#include "infrastructure/repositories/in_memory_transaction_repository.h"

namespace banking::infrastructure {

void InMemoryTransactionRepository::Save(
    const domain::Transaction& transaction) {
  transactions_.push_back(transaction);
}

std::vector<domain::Transaction>
InMemoryTransactionRepository::FindByAccountId(
    const domain::AccountId& account_id) {
  std::vector<domain::Transaction> result;
  for (const domain::Transaction& transaction : transactions_) {
    const bool is_source =
        transaction.FromAccountId().has_value() &&
        transaction.FromAccountId().value() == account_id;
    const bool is_target =
        transaction.ToAccountId().has_value() &&
        transaction.ToAccountId().value() == account_id;
    if (is_source || is_target) {
      result.push_back(transaction);
    }
  }
  return result;
}

std::vector<domain::Transaction> InMemoryTransactionRepository::FindAll() {
  return transactions_;
}

}  // namespace banking::infrastructure
