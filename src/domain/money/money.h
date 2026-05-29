#pragma once

#include <string>

namespace banking::domain {

class Money {
 public:
  explicit Money(long long kopecks);

  static Money Rubles(long long rubles);
  static Money Zero();

  long long Kopecks() const;
  std::string ToString() const;

  Money operator+(const Money& other) const;
  Money operator-(const Money& other) const;

  bool operator==(const Money& other) const;
  bool operator!=(const Money& other) const;
  bool operator<(const Money& other) const;
  bool operator<=(const Money& other) const;
  bool operator>(const Money& other) const;
  bool operator>=(const Money& other) const;

 private:
  long long kopecks_;
};

}
