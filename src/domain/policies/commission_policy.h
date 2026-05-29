#pragma once

#include "domain/money/money.h"

namespace banking::domain {

class ICommissionPolicy {
 public:
  virtual Money Calculate(const Money& amount) const = 0;
  virtual ~ICommissionPolicy() = default;
};

class NoCommissionPolicy : public ICommissionPolicy {
 public:
  Money Calculate(const Money& amount) const override;
};

class FixedCommissionPolicy : public ICommissionPolicy {
 public:
  explicit FixedCommissionPolicy(Money fixed_commission);

  Money Calculate(const Money& amount) const override;

 private:
  Money fixed_commission_;
};

class PercentCommissionPolicy : public ICommissionPolicy {
 public:
  explicit PercentCommissionPolicy(int basis_points);

  Money Calculate(const Money& amount) const override;

 private:
  int basis_points_;
};

}
