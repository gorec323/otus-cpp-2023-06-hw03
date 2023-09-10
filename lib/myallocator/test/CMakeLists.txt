cmake_minimum_required(VERSION 3.0.0)

project(my_allocator_test LANGUAGES CXX)

include(CTest)
enable_testing()

find_package(GTest)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME}
PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${GTEST_BOTH_LIBRARIES}
    my_allocator
)
