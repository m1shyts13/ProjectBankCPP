#include "domain/errors/domain_error.h"

namespace banking::domain {

DomainError::DomainError(const std::string& message) : std::runtime_error(message) {}

}  // namespace banking::domain
