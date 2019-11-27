cmake_minimum_required(VERSION 2.8)

include_directories(.)
include_directories(interop)

add_subdirectory(interop)

set(SIMPLE_SRC
    simple.c
    simpleinterop.c)
set(SIMPLE_HEADERS
    simple.h)

if(WIN32)
    list(APPEND SIMPLE_SRC windows/resource.rc)
    list(APPEND SIMPLE_HEADERS windows/resource.h)
endif()

if(WIN32)
    set(CMAKE_C_FLAGS_DEBUG "/MTd")
    set(CMAKE_C_FLAGS_RELEASE "/MT")
    set(CMAKE_C_FLAGS_MINSIZEREL "/MT")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/MT")

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

add_library(${PROJECT_NAME} SHARED ${SIMPLE_SRC} ${SIMPLE_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} interoplib)
