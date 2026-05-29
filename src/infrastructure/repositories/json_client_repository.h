#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include "application/ports/client_repository.h"

namespace banking::infrastructure {

class JsonClientRepository final : public application::IClientRepository {
 public:
  explicit JsonClientRepository(std::filesystem::path storage_directory);

  void Save(const domain::Client& client) override;
  std::optional<domain::Client> FindById(const domain::ClientId& id) override;
  std::vector<domain::Client> FindAll() override;

 private:
  void Load();
  void Flush() const;

  std::filesystem::path file_path_;
  std::unordered_map<std::string, domain::Client> clients_;
};

}
