#pragma once

#include <string>

namespace banking::application {

class IdGenerator {
 public:
  explicit IdGenerator(std::string prefix);

  std::string Next();

 private:
  std::string prefix_;
  unsigned long long next_value_;
};

}  // namespace banking::application
