//
// Created by pavel on 17.12.17.
//

#ifndef TETRIS_CONTROLBOX_H
#define TETRIS_CONTROLBOX_H

#include <SFML/Graphics.hpp>
#include "../Utils/Constants.h"
#include "Command.h"

class ControlBox: public sf::Drawable{
public:
    ControlBox(sf::RenderWindow& window, std::string title, sf::Font font);
    void setTitle(std::string title);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setFont(sf::Font font);
    void addCommand(Command command){commands.push_back(command);}
    void handleEvents(sf::Event event);
private:
    sf::RenderWindow* window;
    sf::RectangleShape rect;
    std::string m_title;
    sf::Font font;
    sf::Text text;
    std::vector<Command> commands;

    void drawCommands();

    int curIndex = 0;
};


#endif //TETRIS_CONTROLBOX_H
