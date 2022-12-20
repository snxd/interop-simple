cmake_minimum_required(VERSION 3.15)

add_subdirectory(lib/interop)

set(SIMPLE_SRCS
    simple.h
    simple.c
    simpleinterop.c)
if(WIN32)
    list(APPEND SIMPLE_SRCS
        windows/resource.h
        windows/resource.rc)
endif()

if(WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_MODULE_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/windows/simple.def\"")
elseif(APPLE)
    set(CMAKE_XCODE_ATTRIBUTE_EXPORTED_SYMBOLS_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/mac/simple.def")
elseif(UNIX)
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/linux/simple.def")
    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
endif()

project(simple C)

add_library(${PROJECT_NAME} SHARED ${SIMPLE_SRCS})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} interoplib)
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<BOOL:WIN32>:_CRT_SECURE_NO_WARNINGS>)
