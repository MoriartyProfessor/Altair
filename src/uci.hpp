#ifndef GWAIHIR_UCI_HPP
#define GWAIHIR_UCI_HPP

#include "search.hpp"

#include <iostream>

namespace UCI
{
    class Game
    {
    public:
        Game() : output_stream_{std::cout}, input_stream_{std::cin}
        {
        }
        void start();

    private:
        void execute_uci_command_();
        void execute_isready_command_();
        void execute_ucinewgame_command_();
        void execute_position_command_(std::istringstream &command);
        void execute_go_command_(std::istringstream &command);
        void execute_stop_command_(std::istringstream &command);
        void execute_quit_command_(std::istringstream &command);

    private:
        std::ostream &output_stream_;
        std::istream &input_stream_;
        Position position_;
    };
}

#endif // GWAIHIR_UCI_HPP