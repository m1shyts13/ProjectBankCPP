#pragma once

#include "application/dto/responses.h"
#include "application/ports/client_repository.h"

namespace banking::application {

class ListClientsUseCase {
 public:
  explicit ListClientsUseCase(IClientRepository& client_repository);

  ListClientsResponse Execute();

 private:
  IClientRepository& client_repository_;
};

}  // namespace banking::application
