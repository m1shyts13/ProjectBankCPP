#include "infrastructure/repositories/in_memory_client_repository.h"

namespace banking::infrastructure {

void InMemoryClientRepository::Save(const domain::Client& client) {
  clients_.insert_or_assign(client.Id().Value(), client);
}

std::optional<domain::Client> InMemoryClientRepository::FindById(
    const domain::ClientId& id) {
  auto iterator = clients_.find(id.Value());
  if (iterator == clients_.end()) {
    return std::nullopt;
  }
  return iterator->second;
}

std::vector<domain::Client> InMemoryClientRepository::FindAll() {
  std::vector<domain::Client> result;
  result.reserve(clients_.size());
  for (const auto& entry : clients_) {
    result.push_back(entry.second);
  }
  return result;
}

}  // namespace banking::infrastructure
