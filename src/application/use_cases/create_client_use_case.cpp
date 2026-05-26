#include "application/use_cases/create_client_use_case.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

#include "domain/client/client.h"
#include "domain/client/client_id.h"

namespace {

bool IsBlank(const std::string& value) {
  return std::all_of(value.begin(), value.end(), [](unsigned char symbol) {
    return std::isspace(symbol);
  });
}

}  // namespace

namespace banking::application {

CreateClientUseCase::CreateClientUseCase(IClientRepository& client_repository,
                                         IdGenerator& id_generator)
    : client_repository_(client_repository), id_generator_(id_generator) {}

OperationResponse CreateClientUseCase::Execute(
    const CreateClientCommand& command) {
  if (IsBlank(command.full_name)) {
    throw std::runtime_error("client full name cannot be empty");
  }

  domain::ClientId client_id(id_generator_.Next());
  domain::Client client(client_id, command.full_name);
  client_repository_.Save(client);

  return OperationResponse{
      .success = true,
      .message = "Client created",
      .id = client.Id().Value(),
  };
}

}  // namespace banking::application
