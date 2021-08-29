# Find Protobuf
find_package(Protobuf REQUIRED)

# Find Generator Executable
find_program(PROTOBUF_PROTOC_EXECUTABLE protoc)

# Set Proto Name
set(API_PROTO_NAME "v2ray_api")

# Set Libraries
set(PROTOBUF_LIBPROTOBUF_LIB protobuf::libprotobuf)
set(Protobuf_USE_STATIC_LIBS ON)

# File Proto File
get_filename_component(API_PROTO_PATH "${CMAKE_SOURCE_DIR}/misc/" ABSOLUTE)
get_filename_component(API_PROTO "${API_PROTO_PATH}/${API_PROTO_NAME}.proto" ABSOLUTE)

# Generated Sources
protobuf_generate_cpp(API_PROTO_SOURCE API_PROTO_HEADER "${API_PROTO}")
