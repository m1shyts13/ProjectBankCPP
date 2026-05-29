#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "application/ports/account_repository.h"

namespace banking::infrastructure {

class JsonAccountRepository final : public application::IAccountRepository {
 public:
  explicit JsonAccountRepository(std::filesystem::path storage_directory);

  void Save(const domain::BankAccount& account) override;
  std::optional<domain::BankAccount> FindById(
      const domain::AccountId& id) override;
  std::vector<domain::BankAccount> FindByClientId(
      const domain::ClientId& client_id) override;
  std::vector<domain::BankAccount> FindAll() override;

 private:
  void Load();
  void Flush() const;

  std::filesystem::path file_path_;
  std::unordered_map<std::string, domain::BankAccount> accounts_;
};

}
