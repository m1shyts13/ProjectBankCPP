#include "domain/policies/commission_policy.h"

#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

Money NoCommissionPolicy::Calculate(const Money& amount) const {
  static_cast<void>(amount);
  return Money::Zero();
}

FixedCommissionPolicy::FixedCommissionPolicy(Money fixed_commission)
    : fixed_commission_(std::move(fixed_commission)) {}

Money FixedCommissionPolicy::Calculate(const Money& amount) const {
  static_cast<void>(amount);
  return fixed_commission_;
}

PercentCommissionPolicy::PercentCommissionPolicy(int basis_points)
    : basis_points_(basis_points) {
  if (basis_points < 0) {
    throw DomainError("invalid commission percent");
  }
}

Money PercentCommissionPolicy::Calculate(const Money& amount) const {
  return Money(amount.Kopecks() * basis_points_ / 10000);
}

}  // namespace banking::domain
