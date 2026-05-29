#include "application/services/id_generator.h"

#include <utility>

namespace banking::application {

IdGenerator::IdGenerator(std::string prefix, unsigned long long first_value)
    : prefix_(std::move(prefix)), next_value_(first_value) {}

std::string IdGenerator::Next() {
  if (prefix_.empty()) {
    return std::to_string(next_value_++);
  }
  return prefix_ + "-" + std::to_string(next_value_++);
}

}  // namespace banking::application
