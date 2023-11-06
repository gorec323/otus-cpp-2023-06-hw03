cmake_minimum_required(VERSION 3.6)

project(my_containers VERSION 0.1.0 LANGUAGES CXX)

if (BUILD_TESTING)
    add_subdirectory(test)
endif()

add_library(${PROJECT_NAME} INTERFACE)

# cmake_policy(SET CMP0076 NEW)

target_sources(${PROJECT_NAME}
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/my_vector.h
)

target_include_directories(${PROJECT_NAME}
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
