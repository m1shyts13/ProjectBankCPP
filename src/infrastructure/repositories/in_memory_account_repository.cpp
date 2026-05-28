#include "infrastructure/repositories/in_memory_account_repository.h"

namespace banking::infrastructure {

void InMemoryAccountRepository::Save(const domain::BankAccount& account) {
  accounts_.insert_or_assign(account.Id().Value(), account);
}

std::optional<domain::BankAccount> InMemoryAccountRepository::FindById(
    const domain::AccountId& id) {
  auto iterator = accounts_.find(id.Value());
  if (iterator == accounts_.end()) {
    return std::nullopt;
  }
  return iterator->second;
}

std::vector<domain::BankAccount> InMemoryAccountRepository::FindByClientId(
    const domain::ClientId& client_id) {
  std::vector<domain::BankAccount> result;
  for (const auto& entry : accounts_) {
    if (entry.second.OwnerId() == client_id) {
      result.push_back(entry.second);
    }
  }
  return result;
}

std::vector<domain::BankAccount> InMemoryAccountRepository::FindAll() {
  std::vector<domain::BankAccount> result;
  result.reserve(accounts_.size());
  for (const auto& entry : accounts_) {
    result.push_back(entry.second);
  }
  return result;
}

}  // namespace banking::infrastructure
