cmake_minimum_required(VERSION 3.0.0)

project(my_allocator VERSION 0.1.0 LANGUAGES CXX)

if (BUILD_TESTING)
    add_subdirectory(test)
endif()

add_library(${PROJECT_NAME} INTERFACE
    include/my_poll_allocator.h
)

target_include_directories(${PROJECT_NAME}
INTERFACE
    include
)
