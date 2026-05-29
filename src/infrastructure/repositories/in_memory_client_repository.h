#pragma once

#include <string>
#include <unordered_map>

#include "application/ports/client_repository.h"

namespace banking::infrastructure {

class InMemoryClientRepository final : public application::IClientRepository {
 public:
  void Save(const domain::Client& client) override;
  std::optional<domain::Client> FindById(const domain::ClientId& id) override;
  std::vector<domain::Client> FindAll() override;

 private:
  std::unordered_map<std::string, domain::Client> clients_;
};

}
