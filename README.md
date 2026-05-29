# Mini Banking System DDD

Учебное C++20-приложение, показывающее ядро банковской системы через DDD и Clean Architecture. Проект моделирует клиентов, счета, денежные операции, переводы, комиссии, лимиты и хранение данных в памяти.

## Возможности

- создание клиентов;
- открытие дебетовых и накопительных счетов;
- пополнение и снятие денег;
- перевод между счетами с комиссией;
- проверка лимита одной операции;
- просмотр баланса;
- просмотр истории транзакций;
- получение списков клиентов и счетов.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск демонстрационного composition root:

```bash
./build/banking_app
```

## Архитектура

Проект разделен на слои:

- `domain` - бизнес-логика: клиенты, счета, деньги, транзакции, политики и доменные сервисы;
- `application` - use cases, DTO, порты репозиториев и генерация идентификаторов;
- `infrastructure` - реализации портов, сейчас используются in-memory репозитории.

Основные DDD элементы:

- Entities: `Client`, `BankAccount`, `Transaction`;
- Value Objects: `Money`, `ClientId`, `AccountId`, `TransactionId`;
- Aggregate Root: `BankAccount`;
- Domain Service: `TransferService`;
- Repository Ports: `IClientRepository`, `IAccountRepository`, `ITransactionRepository`.

## Паттерны и принципы

- Strategy - политики комиссий и лимитов: `ICommissionPolicy`, `ILimitPolicy`;
- Repository - интерфейсы репозиториев в application и реализации в infrastructure;
- Factory - создание счетов через `AccountFactory`, создание id через `IdGenerator`;
- Dependency Inversion - application зависит от абстракций, infrastructure их реализует;
- Single Responsibility - сущности, use cases и репозитории имеют отдельные зоны ответственности.
