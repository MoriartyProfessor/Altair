#ifndef ALTAIR_MOVEPARSE_HPP
#define ALTAIR_MOVEPARSE_HPP

#include <string>
#include <vector>

#include "move.hpp"
#include "position.hpp"

class MoveParser
{
    public:
    virtual Move parse_move(const std::string& descriptor) = 0;
    virtual std::vector<Move> parse_sequence(const std::string& descriptor) = 0;
};

class CoordinateMoveParser : public MoveParser
{
    public:

    explicit CoordinateMoveParser(const Position& position);
    
    void set_position(const Position& position);

    virtual Move parse_move(const std::string& descriptor);

    virtual std::vector<Move> parse_sequence(const std::string& descriptor) override;

    protected:
    Position position_;
};

#endif // ALTAIR_MOVEPARSE_HPP