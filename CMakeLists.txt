cmake_minimum_required(VERSION 3.16)

project(MyManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------
# Main executable
# -----------------------

add_executable(MyManager
    main.cpp
    crypto/crypto_manager.cpp
    storage/file_storage.cpp
    app/vault_manager.cpp
)

target_include_directories(MyManager PRIVATE
    ${PROJECT_SOURCE_DIR}
)

# -----------------------
# Tests
# -----------------------

add_executable(tests
    tests/test_crypto.cpp
    tests/test_storage.cpp
    tests/test_vault.cpp

    crypto/crypto_manager.cpp
    storage/file_storage.cpp
    app/vault_manager.cpp
)

target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}
    tests
)

# -----------------------
# libsodium
# -----------------------

find_package(unofficial-sodium CONFIG REQUIRED)

target_link_libraries(MyManager PRIVATE unofficial-sodium::sodium)
target_link_libraries(tests PRIVATE unofficial-sodium::sodium)

# -----------------------
# Testing
# -----------------------

enable_testing()

add_test(NAME VaultTests COMMAND tests)