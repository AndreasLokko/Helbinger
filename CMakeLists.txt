cmake_minimum_required(VERSION 3.16)
project(Helbinger)

set(CMAKE_C_STANDARD 11)

find_package(Clang REQUIRED)
if (Clang_FOUND)
    include_directories(${CLANG_INCLUDE_DIRS})
else()
    message("Libclang not found. Please install package libclang-dev")
endif ()

message(${CLANG_INCLUDE_DIRS})
add_executable(HelbingerExe example.c library.h library.c internal.c internal.h)
target_link_libraries(HelbingerExe libclang)
add_library(Helbinger SHARED library.h library.c internal.c internal.h)

set(compile_options -Wall -Wextra -pedantic -O2) # -fsanitize=address
target_compile_options(HelbingerExe PUBLIC ${compile_options})