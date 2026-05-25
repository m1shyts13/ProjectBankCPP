#pragma once

#include "domain/money/money.h"

namespace banking::domain {

class ILimitPolicy {
 public:
  virtual void Check(const Money& amount) const = 0;
  virtual ~ILimitPolicy() = default;
};

class NoLimitPolicy : public ILimitPolicy {
 public:
  void Check(const Money& amount) const override;
};

class SingleOperationLimitPolicy : public ILimitPolicy {
 public:
  explicit SingleOperationLimitPolicy(Money limit);

  void Check(const Money& amount) const override;

 private:
  Money limit_;
};

}  // namespace banking::domain
