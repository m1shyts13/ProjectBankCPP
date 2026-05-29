#pragma once

#include <vector>

#include "application/ports/transaction_repository.h"

namespace banking::infrastructure {

class InMemoryTransactionRepository final
    : public application::ITransactionRepository {
 public:
  void Save(const domain::Transaction& transaction) override;
  std::vector<domain::Transaction> FindByAccountId(
      const domain::AccountId& account_id) override;
  std::vector<domain::Transaction> FindAll() override;

 private:
  std::vector<domain::Transaction> transactions_;
};

}
