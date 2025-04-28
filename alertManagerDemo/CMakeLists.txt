cmake_minimum_required(VERSION 3.20)
project(AlertManagerDemo
    VERSION 0.0.3
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(AlertManagerDemo
    src/AlertManagerDemo.cpp
)

target_link_libraries(AlertManagerDemo
    PRIVATE
        alert_manager::AlertManagerLib
)

target_include_directories(AlertManagerDemo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(GNUInstallDirs)

install(TARGETS AlertManagerDemo
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)