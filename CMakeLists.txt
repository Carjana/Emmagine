cmake_minimum_required(VERSION 4.0)
project(Emmagine)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_definitions(RELEASE)
endif ()

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

string(TOUPPER "PLATFORM_${CMAKE_SYSTEM_NAME}" SYS_NAME)
add_compile_definitions(${SYS_NAME})


add_subdirectory(dependencies)

add_subdirectory(Engine)

add_subdirectory(Editor)

add_subdirectory(Runtime)



