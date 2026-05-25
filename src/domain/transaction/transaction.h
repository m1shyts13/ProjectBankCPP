#pragma once

#include <optional>
#include <string>

#include "domain/account/account_id.h"
#include "domain/money/money.h"
#include "domain/transaction/transaction_id.h"
#include "domain/transaction/transaction_type.h"

namespace banking::domain {

class Transaction {
 public:
  Transaction(TransactionId id, std::optional<AccountId> from_account_id,
              std::optional<AccountId> to_account_id, Money amount,
              Money commission, TransactionType type, std::string created_at,
              std::string description);

  const TransactionId& Id() const;
  const std::optional<AccountId>& FromAccountId() const;
  const std::optional<AccountId>& ToAccountId() const;
  const Money& Amount() const;
  const Money& Commission() const;
  TransactionType Type() const;
  const std::string& CreatedAt() const;
  const std::string& Description() const;

 private:
  void Validate() const;

  TransactionId id_;
  std::optional<AccountId> from_account_id_;
  std::optional<AccountId> to_account_id_;
  Money amount_;
  Money commission_;
  TransactionType type_;
  std::string created_at_;
  std::string description_;
};

}  // namespace banking::domain
