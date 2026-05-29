#include "infrastructure/repositories/json_account_repository.h"

#include <fstream>

#include <nlohmann/json.hpp>

namespace banking::infrastructure {
namespace {

std::string AccountTypeToString(domain::AccountType type) {
  switch (type) {
    case domain::AccountType::Debit:
      return "debit";
    case domain::AccountType::Savings:
      return "savings";
  }
  return "debit";
}

domain::AccountType AccountTypeFromString(const std::string& value) {
  if (value == "savings") {
    return domain::AccountType::Savings;
  }
  return domain::AccountType::Debit;
}

}

JsonAccountRepository::JsonAccountRepository(
    std::filesystem::path storage_directory)
    : file_path_(std::move(storage_directory) / "accounts.json") {
  Load();
}

void JsonAccountRepository::Save(const domain::BankAccount& account) {
  accounts_.insert_or_assign(account.Id().Value(), account);
  Flush();
}

std::optional<domain::BankAccount> JsonAccountRepository::FindById(
    const domain::AccountId& id) {
  auto iterator = accounts_.find(id.Value());
  if (iterator == accounts_.end()) {
    return std::nullopt;
  }
  return iterator->second;
}

std::vector<domain::BankAccount> JsonAccountRepository::FindByClientId(
    const domain::ClientId& client_id) {
  std::vector<domain::BankAccount> result;
  for (const auto& entry : accounts_) {
    if (entry.second.OwnerId() == client_id) {
      result.push_back(entry.second);
    }
  }
  return result;
}

std::vector<domain::BankAccount> JsonAccountRepository::FindAll() {
  std::vector<domain::BankAccount> result;
  result.reserve(accounts_.size());
  for (const auto& entry : accounts_) {
    result.push_back(entry.second);
  }
  return result;
}

void JsonAccountRepository::Load() {
  if (!std::filesystem::exists(file_path_)) {
    return;
  }

  std::ifstream stream(file_path_);
  nlohmann::json document;
  stream >> document;

  accounts_.clear();
  for (const nlohmann::json& item : document.value("accounts", nlohmann::json::array())) {
    domain::BankAccount account(
        domain::AccountId(item.at("id").get<std::string>()),
        domain::ClientId(item.at("owner_id").get<std::string>()),
        AccountTypeFromString(item.at("type").get<std::string>()));
    const long long kopecks = item.at("balance_kopecks").get<long long>();
    if (kopecks > 0) {
      account.Deposit(domain::Money(kopecks));
    }
    if (item.value("closed", false)) {
      account.Close();
    }
    accounts_.insert_or_assign(account.Id().Value(), account);
  }
}

void JsonAccountRepository::Flush() const {
  std::filesystem::create_directories(file_path_.parent_path());

  nlohmann::json document;
  document["accounts"] = nlohmann::json::array();
  for (const auto& entry : accounts_) {
    const domain::BankAccount& account = entry.second;
    document["accounts"].push_back({
        {"id", account.Id().Value()},
        {"owner_id", account.OwnerId().Value()},
        {"type", AccountTypeToString(account.Type())},
        {"balance_kopecks", account.Balance().Kopecks()},
        {"closed", account.IsClosed()},
    });
  }

  std::ofstream stream(file_path_);
  stream << document.dump(2);
}

}
