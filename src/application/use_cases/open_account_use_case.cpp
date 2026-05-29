#include "application/use_cases/open_account_use_case.h"

#include <stdexcept>

#include "domain/account/account_id.h"
#include "domain/client/client_id.h"

namespace banking::application {

OpenAccountUseCase::OpenAccountUseCase(
    IClientRepository& client_repository, IAccountRepository& account_repository,
    IdGenerator& id_generator, const domain::AccountFactory& account_factory)
    : client_repository_(client_repository),
      account_repository_(account_repository),
      id_generator_(id_generator),
      account_factory_(account_factory) {}

OperationResponse OpenAccountUseCase::Execute(
    const OpenAccountCommand& command) {
  domain::ClientId client_id(command.client_id);
  if (!client_repository_.FindById(client_id).has_value()) {
    throw std::runtime_error("client not found");
  }

  const domain::AccountType account_type = ParseAccountType(command.account_type);
  domain::AccountId account_id(id_generator_.Next());
  domain::BankAccount account =
      account_factory_.Create(account_id, client_id, account_type);
  account_repository_.Save(account);

  return OperationResponse{
      .success = true,
      .message = "Account opened",
      .id = account.Id().Value(),
  };
}

domain::AccountType OpenAccountUseCase::ParseAccountType(
    const std::string& value) {
  if (value == "debit" || value == "Debit") {
    return domain::AccountType::Debit;
  }
  if (value == "savings" || value == "Savings") {
    return domain::AccountType::Savings;
  }
  throw std::runtime_error("unknown account type");
}

}
