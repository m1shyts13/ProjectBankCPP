#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/client_repository.h"
#include "application/services/id_generator.h"

namespace banking::application {

class CreateClientUseCase {
 public:
  CreateClientUseCase(IClientRepository& client_repository,
                      IdGenerator& id_generator);

  OperationResponse Execute(const CreateClientCommand& command);

 private:
  IClientRepository& client_repository_;
  IdGenerator& id_generator_;
};

}
