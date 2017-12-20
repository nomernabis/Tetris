//
// Created by pavel on 17.12.17.
//

#include <iostream>
#include "ControlBox.h"

void ControlBox::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect, states);
    //drawTitle
    target.draw(text, states);
    //drawCommands

    float centerX = text.getPosition().x + text.getLocalBounds().width / 2;
    float centerY = rect.getPosition().y + rect.getSize().y / 2;


    float commandsHeight = 0;
    float maringTop = 50;

    sf::Text commandText;
    commandText.setCharacterSize(28);
    commandText.setFillColor(sf::Color::White);
    commandText.setFont(font);
    commandText.setString("AAAAAAA");

    commandsHeight = commandText.getLocalBounds().height * commands.size() + maringTop * (commands.size() - 1);

    sf::RectangleShape indicator;
    indicator.setSize({10, 10});
    indicator.setFillColor(sf::Color::White);


    float startY = centerY - commandsHeight / 2;
    //commandText.setPosition(centerX - commandText.getLocalBounds().width / 2, startY);

    int offsetY = 0;
    for(int i=0; i < commands.size(); ++i){
        commandText.setString(commands.at(i).name);
        commandText.setPosition(centerX - commandText.getLocalBounds().width / 2, startY + offsetY);
        offsetY += maringTop + commandText.getLocalBounds().height;
        target.draw(commandText, states);

        if(curIndex == i){
            indicator.setPosition({commandText.getPosition().x - 20,
                                   commandText.getLocalBounds().top
                                   + commandText.getPosition().y
                                   + commandText.getLocalBounds().height/2
                                   - indicator.getSize().y/2});
        }
    }

    target.draw(indicator, states);
}

ControlBox::ControlBox(sf::RenderWindow &window, std::string title, sf::Font font) {
    m_title = title;
    this->font = font;
    int paddingSides = 20;
    int paddingTopBottom = 50;
    this->window = &window;

    float x = paddingSides;
    float y = paddingTopBottom;

    rect.setPosition(x, y);
    rect.setSize({this->window->getSize().x - paddingSides * 2, this->window->getSize().y - paddingTopBottom * 2});
    rect.setFillColor(sf::Color::Red);


    text.setString(m_title);
    text.setCharacterSize(22);
    text.setFillColor(sf::Color::White);
    text.setFont(this->font);
    text.setPosition(x + (rect.getSize().x - text.getLocalBounds().width) / 2,
                     y + paddingTopBottom);

}

void ControlBox::setTitle(std::string title) {
    m_title = title;
}

void ControlBox::setFont(sf::Font font) {
    this->font = font;
}

void ControlBox::drawCommands() {

}

void ControlBox::handleEvents(sf::Event event) {
    if(event.type == sf::Event::KeyPressed){
        switch (event.key.code){
            case sf::Keyboard::Down:
                if(commands.size() != 0){
                    curIndex = (curIndex + 1) % commands.size();
                }
                break;
            case sf::Keyboard::Up:
                if(commands.size() != 0){
                    curIndex--;
                    if(curIndex < 0){
                        curIndex = commands.size() - 1;
                    }
                }
                break;
            case sf::Keyboard::Return:
                if(!commands.empty()){
                    commands.at(curIndex).action();
                }
                break;
            default:
                break;
        }
    }
}




