#include "uci.hpp"
#include "rays.hpp"
#include "patterns.hpp"
#include "engineinfo.hpp"
#include "moveparser.hpp"
#include "zobrist.hpp"

#include <sstream>
#include <format>
#include <cassert>

namespace UCI
{
    void Game::start()
    {
        Rays::init();
        Patterns::init();
        Zobrist::init();

        std::string command_line, command_name;
        while (std::getline(input_stream_, command_line))
        {
            std::istringstream command_stream(command_line);
            command_stream >> command_name;

            if (command_name == "uci")
                execute_uci_command_();
            if (command_name == "isready")
                execute_isready_command_();
            if (command_name == "ucinewgame")
                execute_ucinewgame_command_();
            if (command_name == "position")
                execute_position_command_(command_stream);
            if (command_name == "go")
                execute_go_command_(command_stream);
            if (command_name == "stop")
                execute_stop_command_(command_stream);
            if (command_name == "quit")
            {
                execute_quit_command_(command_stream);
                break;
            }
        }
    }

    void Game::execute_uci_command_()
    {
        output_stream_ << std::format("id name {}", EngineInfo::engine_id()) << std::endl;
        output_stream_ << std::format("id author {}", EngineInfo::AUTHOR) << std::endl;
        output_stream_ << "uciok" << std::endl;
    }

    void Game::execute_isready_command_()
    {
        output_stream_ << "readyok" << std::endl;
    }

    void Game::execute_ucinewgame_command_() {}

    void Game::execute_position_command_(std::istringstream &command_args)
    {
        std::string argument;

        while (command_args >> argument)
        {
            if (argument == "startpos")
            {
                position_ = Position{};
            }
            else if (argument == "fen")
            {
                position_.set_from_fen(command_args);
            }
            else if (argument == "moves")
            {
                while (command_args >> argument)
                {
                    CoordinateMoveParser move_parser{position_};
                    auto move = move_parser.parse_move(argument);
                    position_.make_move(move);
                }
            }
            else
            {
                throw std::runtime_error(std::format("Invalid argument in position command: {}", argument));
            }
        }
    }

    void Game::execute_go_command_(std::istringstream &command_args)
    {
        auto best_move = Search::search(position_);
        output_stream_ << "bestmove " << best_move.uci_notation() << std::endl;
    }

    void Game::execute_stop_command_(std::istringstream &command_args) {}

    void Game::execute_quit_command_(std::istringstream &command_args) {}
}