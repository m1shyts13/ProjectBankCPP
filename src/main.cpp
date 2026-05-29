#include <exception>
#include <iostream>

#include "infrastructure/facades/banking_facade.h"

int main() {
  try {
    banking::infrastructure::BankingFacade bank;

    const auto client = bank.CreateClient("Ada Lovelace");
    const auto account = bank.OpenAccount(client.id, "debit");
    bank.Deposit(account.id, 250000);
    const auto balance = bank.ShowBalance(account.id);

    std::cout << "Mini Banking System DDD" << "\n";
    std::cout << "Client: " << client.id << "\n";
    std::cout << "Account: " << account.id << "\n";
    std::cout << "Balance: " << balance.balance << "\n";
  } catch (const std::exception& exception) {
    std::cerr << "Fatal error: " << exception.what() << "\n";
    return 1;
  }

  return 0;
}
