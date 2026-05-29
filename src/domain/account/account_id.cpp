#include "domain/account/account_id.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

AccountId::AccountId(std::string value) : value_(std::move(value)) {
  if (value_.empty()) {
    throw DomainError("account id cannot be empty");
  }
}

const std::string& AccountId::Value() const {
  return value_;
}

bool AccountId::operator==(const AccountId& other) const {
  return value_ == other.value_;
}

}
