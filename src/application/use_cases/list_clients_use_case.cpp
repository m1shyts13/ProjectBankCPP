#include "application/use_cases/list_clients_use_case.h"

namespace banking::application {

ListClientsUseCase::ListClientsUseCase(IClientRepository& client_repository)
    : client_repository_(client_repository) {}

ListClientsResponse ListClientsUseCase::Execute() {
  ListClientsResponse response;
  for (const domain::Client& client : client_repository_.FindAll()) {
    response.clients.push_back(ClientSummaryResponse{
        .id = client.Id().Value(),
        .full_name = client.FullName(),
    });
  }
  return response;
}

}  // namespace banking::application
