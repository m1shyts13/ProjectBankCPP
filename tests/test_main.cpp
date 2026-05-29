#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "domain/account/bank_account.h"
#include "domain/client/client.h"
#include "domain/errors/domain_error.h"
#include "domain/money/money.h"
#include "domain/transaction/transaction.h"
#include "infrastructure/facades/banking_facade.h"
#include "infrastructure/repositories/json_account_repository.h"
#include "infrastructure/repositories/json_client_repository.h"
#include "infrastructure/repositories/json_transaction_repository.h"

namespace {

void Expect(bool condition, const std::string& message) {
  if (!condition) {
    throw std::runtime_error(message);
  }
}

template <typename T>
void ExpectEqual(const T& actual, const T& expected,
                 const std::string& message) {
  if (!(actual == expected)) {
    throw std::runtime_error(message);
  }
}

template <typename Exception, typename Callable>
void ExpectThrows(Callable callable, const std::string& message) {
  try {
    callable();
  } catch (const Exception&) {
    return;
  }
  throw std::runtime_error(message);
}

std::filesystem::path MakeTempDirectory(const std::string& name) {
  const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
  auto path = std::filesystem::temp_directory_path() /
              ("banking-" + name + "-" + std::to_string(now));
  std::filesystem::remove_all(path);
  std::filesystem::create_directories(path);
  return path;
}

void TestMoney() {
  banking::domain::Money amount(1250);
  ExpectEqual(amount.Kopecks(), 1250LL, "money stores kopecks");
  ExpectEqual(amount.ToString(), std::string("12.50 RUB"),
              "money formats rubles");
  ExpectEqual((amount + banking::domain::Money(750)).Kopecks(), 2000LL,
              "money adds values");
  ExpectEqual((amount - banking::domain::Money(250)).Kopecks(), 1000LL,
              "money subtracts values");
  ExpectThrows<banking::domain::DomainError>(
      [] { banking::domain::Money(-1); }, "money rejects negative values");
}

void TestJsonRepositories() {
  auto directory = MakeTempDirectory("json-repositories");

  {
    banking::infrastructure::JsonClientRepository clients(directory);
    clients.Save(banking::domain::Client(banking::domain::ClientId("1"),
                                         "Ada Lovelace"));

    banking::domain::BankAccount account(
        banking::domain::AccountId("account-1"),
        banking::domain::ClientId("1"),
        banking::domain::AccountType::Savings);
    account.Deposit(banking::domain::Money(12345));
    banking::infrastructure::JsonAccountRepository accounts(directory);
    accounts.Save(account);

    banking::domain::Transaction transaction(
        banking::domain::TransactionId("transaction-1"), std::nullopt,
        banking::domain::AccountId("account-1"), banking::domain::Money(5000),
        banking::domain::Money::Zero(), banking::domain::TransactionType::Deposit,
        "2026-05-29 18:30:00", "Deposit");
    banking::infrastructure::JsonTransactionRepository transactions(directory);
    transactions.Save(transaction);
  }

  {
    banking::infrastructure::JsonClientRepository clients(directory);
    auto client = clients.FindById(banking::domain::ClientId("1"));
    Expect(client.has_value(), "json client repository loads client");
    ExpectEqual(client->FullName(), std::string("Ada Lovelace"),
                "json client full name");

    banking::infrastructure::JsonAccountRepository accounts(directory);
    auto account =
        accounts.FindById(banking::domain::AccountId("account-1"));
    Expect(account.has_value(), "json account repository loads account");
    ExpectEqual(account->Balance().Kopecks(), 12345LL,
                "json account balance");
    Expect(account->Type() == banking::domain::AccountType::Savings,
           "json account type");

    banking::infrastructure::JsonTransactionRepository transactions(directory);
    auto history =
        transactions.FindByAccountId(banking::domain::AccountId("account-1"));
    ExpectEqual(history.size(), static_cast<std::size_t>(1),
                "json transaction repository loads history");
    ExpectEqual(history.front().Amount().Kopecks(), 5000LL,
                "json transaction amount");
  }

  std::filesystem::remove_all(directory);
}

void TestJsonFacade() {
  auto directory = MakeTempDirectory("json-facade");
  std::string account_id;

  {
    banking::infrastructure::BankingFacade bank(directory);
    const auto client = bank.CreateClient("Grace Hopper");
    const auto account = bank.OpenAccount(client.id, "debit");
    bank.Deposit(account.id, 250000);
    account_id = account.id;
  }

  {
    banking::infrastructure::BankingFacade bank(directory);
    const auto balance = bank.ShowBalance(account_id);
    ExpectEqual(balance.kopecks, 250000LL, "facade restores json balance");

    const auto history = bank.ShowTransactionHistory(account_id);
    ExpectEqual(history.transactions.size(), static_cast<std::size_t>(1),
                "facade restores json history");

    const auto client = bank.CreateClient("Katherine Johnson");
    ExpectEqual(client.id, std::string("2"),
                "facade continues client ids after load");
  }

  std::filesystem::remove_all(directory);
}

}

int main() {
  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"money", TestMoney},
      {"json repositories", TestJsonRepositories},
      {"json facade", TestJsonFacade},
  };

  for (const auto& test : tests) {
    try {
      test.second();
      std::cout << "[PASS] " << test.first << "\n";
    } catch (const std::exception& exception) {
      std::cerr << "[FAIL] " << test.first << ": " << exception.what()
                << "\n";
      return 1;
    }
  }

  return 0;
}
