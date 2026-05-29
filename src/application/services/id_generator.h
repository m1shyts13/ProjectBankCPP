#pragma once

#include <string>

namespace banking::application {

class IdGenerator {
 public:
  explicit IdGenerator(std::string prefix, unsigned long long first_value = 1);

  std::string Next();

 private:
  std::string prefix_;
  unsigned long long next_value_;
};

}  // namespace banking::application
