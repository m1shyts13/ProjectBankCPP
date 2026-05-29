#include "domain/account/bank_account.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

BankAccount::BankAccount(AccountId id, ClientId owner_id, AccountType type)
    : id_(std::move(id)),
      owner_id_(std::move(owner_id)),
      balance_(Money::Zero()),
      status_(AccountStatus::Open),
      type_(type) {}

void BankAccount::Deposit(const Money& amount) {
  EnsureOpen();
  EnsurePositiveAmount(amount);
  balance_ = balance_ + amount;
}

void BankAccount::Withdraw(const Money& amount) {
  EnsureOpen();
  EnsurePositiveAmount(amount);
  if (amount > balance_) {
    throw DomainError("insufficient funds");
  }
  balance_ = balance_ - amount;
}

void BankAccount::Close() {
  if (IsClosed()) {
    throw DomainError("account already closed");
  }
  if (balance_ > Money::Zero()) {
    throw DomainError("cannot close account with non-zero balance");
  }
  status_ = AccountStatus::Closed;
}

bool BankAccount::IsClosed() const {
  return status_ == AccountStatus::Closed;
}

const Money& BankAccount::Balance() const {
  return balance_;
}

const AccountId& BankAccount::Id() const {
  return id_;
}

const ClientId& BankAccount::OwnerId() const {
  return owner_id_;
}

AccountType BankAccount::Type() const {
  return type_;
}

void BankAccount::EnsureOpen() const {
  if (IsClosed()) {
    throw DomainError("account is closed");
  }
}

void BankAccount::EnsurePositiveAmount(const Money& amount) {
  if (amount <= Money::Zero()) {
    throw DomainError("invalid amount");
  }
}

}
