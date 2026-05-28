#pragma once

#include <string>
#include <unordered_map>

#include "application/ports/account_repository.h"

namespace banking::infrastructure {

class InMemoryAccountRepository final : public application::IAccountRepository {
 public:
  void Save(const domain::BankAccount& account) override;
  std::optional<domain::BankAccount> FindById(
      const domain::AccountId& id) override;
  std::vector<domain::BankAccount> FindByClientId(
      const domain::ClientId& client_id) override;
  std::vector<domain::BankAccount> FindAll() override;

 private:
  std::unordered_map<std::string, domain::BankAccount> accounts_;
};

}  // namespace banking::infrastructure
