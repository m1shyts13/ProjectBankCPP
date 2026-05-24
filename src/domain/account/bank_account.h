#pragma once

#include "domain/account/account_id.h"
#include "domain/account/account_status.h"
#include "domain/account/account_type.h"
#include "domain/client/client_id.h"
#include "domain/money/money.h"

namespace banking::domain {

class BankAccount {
 public:
  BankAccount(AccountId id, ClientId owner_id, AccountType type);

  void Deposit(const Money& amount);
  void Withdraw(const Money& amount);
  void Close();

  bool IsClosed() const;
  const Money& Balance() const;
  const AccountId& Id() const;
  const ClientId& OwnerId() const;
  AccountType Type() const;

 private:
  void EnsureOpen() const;
  static void EnsurePositiveAmount(const Money& amount);

  AccountId id_;
  ClientId owner_id_;
  Money balance_;
  AccountStatus status_;
  AccountType type_;
};

}  // namespace banking::domain
