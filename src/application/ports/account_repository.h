#pragma once

#include <optional>
#include <vector>

#include "domain/account/bank_account.h"
#include "domain/account/account_id.h"
#include "domain/client/client_id.h"

namespace banking::application {

class IAccountRepository {
 public:
  virtual void Save(const domain::BankAccount& account) = 0;
  virtual std::optional<domain::BankAccount> FindById(
      const domain::AccountId& id) = 0;
  virtual std::vector<domain::BankAccount> FindByClientId(
      const domain::ClientId& client_id) = 0;
  virtual std::vector<domain::BankAccount> FindAll() = 0;
  virtual ~IAccountRepository() = default;
};

}
