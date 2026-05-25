#include "domain/policies/limit_policy.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

void NoLimitPolicy::Check(const Money& amount) const {
  static_cast<void>(amount);
}

SingleOperationLimitPolicy::SingleOperationLimitPolicy(Money limit)
    : limit_(std::move(limit)) {}

void SingleOperationLimitPolicy::Check(const Money& amount) const {
  if (amount > limit_) {
    throw DomainError("operation limit exceeded");
  }
}

}  // namespace banking::domain
