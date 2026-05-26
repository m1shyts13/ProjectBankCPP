#pragma once

#include <string>

namespace banking::application {

struct CreateClientCommand {
  std::string full_name;
};

struct OpenAccountCommand {
  std::string client_id;
  std::string account_type;
};

struct DepositMoneyCommand {
  std::string account_id;
  long long kopecks;
};

struct WithdrawMoneyCommand {
  std::string account_id;
  long long kopecks;
};

struct TransferMoneyCommand {
  std::string from_account_id;
  std::string to_account_id;
  long long kopecks;
};

struct ShowBalanceCommand {
  std::string account_id;
};

struct ShowTransactionHistoryCommand {
  std::string account_id;
};

}  // namespace banking::application
