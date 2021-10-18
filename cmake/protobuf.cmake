# Find Protobuf
find_package(Protobuf REQUIRED)

# Find Generator Executable
find_program(PROTOBUF_PROTOC_EXECUTABLE protoc)

# Set Proto Name
list(APPEND
    PROTO_NAME_LISTS
    "v2ray_api"
    "v2ray_config"
    "across"
    )

# Set Libraries
set(PROTOBUF_LIBPROTOBUF_LIB protobuf::libprotobuf)
set(Protobuf_USE_STATIC_LIBS ON)

# Generated Sources
get_filename_component(PROTO_DIR "${CMAKE_SOURCE_DIR}/misc/" ABSOLUTE)

foreach(PROTO_NAME IN LISTS PROTO_NAME_LISTS)
    get_filename_component(PROTO_FILE "${PROTO_DIR}/${PROTO_NAME}.proto" ABSOLUTE)
    protobuf_generate_cpp(PROTO_SOURCE PROTO_HEADER "${PROTO_FILE}")
    list(APPEND PROTO_SOURCES ${PROTO_SOURCE})
    list(APPEND PROTO_HEADERS ${PROTO_HEADER})
endforeach()
