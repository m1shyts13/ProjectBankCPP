#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/account_repository.h"
#include "application/ports/client_repository.h"
#include "application/services/id_generator.h"
#include "domain/account/account_factory.h"

namespace banking::application {

class OpenAccountUseCase {
 public:
  OpenAccountUseCase(IClientRepository& client_repository,
                     IAccountRepository& account_repository,
                     IdGenerator& id_generator,
                     const domain::AccountFactory& account_factory);

  OperationResponse Execute(const OpenAccountCommand& command);

 private:
  static domain::AccountType ParseAccountType(const std::string& value);

  IClientRepository& client_repository_;
  IAccountRepository& account_repository_;
  IdGenerator& id_generator_;
  const domain::AccountFactory& account_factory_;
};

}
