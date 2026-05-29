#pragma once

#include <string>

namespace banking::domain {

class AccountId {
 public:
  explicit AccountId(std::string value);

  const std::string& Value() const;
  bool operator==(const AccountId& other) const;

 private:
  std::string value_;
};

}
