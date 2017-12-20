//
// Created by pavel on 21.12.17.
//

#ifndef TETRIS_MESSAGEINTERFACE_H
#define TETRIS_MESSAGEINTERFACE_H
class MessageInterface{
public:
    virtual ~MessageInterface(){};
    virtual void message() = 0;
};
#endif //TETRIS_MESSAGEINTERFACE_H
