#pragma once

#include <chrono>
#include <string>

namespace banking::application {

inline std::string CurrentTimestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
      now.time_since_epoch());
  return std::to_string(seconds.count());
}

}  // namespace banking::application
