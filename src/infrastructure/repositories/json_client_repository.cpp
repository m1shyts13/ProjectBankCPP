#include "infrastructure/repositories/json_client_repository.h"

#include <fstream>

#include <nlohmann/json.hpp>

namespace banking::infrastructure {

JsonClientRepository::JsonClientRepository(
    std::filesystem::path storage_directory)
    : file_path_(std::move(storage_directory) / "clients.json") {
  Load();
}

void JsonClientRepository::Save(const domain::Client& client) {
  clients_.insert_or_assign(client.Id().Value(), client);
  Flush();
}

std::optional<domain::Client> JsonClientRepository::FindById(
    const domain::ClientId& id) {
  auto iterator = clients_.find(id.Value());
  if (iterator == clients_.end()) {
    return std::nullopt;
  }
  return iterator->second;
}

std::vector<domain::Client> JsonClientRepository::FindAll() {
  std::vector<domain::Client> result;
  result.reserve(clients_.size());
  for (const auto& entry : clients_) {
    result.push_back(entry.second);
  }
  return result;
}

void JsonClientRepository::Load() {
  if (!std::filesystem::exists(file_path_)) {
    return;
  }

  std::ifstream stream(file_path_);
  nlohmann::json document;
  stream >> document;

  clients_.clear();
  for (const nlohmann::json& item : document.value("clients", nlohmann::json::array())) {
    domain::Client client(domain::ClientId(item.at("id").get<std::string>()),
                          item.at("full_name").get<std::string>());
    clients_.insert_or_assign(client.Id().Value(), client);
  }
}

void JsonClientRepository::Flush() const {
  std::filesystem::create_directories(file_path_.parent_path());

  nlohmann::json document;
  document["clients"] = nlohmann::json::array();
  for (const auto& entry : clients_) {
    const domain::Client& client = entry.second;
    document["clients"].push_back({
        {"id", client.Id().Value()},
        {"full_name", client.FullName()},
    });
  }

  std::ofstream stream(file_path_);
  stream << document.dump(2);
}

}
