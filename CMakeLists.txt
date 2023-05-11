cmake_minimum_required(VERSION 3.12)
project(Altair)

set(CMAKE_CXX_STANDARD 17)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(SOURCE_FILES main.cpp)
add_executable(${CMAKE_PROJECT_NAME}_executable ${SOURCE_FILES})

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(libs/googletest)

target_link_libraries(${CMAKE_PROJECT_NAME}_executable ${CMAKE_PROJECT_NAME}_lib)