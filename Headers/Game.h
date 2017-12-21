//
// Created by pavel on 17.12.17.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <SFML/Graphics.hpp>

#include "../Utils/Constants.h"
#include "../UI/ControlBox.h"
#include "../UI/InfoScreen.h"
#include "Shape.h"

class Game: public MessageInterface {
public:
    void run();
    Game();
private:
    void handleEvents();
    void draw();
    void setCommands();
    void clearRow(int row);
    void move(int from, int to);
    void moveRows(int to);
    void countScores();
    void message();
    void clearGame();
    int map[HEIGHT][WIDTH] = {};
    bool m_running = false;
    bool m_loose = false;
    sf::RenderWindow mainWindow;
    ControlBox* controlBox;
    InfoScreen* infoScreen;
    sf::Font font;
    Shape* shape;
    sf::Clock clock;
};


#endif //TETRIS_GAME_H
