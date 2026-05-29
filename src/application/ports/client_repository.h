#pragma once

#include <optional>
#include <vector>

#include "domain/client/client.h"
#include "domain/client/client_id.h"

namespace banking::application {

class IClientRepository {
 public:
  virtual void Save(const domain::Client& client) = 0;
  virtual std::optional<domain::Client> FindById(const domain::ClientId& id) = 0;
  virtual std::vector<domain::Client> FindAll() = 0;
  virtual ~IClientRepository() = default;
};

}
