include(FetchContent)

cmake_minimum_required(VERSION 3.30)
project(std_math C)

set(CMAKE_C_STANDARD 11)

add_library(std_math STATIC std_math.c std_math.h)

if(NOT FLUENT_LIBC_RELEASE) # Manually add libraries only if not in release mode
    FetchContent_Declare(
            types
            GIT_REPOSITORY https://github.com/rodrigoo-r/types
            GIT_TAG        master
    )

    FetchContent_MakeAvailable(types)

    target_include_directories(std_math PRIVATE ${CMAKE_BINARY_DIR}/_deps/types-src)
    target_link_libraries(std_math PRIVATE types)
endif ()
