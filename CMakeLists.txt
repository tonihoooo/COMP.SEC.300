cmake_minimum_required(VERSION 3.16)

project(MyManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Main executable ----

add_executable(MyManager
    main.cpp
    crypto/crypto_manager.cpp
    storage/file_storage.cpp
    app/vault_manager.cpp
)

target_include_directories(MyManager PRIVATE
    ${PROJECT_SOURCE_DIR}
)

# ---- libsodium ----
# Change this path if your libsodium is elsewhere

target_include_directories(MyManager PRIVATE
    C:/msys64/mingw64/include
)

target_link_directories(MyManager PRIVATE
    C:/msys64/mingw64/lib
)

target_link_libraries(MyManager PRIVATE sodium)

# ---- Tests ----

add_executable(tests
    tests/test_main.cpp
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
    C:/msys64/mingw64/include
)

target_link_directories(tests PRIVATE
    C:/msys64/mingw64/lib
)

target_link_libraries(tests PRIVATE sodium)

enable_testing()

add_test(NAME VaultTests COMMAND tests)