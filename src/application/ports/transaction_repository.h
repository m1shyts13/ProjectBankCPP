#pragma once

#include <vector>

#include "domain/account/account_id.h"
#include "domain/transaction/transaction.h"

namespace banking::application {

class ITransactionRepository {
 public:
  virtual void Save(const domain::Transaction& transaction) = 0;
  virtual std::vector<domain::Transaction> FindByAccountId(
      const domain::AccountId& account_id) = 0;
  virtual std::vector<domain::Transaction> FindAll() = 0;
  virtual ~ITransactionRepository() = default;
};

}  // namespace banking::application
