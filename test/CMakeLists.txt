cmake_minimum_required(VERSION 3.20)
project(AlertManagerTests
    VERSION 0.0.3
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest CONFIG REQUIRED)

add_executable(AlertManagerTests
    src/testAlertManager.cpp
)

target_link_libraries(AlertManagerTests
    PRIVATE
        alert_manager::AlertManagerLib
        GTest::gtest
        GTest::gtest_main
)

target_include_directories(AlertManagerTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME AlertManagerTests COMMAND AlertManagerTests)