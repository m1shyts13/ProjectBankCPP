#include "domain/client/client_id.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

ClientId::ClientId(std::string value) : value_(std::move(value)) {
  if (value_.empty()) {
    throw DomainError("client id cannot be empty");
  }
  if (!std::all_of(value_.begin(), value_.end(), [](unsigned char symbol) {
        return std::isdigit(symbol);
      })) {
    throw DomainError("client id must be numeric");
  }
}

const std::string& ClientId::Value() const {
  return value_;
}

bool ClientId::operator==(const ClientId& other) const {
  return value_ == other.value_;
}

}  // namespace banking::domain
