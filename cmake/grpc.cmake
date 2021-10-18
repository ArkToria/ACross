CPMAddPackage(
    NAME gRPC
    GITHUB_REPOSITORY grpc/grpc
    VERSION 1.30.2
    )

find_program(GRPC_CC_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Generate gRPC Sources
foreach(PROTO_NAME PROTO_FILE IN ZIP_LISTS PROTO_NAME_LISTS PTORO_FILES)
    set(GRPC_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HEADER "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.grpc.pb.h")

    add_custom_command(
        OUTPUT "${GRPC_SOURCE}" "${GRPC_HEADER}"
        COMMAND "${PROTOBUF_PROTOC_EXECUTABLE}"
        ARGS --grpc_out="${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
        -I="${PROTO_DIR}"
        --plugin=protoc-gen-grpc="${GRPC_CC_PLUGIN_EXECUTABLE}"
        "${PROTO_FILE}"
        DEPENDS "${PROTO_FILE}"
        )

    list(APPEND GRPC_SOURCES
        "${GRPC_SOURCE}"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_NAME}.pb.cc"
        )

    list(APPEND GRPC_HEADERS "${GRPC_HEADER}")
endforeach()
