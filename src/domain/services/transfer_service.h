#pragma once

#include <memory>

#include "domain/account/bank_account.h"
#include "domain/money/money.h"
#include "domain/policies/commission_policy.h"
#include "domain/policies/limit_policy.h"

namespace banking::domain {

struct TransferResult {
  Money amount;
  Money commission;
  Money total_withdrawn;
};

class TransferService {
 public:
  TransferService(std::shared_ptr<const ICommissionPolicy> commission_policy,
                  std::shared_ptr<const ILimitPolicy> limit_policy);

  TransferResult Transfer(BankAccount& from, BankAccount& to,
                          const Money& amount) const;

 private:
  std::shared_ptr<const ICommissionPolicy> commission_policy_;
  std::shared_ptr<const ILimitPolicy> limit_policy_;
};

}
