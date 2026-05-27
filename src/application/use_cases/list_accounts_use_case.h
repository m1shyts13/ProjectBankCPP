#pragma once

#include "application/dto/responses.h"
#include "application/ports/account_repository.h"

namespace banking::application {

class ListAccountsUseCase {
 public:
  explicit ListAccountsUseCase(IAccountRepository& account_repository);

  ListAccountsResponse Execute();

 private:
  IAccountRepository& account_repository_;
};

}  // namespace banking::application
