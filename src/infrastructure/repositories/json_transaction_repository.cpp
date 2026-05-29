#include "infrastructure/repositories/json_transaction_repository.h"

#include <fstream>

#include <nlohmann/json.hpp>

namespace banking::infrastructure {
namespace {

std::string TransactionTypeToString(domain::TransactionType type) {
  switch (type) {
    case domain::TransactionType::Deposit:
      return "deposit";
    case domain::TransactionType::Withdraw:
      return "withdraw";
    case domain::TransactionType::Transfer:
      return "transfer";
  }
  return "deposit";
}

domain::TransactionType TransactionTypeFromString(const std::string& value) {
  if (value == "withdraw") {
    return domain::TransactionType::Withdraw;
  }
  if (value == "transfer") {
    return domain::TransactionType::Transfer;
  }
  return domain::TransactionType::Deposit;
}

nlohmann::json OptionalAccountIdToJson(
    const std::optional<domain::AccountId>& account_id) {
  if (!account_id.has_value()) {
    return nullptr;
  }
  return account_id->Value();
}

std::optional<domain::AccountId> OptionalAccountIdFromJson(
    const nlohmann::json& value) {
  if (value.is_null()) {
    return std::nullopt;
  }
  return domain::AccountId(value.get<std::string>());
}

}

JsonTransactionRepository::JsonTransactionRepository(
    std::filesystem::path storage_directory)
    : file_path_(std::move(storage_directory) / "transactions.json") {
  Load();
}

void JsonTransactionRepository::Save(
    const domain::Transaction& transaction) {
  transactions_.push_back(transaction);
  Flush();
}

std::vector<domain::Transaction> JsonTransactionRepository::FindByAccountId(
    const domain::AccountId& account_id) {
  std::vector<domain::Transaction> result;
  for (const domain::Transaction& transaction : transactions_) {
    const bool is_source =
        transaction.FromAccountId().has_value() &&
        transaction.FromAccountId().value() == account_id;
    const bool is_target =
        transaction.ToAccountId().has_value() &&
        transaction.ToAccountId().value() == account_id;
    if (is_source || is_target) {
      result.push_back(transaction);
    }
  }
  return result;
}

std::vector<domain::Transaction> JsonTransactionRepository::FindAll() {
  return transactions_;
}

void JsonTransactionRepository::Load() {
  if (!std::filesystem::exists(file_path_)) {
    return;
  }

  std::ifstream stream(file_path_);
  nlohmann::json document;
  stream >> document;

  transactions_.clear();
  for (const nlohmann::json& item :
       document.value("transactions", nlohmann::json::array())) {
    transactions_.push_back(domain::Transaction(
        domain::TransactionId(item.at("id").get<std::string>()),
        OptionalAccountIdFromJson(item.at("from_account_id")),
        OptionalAccountIdFromJson(item.at("to_account_id")),
        domain::Money(item.at("amount_kopecks").get<long long>()),
        domain::Money(item.at("commission_kopecks").get<long long>()),
        TransactionTypeFromString(item.at("type").get<std::string>()),
        item.at("created_at").get<std::string>(),
        item.at("description").get<std::string>()));
  }
}

void JsonTransactionRepository::Flush() const {
  std::filesystem::create_directories(file_path_.parent_path());

  nlohmann::json document;
  document["transactions"] = nlohmann::json::array();
  for (const domain::Transaction& transaction : transactions_) {
    document["transactions"].push_back({
        {"id", transaction.Id().Value()},
        {"from_account_id", OptionalAccountIdToJson(transaction.FromAccountId())},
        {"to_account_id", OptionalAccountIdToJson(transaction.ToAccountId())},
        {"amount_kopecks", transaction.Amount().Kopecks()},
        {"commission_kopecks", transaction.Commission().Kopecks()},
        {"type", TransactionTypeToString(transaction.Type())},
        {"created_at", transaction.CreatedAt()},
        {"description", transaction.Description()},
    });
  }

  std::ofstream stream(file_path_);
  stream << document.dump(2);
}

}
