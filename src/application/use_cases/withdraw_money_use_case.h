#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/account_repository.h"
#include "application/ports/transaction_repository.h"
#include "application/services/id_generator.h"

namespace banking::application {

class WithdrawMoneyUseCase {
 public:
  WithdrawMoneyUseCase(IAccountRepository& account_repository,
                       ITransactionRepository& transaction_repository,
                       IdGenerator& transaction_id_generator);

  OperationResponse Execute(const WithdrawMoneyCommand& command);

 private:
  IAccountRepository& account_repository_;
  ITransactionRepository& transaction_repository_;
  IdGenerator& transaction_id_generator_;
};

}
