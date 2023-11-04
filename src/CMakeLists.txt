project(${CMAKE_PROJECT_NAME}_lib)

set(HEADER_FILES 
    bitboard.hpp
    defs.hpp
    eval.hpp
    move.hpp
    movegenerator.hpp
    moveorderer.hpp
    moveparser.hpp
    patterns.hpp
    perft.hpp
    position.cpp
    rays.hpp
    search.hpp
    uci.hpp
    utils.hpp
)

set(SOURCE_FILES
    bitboard.cpp
    eval.cpp
    move.cpp
    movegenerator.cpp
    moveorderer.cpp
    moveparser.cpp
    patterns.cpp
    perft.cpp
    position.cpp
    rays.cpp
    search.cpp
    uci.cpp
)

add_library(${CMAKE_PROJECT_NAME}_lib STATIC
    ${SOURCE_FILES}
    ${HEADER_FILES}
)