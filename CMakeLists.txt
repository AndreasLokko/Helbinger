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

add_executable(HelbingerExe library.c library.h functions.c functions.h)
target_link_libraries(HelbingerExe libclang)
add_library(Helbinger SHARED library.c library.h functions.c functions.h)