#include "domain/client/client.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include "domain/errors/domain_error.h"

namespace banking::domain {

Client::Client(ClientId id, std::string full_name)
    : id_(std::move(id)), full_name_(std::move(full_name)) {
  if (std::all_of(full_name_.begin(), full_name_.end(),
                  [](unsigned char symbol) { return std::isspace(symbol); })) {
    throw DomainError("client full name cannot be empty");
  }
}

const ClientId& Client::Id() const {
  return id_;
}

const std::string& Client::FullName() const {
  return full_name_;
}

}
