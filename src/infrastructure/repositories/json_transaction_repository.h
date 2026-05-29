#pragma once

#include <filesystem>
#include <vector>

#include "application/ports/transaction_repository.h"

namespace banking::infrastructure {

class JsonTransactionRepository final : public application::ITransactionRepository {
 public:
  explicit JsonTransactionRepository(std::filesystem::path storage_directory);

  void Save(const domain::Transaction& transaction) override;
  std::vector<domain::Transaction> FindByAccountId(
      const domain::AccountId& account_id) override;
  std::vector<domain::Transaction> FindAll() override;

 private:
  void Load();
  void Flush() const;

  std::filesystem::path file_path_;
  std::vector<domain::Transaction> transactions_;
};

}
