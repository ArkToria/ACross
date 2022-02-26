CPMAddPackage(
    NAME gRPC
    GITHUB_REPOSITORY grpc/grpc
    VERSION 1.30.2
    )

# Find Protobuf
find_package(Protobuf REQUIRED)

# Find Generator Executable
find_program(PROTOBUF_PROTOC_EXECUTABLE protoc)

# Set Proto Name
list(APPEND PROTO_NAME_LISTS
    "v2ray_api"
    "v2ray_config"
    "acolors"
    "across"
    )

find_program(GRPC_CC_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Generate gRPC and protobuf Sources
foreach(PROTO_NAME ${PROTO_NAME_LISTS})
    set(GRPC_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")
    set(PB_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc")
    set(PB_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.h")
    set(PROTO_DIR "${CMAKE_SOURCE_DIR}/misc/")
    set(PROTO_FILE "${PROTO_DIR}/${PROTO_NAME}.proto")

    add_custom_command(
        OUTPUT "${GRPC_SOURCE}" "${GRPC_HEADER}" "${PB_SOURCE}" "${PB_HEADER}"
        COMMAND "${PROTOBUF_PROTOC_EXECUTABLE}"
        ARGS
            --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
            --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
            -I="${PROTO_DIR}"
            --plugin=protoc-gen-grpc="${GRPC_CC_PLUGIN_EXECUTABLE}"
            "${PROTO_FILE}"
        DEPENDS "${PROTO_FILE}")

    list(APPEND GRPC_HEADERS "${GRPC_HEADER}")
    list(APPEND GRPC_SOURCES "${GRPC_SOURCE}")

    list(APPEND PROTO_HEADERS "${PB_HEADER}")
    list(APPEND PROTO_SOURCES "${PB_SOURCE}")
endforeach()
