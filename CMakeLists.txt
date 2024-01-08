cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 11)

add_subdirectory(lib/interop)

set(SIMPLE_SRCS
    interop.c
    simple.c
    simple.h)
if(WIN32)
    list(APPEND SIMPLE_SRCS
        windows/resource.h
        windows/resource.rc)
endif()

project(simple C)

add_library(simple SHARED ${SIMPLE_SRCS})
target_link_libraries(simple interoplib)
target_compile_definitions(simple PRIVATE $<$<BOOL:WIN32>:_CRT_SECURE_NO_WARNINGS>)
set_target_properties(simple PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(MSVC)
    target_link_options(simple PRIVATE
        "/DEF:\"${CMAKE_CURRENT_SOURCE_DIR}/windows/simple.def\"")
elseif(APPLE)
    set_target_properties(simple PROPERTIES
        XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/mac/simple.exp")
else()
    target_link_options(simple PRIVATE
        "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/linux/simple.map"
        "-Wl,--gc-sections")
endif()
