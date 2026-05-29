cmake_minimum_required(VERSION 3.20)

project(MiniBankingSystemDDD VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(banking_core
  src/domain/money/money.cpp
  src/domain/client/client.cpp
  src/domain/client/client_id.cpp
  src/domain/account/account_id.cpp
  src/domain/account/bank_account.cpp
  src/domain/account/account_factory.cpp
  src/domain/transaction/transaction.cpp
  src/domain/transaction/transaction_id.cpp
  src/domain/policies/commission_policy.cpp
  src/domain/policies/limit_policy.cpp
  src/domain/services/transfer_service.cpp
  src/domain/errors/domain_error.cpp
  src/application/services/id_generator.cpp
  src/application/use_cases/create_client_use_case.cpp
  src/application/use_cases/open_account_use_case.cpp
  src/application/use_cases/deposit_money_use_case.cpp
  src/application/use_cases/withdraw_money_use_case.cpp
  src/application/use_cases/transfer_money_use_case.cpp
  src/application/use_cases/show_balance_use_case.cpp
  src/application/use_cases/show_transaction_history_use_case.cpp
  src/application/use_cases/list_clients_use_case.cpp
  src/application/use_cases/list_accounts_use_case.cpp
  src/infrastructure/repositories/in_memory_client_repository.cpp
  src/infrastructure/repositories/in_memory_account_repository.cpp
  src/infrastructure/repositories/in_memory_transaction_repository.cpp
  src/infrastructure/repositories/json_client_repository.cpp
  src/infrastructure/repositories/json_account_repository.cpp
  src/infrastructure/repositories/json_transaction_repository.cpp
  src/infrastructure/facades/banking_facade.cpp
)

target_include_directories(banking_core PUBLIC src)
target_link_libraries(banking_core PUBLIC nlohmann_json::nlohmann_json)

add_executable(banking_app src/main.cpp)
target_link_libraries(banking_app PRIVATE banking_core)

add_executable(banking_tests tests/test_main.cpp)
target_link_libraries(banking_tests PRIVATE banking_core)

enable_testing()
add_test(NAME banking_tests COMMAND banking_tests)
