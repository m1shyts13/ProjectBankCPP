#pragma once

#include <string>

#include "domain/client/client_id.h"

namespace banking::domain {

class Client {
 public:
  Client(ClientId id, std::string full_name);

  const ClientId& Id() const;
  const std::string& FullName() const;

 private:
  ClientId id_;
  std::string full_name_;
};

}
