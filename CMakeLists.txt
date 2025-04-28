cmake_minimum_required(VERSION 3.20)
project(AlertManager
    VERSION 0.0.3
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories for all build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(CTest)
enable_testing()

add_subdirectory(protobuf)
add_subdirectory(alertManagerLib)
add_subdirectory(alertManagerDemo)
add_subdirectory(test)