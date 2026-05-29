# Mini Banking System DDD

The project implements clients, accounts, money transactions, transfers, fees, limits, and in-memory data storage.

## Features

- creating clients;
- opening debit and savings accounts;
- depositing and withdrawing funds;
- transferring between accounts with a fee;
- checking the limit for a single transaction;
- viewing the balance;
- viewing transaction history;
- obtaining lists of clients and accounts.

## Build

```bash
cmake -S . -B build
cmake --build build
```

Running the demo composition root:

```bash
./build/banking_app
```

Running with JSON storage:

```bash
./build/banking_app data
```

Tests:

```bash
ctest --test-dir build --output-on-failure
```

## Architecture

The project is divided into layers:

- `domain` - business logic: clients, accounts, money, transactions, policies, and domain services;
- `application` - use cases, repository ports, and identifier generation;
- `infrastructure` - port implementations, in-memory and JSON repositories.

Main DDD elements:

- Entities: `Client`, `BankAccount`, `Transaction`;
- Value Objects: `Money`, `ClientId`, `AccountId`, `TransactionId`;
- Aggregate Root: `BankAccount`;
- Domain Service: `TransferService`;
- Repository Ports: `IClientRepository`, `IAccountRepository`, `ITransactionRepository`.

## Patterns and Principles

- Strategy - commission and limit policies: `ICommissionPolicy`, `ILimitPolicy`;
- Repository - repository interfaces in the application and implementations in the infrastructure;
- Factory - account creation via `AccountFactory`, id creation via `IdGenerator`;
- Facade - `BankingFacade` hides the creation of repositories, policies, and use cases;
- Dependency Inversion - the application depends on abstractions, the infrastructure implements them;
- Single Responsibility - entities, use cases, and repositories have separate areas of responsibility.
