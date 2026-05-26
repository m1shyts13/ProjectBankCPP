#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/account_repository.h"

namespace banking::application {

class ShowBalanceUseCase {
 public:
  explicit ShowBalanceUseCase(IAccountRepository& account_repository);

  BalanceResponse Execute(const ShowBalanceCommand& command);

 private:
  IAccountRepository& account_repository_;
};

}  // namespace banking::application
