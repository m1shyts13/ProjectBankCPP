#pragma once

#include "application/dto/commands.h"
#include "application/dto/responses.h"
#include "application/ports/transaction_repository.h"

namespace banking::application {

class ShowTransactionHistoryUseCase {
 public:
  explicit ShowTransactionHistoryUseCase(
      ITransactionRepository& transaction_repository);

  TransactionHistoryResponse Execute(
      const ShowTransactionHistoryCommand& command);

 private:
  ITransactionRepository& transaction_repository_;
};

}
