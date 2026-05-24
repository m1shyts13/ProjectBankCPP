#include "domain/account/account_factory.h"

#include <utility>

namespace banking::domain {

BankAccount AccountFactory::Create(AccountId id, ClientId owner_id,
                                   AccountType type) const {
  return BankAccount(std::move(id), std::move(owner_id), type);
}

}  // namespace banking::domain
