# Find Protobuf
find_package(Protobuf REQUIRED)

# Find Generator Executable
find_program(PROTOBUF_PROTOC_EXECUTABLE protoc)

# Set Proto Name
set(V2RAY_API_PROTO_NAME "v2ray_api")
set(V2RAY_CONFIG_PROTO_NAME "v2ray_config")
set(ACROSS_PROTO_NAME "across")

# Set Libraries
set(PROTOBUF_LIBPROTOBUF_LIB protobuf::libprotobuf)
set(Protobuf_USE_STATIC_LIBS ON)

# File Proto File
get_filename_component(PROTO_DIR "${CMAKE_SOURCE_DIR}/misc/" ABSOLUTE)
get_filename_component(V2RAY_API_PROTO "${PROTO_DIR}/${V2RAY_API_PROTO_NAME}.proto" ABSOLUTE)
get_filename_component(V2RAY_CONFIG_PROTO "${PROTO_DIR}/${V2RAY_CONFIG_PROTO_NAME}.proto" ABSOLUTE)
get_filename_component(ACROSS_PROTO "${PROTO_DIR}/${ACROSS_PROTO_NAME}.proto" ABSOLUTE)

# Generated Sources
protobuf_generate_cpp(V2RAY_API_PROTO_SOURCE V2RAY_API_PROTO_HEADER "${V2RAY_API_PROTO}")
protobuf_generate_cpp(V2RAY_API_CONFIG_SOURCE V2RAY_CONFIG_PROTO_HEADER "${V2RAY_CONFIG_PROTO}")
protobuf_generate_cpp(ACROSS_PROTO_SOURCE ACROSS_PROTO_HEADER "${ACROSS_PROTO}")
