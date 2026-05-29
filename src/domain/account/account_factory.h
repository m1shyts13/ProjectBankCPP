#pragma once

#include "domain/account/account_id.h"
#include "domain/account/account_type.h"
#include "domain/account/bank_account.h"
#include "domain/client/client_id.h"

namespace banking::domain {

class AccountFactory {
 public:
  BankAccount Create(AccountId id, ClientId owner_id, AccountType type) const;
};

}
