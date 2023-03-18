include(GoogleTest)
enable_testing()

set(SOURCE_FILES
    helpers.cpp  
    main.cpp
)

add_executable(${CMAKE_PROJECT_NAME}_test ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest)

gtest_discover_tests(${CMAKE_PROJECT_NAME}_test)