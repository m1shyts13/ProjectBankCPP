#include "domain/services/transfer_service.h"

#include <utility>

namespace banking::domain {

TransferService::TransferService(
    std::shared_ptr<const ICommissionPolicy> commission_policy,
    std::shared_ptr<const ILimitPolicy> limit_policy)
    : commission_policy_(std::move(commission_policy)),
      limit_policy_(std::move(limit_policy)) {}

TransferResult TransferService::Transfer(BankAccount& from, BankAccount& to,
                                         const Money& amount) const {
  limit_policy_->Check(amount);
  Money commission = commission_policy_->Calculate(amount);
  Money total_withdrawn = amount + commission;

  from.Withdraw(total_withdrawn);
  to.Deposit(amount);

  return TransferResult{
      .amount = amount,
      .commission = commission,
      .total_withdrawn = total_withdrawn,
  };
}

}  // namespace banking::domain
