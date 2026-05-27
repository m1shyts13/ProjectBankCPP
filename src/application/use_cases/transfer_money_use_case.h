#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/account_repository.h"
#include "application/ports/transaction_repository.h"
#include "application/services/id_generator.h"
#include "domain/services/transfer_service.h"

namespace banking::application {

class TransferMoneyUseCase {
 public:
  TransferMoneyUseCase(IAccountRepository& account_repository,
                       ITransactionRepository& transaction_repository,
                       IdGenerator& transaction_id_generator,
                       const domain::TransferService& transfer_service);

  OperationResponse Execute(const TransferMoneyCommand& command);

 private:
  IAccountRepository& account_repository_;
  ITransactionRepository& transaction_repository_;
  IdGenerator& transaction_id_generator_;
  const domain::TransferService& transfer_service_;
};

}  // namespace banking::application
