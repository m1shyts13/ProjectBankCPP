#include "domain/money/money.h"

#include <iomanip>
#include <sstream>

#include "domain/errors/domain_error.h"

namespace banking::domain {

Money::Money(long long kopecks) : kopecks_(kopecks) {
  if (kopecks < 0) {
    throw DomainError("invalid amount");
  }
}

Money Money::Rubles(long long rubles) {
  if (rubles < 0) {
    throw DomainError("invalid amount");
  }
  return Money(rubles * 100);
}

Money Money::Zero() {
  return Money(0);
}

long long Money::Kopecks() const {
  return kopecks_;
}

std::string Money::ToString() const {
  std::ostringstream stream;
  stream << kopecks_ / 100 << "." << std::setw(2) << std::setfill('0')
         << kopecks_ % 100 << " RUB";
  return stream.str();
}

Money Money::operator+(const Money& other) const {
  return Money(kopecks_ + other.kopecks_);
}

Money Money::operator-(const Money& other) const {
  if (kopecks_ < other.kopecks_) {
    throw DomainError("invalid amount");
  }
  return Money(kopecks_ - other.kopecks_);
}

bool Money::operator==(const Money& other) const {
  return kopecks_ == other.kopecks_;
}

bool Money::operator!=(const Money& other) const {
  return !(*this == other);
}

bool Money::operator<(const Money& other) const {
  return kopecks_ < other.kopecks_;
}

bool Money::operator<=(const Money& other) const {
  return kopecks_ <= other.kopecks_;
}

bool Money::operator>(const Money& other) const {
  return kopecks_ > other.kopecks_;
}

bool Money::operator>=(const Money& other) const {
  return kopecks_ >= other.kopecks_;
}

}
