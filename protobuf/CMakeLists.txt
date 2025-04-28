cmake_minimum_required(VERSION 3.20)
project(AlertProto
    VERSION 0.0.3
    LANGUAGES CXX
)

find_package(protobuf CONFIG REQUIRED)

# Generate protobuf code
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS alert.proto)

add_library(AlertProto
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_include_directories(AlertProto
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AlertProto
    PUBLIC
        protobuf::libprotobuf
)

install(TARGETS AlertProto
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${PROTO_HDRS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/alert_manager
)
