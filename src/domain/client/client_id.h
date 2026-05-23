#pragma once

#include <string>

namespace banking::domain {

class ClientId {
 public:
  explicit ClientId(std::string value);

  const std::string& Value() const;
  bool operator==(const ClientId& other) const;

 private:
  std::string value_;
};

}  // namespace banking::domain
