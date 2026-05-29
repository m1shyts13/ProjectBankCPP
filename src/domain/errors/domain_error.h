#pragma once

#include <stdexcept>
#include <string>

namespace banking::domain {

class DomainError : public std::runtime_error {
 public:
  explicit DomainError(const std::string& message);
};

}
