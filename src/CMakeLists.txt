project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES 
    bitboard.hpp
    defs.hpp
    move.hpp
    movegenerator.hpp
    patterns.hpp
    position.cpp
    utils.hpp
)

set(SOURCE_FILES
    bitboard.cpp
    move.cpp
    movegenerator.cpp
    patterns.hpp
    position.cpp
)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)