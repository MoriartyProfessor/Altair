cmake_minimum_required(VERSION 3.12)
project(Altair)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost)

set(SOURCE_FILES main.cpp)
add_executable(${CMAKE_PROJECT_NAME}_executable ${SOURCE_FILES})

target_include_directories(${CMAKE_PROJECT_NAME}_executable PRIVATE ${Boost_INCLUDE_DIRS})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(libs/googletest)

target_link_libraries(${CMAKE_PROJECT_NAME}_executable ${CMAKE_PROJECT_NAME}_lib)