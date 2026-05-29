#pragma once

#include <string>
#include <vector>

namespace banking::application {

struct OperationResponse {
  bool success;
  std::string message;
  std::string id;
};

struct BalanceResponse {
  std::string account_id;
  std::string balance;
  long long kopecks;
};

struct TransactionDto {
  std::string id;
  std::string from_account_id;
  std::string to_account_id;
  std::string amount;
  std::string commission;
  std::string type;
  std::string created_at;
  std::string description;
};

struct TransactionHistoryResponse {
  std::vector<TransactionDto> transactions;
};

struct ClientSummaryResponse {
  std::string id;
  std::string full_name;
};

struct ListClientsResponse {
  std::vector<ClientSummaryResponse> clients;
};

struct AccountSummaryResponse {
  std::string id;
  std::string owner_id;
  std::string balance;
  std::string type;
  std::string status;
};

struct ListAccountsResponse {
  std::vector<AccountSummaryResponse> accounts;
};

}
