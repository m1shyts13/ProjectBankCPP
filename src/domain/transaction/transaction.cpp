#include "domain/transaction/transaction.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

Transaction::Transaction(TransactionId id,
                         std::optional<AccountId> from_account_id,
                         std::optional<AccountId> to_account_id, Money amount,
                         Money commission, TransactionType type,
                         std::string created_at, std::string description)
    : id_(std::move(id)),
      from_account_id_(std::move(from_account_id)),
      to_account_id_(std::move(to_account_id)),
      amount_(std::move(amount)),
      commission_(std::move(commission)),
      type_(type),
      created_at_(std::move(created_at)),
      description_(std::move(description)) {
  Validate();
}

const TransactionId& Transaction::Id() const {
  return id_;
}

const std::optional<AccountId>& Transaction::FromAccountId() const {
  return from_account_id_;
}

const std::optional<AccountId>& Transaction::ToAccountId() const {
  return to_account_id_;
}

const Money& Transaction::Amount() const {
  return amount_;
}

const Money& Transaction::Commission() const {
  return commission_;
}

TransactionType Transaction::Type() const {
  return type_;
}

const std::string& Transaction::CreatedAt() const {
  return created_at_;
}

const std::string& Transaction::Description() const {
  return description_;
}

void Transaction::Validate() const {
  if (amount_ <= Money::Zero()) {
    throw DomainError("invalid amount");
  }

  if (created_at_.empty()) {
    throw DomainError("transaction created_at cannot be empty");
  }

  switch (type_) {
    case TransactionType::Deposit:
      if (from_account_id_.has_value() || !to_account_id_.has_value()) {
        throw DomainError("deposit transaction requires only target account");
      }
      break;
    case TransactionType::Withdraw:
      if (!from_account_id_.has_value() || to_account_id_.has_value()) {
        throw DomainError("withdraw transaction requires only source account");
      }
      break;
    case TransactionType::Transfer:
      if (!from_account_id_.has_value() || !to_account_id_.has_value()) {
        throw DomainError("transfer transaction requires both accounts");
      }
      break;
  }
}

}
