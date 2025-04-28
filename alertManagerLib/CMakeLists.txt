cmake_minimum_required(VERSION 3.20)
project(AlertManagerLib
    VERSION 0.0.3
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find protobuf (from Conan)
find_package(protobuf CONFIG REQUIRED)

add_library(AlertManagerLib
    src/AlertManager.cpp
)

add_library(alert_manager::AlertManagerLib ALIAS AlertManagerLib)

set_target_properties(AlertManagerLib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME AlertManagerLib
)

target_include_directories(AlertManagerLib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AlertManagerLib
    PRIVATE
        protobuf::protobuf
)

include(GNUInstallDirs)

install(TARGETS AlertManagerLib
    EXPORT AlertManagerLibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/alert_manager
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

include(CMakePackageConfigHelpers)

install(EXPORT AlertManagerLibTargets
    FILE AlertManagerLibTargets.cmake
    NAMESPACE alert_manager::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AlertManagerLib
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/AlertManagerLibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AlertManagerLibConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AlertManagerLib
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/AlertManagerLibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/AlertManagerLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AlertManagerLib
)

