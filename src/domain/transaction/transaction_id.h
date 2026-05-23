#pragma once

#include <string>

namespace banking::domain {

class TransactionId {
 public:
  explicit TransactionId(std::string value);

  const std::string& Value() const;
  bool operator==(const TransactionId& other) const;

 private:
  std::string value_;
};

}  // namespace banking::domain
