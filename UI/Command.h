//
// Created by pavel on 18.12.17.
//

#ifndef TETRIS_COMMAND_H
#define TETRIS_COMMAND_H

#include <string>
#include <functional>

struct Command {
public:
    std::string name;
    std::function<void()> action;
};


#endif //TETRIS_COMMAND_H
