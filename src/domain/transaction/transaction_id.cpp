#include "domain/transaction/transaction_id.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

TransactionId::TransactionId(std::string value) : value_(std::move(value)) {
  if (value_.empty()) {
    throw DomainError("transaction id cannot be empty");
  }
}

const std::string& TransactionId::Value() const {
  return value_;
}

bool TransactionId::operator==(const TransactionId& other) const {
  return value_ == other.value_;
}

}
