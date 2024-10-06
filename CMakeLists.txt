cmake_minimum_required(VERSION 3.14)

project(PBTest CXX)

find_package(Protobuf REQUIRED)

# 设置 proto 文件的路径
# file(GLOB HEADERS "${INCLUDE_DIR}/*.h")  file(GLOB XXX PATH)
file(GLOB PROTO_FILES "proto/*.proto")
message(STATUS "Found proto files: ${PROTO_FILES}")

# 指定 proto 格式文件生成的位置
set(PROTO_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto_generate)
# 确保目录一定存在
file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})

# 生成 C++ 代码
set(PROTO_SRCS "")
set(PROTO_HDRS "")

foreach(proto_file ${PROTO_FILES})
    get_filename_component(file_name ${proto_file} NAME_WE)
    
    set(proto_src "${PROTO_OUTPUT_DIR}/${file_name}.pb.cc")
    set(proto_hdr "${PROTO_OUTPUT_DIR}/${file_name}.pb.h")

    # 添加自定义命令，如果生成文件不存在或者 proto 文件更新，才重新生成
    # 通过 OUTPUT 指定生成的文件，并使用 DEPENDS 让它依赖于相应的 .proto 文件。如果 .proto 文件发生变化，CMake 会触发重新生成。
    add_custom_command(
        OUTPUT ${proto_src} ${proto_hdr}
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out=${PROTO_OUTPUT_DIR} --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto ${proto_file}
        # protoc -I=<path1> -I=<path2> --cpp_out=<output_dir> your_proto_file.proto 可以指定多个 -I 选项，以添加多个搜索路径。
        DEPENDS ${proto_file}
        COMMENT "Generating ${proto_src} and ${proto_hdr}"
    )

    list(APPEND PROTO_SRCS ${proto_src})
    list(APPEND PROTO_HDRS ${proto_hdr})
endforeach()

# 创建自定义目标，确保在构建之前执行 proto 生成
add_custom_target(generate_protos DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})

# 包含生成的源文件和头文件
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

aux_source_directory(src SRC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${PROTO_OUTPUT_DIR})

# 添加可执行文件
add_executable(pb_test ${SRC} ${PROTO_SRCS} ${PROTO_HDRS})
add_dependencies(pb_test generate_protos)  # 依赖生成目标

target_link_libraries(pb_test protobuf)


# 学习记录
# 使用 protobuf_generate_cpp()是没法执行 XX.proto.cc .h 这种文件的生成路径的。
# # 一、详细解释
#     1、使用 add_custom_command：
#     通过 OUTPUT 指定生成的文件，并使用 DEPENDS 让它依赖于相应的 .proto 文件。如果 .proto 文件发生变化，CMake 会触发重新生成。
#     这样只有在源 .proto 文件被修改时，才会重新生成 .pb.cc 和 .pb.h 文件。

#     2、使用 add_custom_target：
#     创建一个名为 generate_protos 的自定义目标，使得 pb_test 在构建之前确保执行 proto 文件的生成。

# 二、DEPENDS
# DEPENDS 关键字用于指定构建目标的依赖关系，当你在一个构建目标中使用 DEPENDS，你可以指定该目标依赖于其他文件或目标。
# DEPENDS ${proto_file} 当你指定某个文件作为依赖时，CMake 会监视这个文件的更改。如果该文件发生更改，CMake 会在下次构建时自动重新构建依赖于该文件的目标。

# 三、list APPEND 将生成的文件打包到 PROTO_SRCS列表中，这样做的目的是为了在创建可执行文件时，将这些生成的文件包含进来
# list(APPEND PROTO_SRCS ${proto_src})
# 后续 add_executable(pb_test ${SRC} ${PROTO_SRCS} ${PROTO_HDRS}) 这里用到了。

# 四、add_custom_target 和 add_dependencies 的解释
# add_custom_target(generate_protos DEPENDS ${PROTO_SRCS} ${PROTO_HDRS}):
# 这一行创建了一个名为 generate_protos 的自定义目标。这个目标的依赖项是 ${PROTO_SRCS} 和 ${PROTO_HDRS}，也就是生成的 Protobuf 源文件和头文件。
# 当你构建项目时，CMake 会检查 generate_protos 这个目标是否是最新的。如果依赖的文件（即生成的 Protobuf 文件）存在并且是最新的，CMake 会跳过这个目标的构建。

# add_dependencies(pb_test generate_protos):
# 这一行表示 pb_test 目标依赖于 generate_protos 目标。这意味着在构建 pb_test 之前，CMake 会确保 generate_protos 目标已经完成。
# 如果 generate_protos 被触发（例如，当 Protobuf 文件发生更改时），CMake 会首先生成最新的 .cc 和 .h 文件，然后再构建 pb_test 可执行文件。

# protobuf学习参考链接: https://subingwen.cn/cpp/protobuf/