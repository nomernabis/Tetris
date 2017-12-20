//
// Created by pavel on 18.12.17.
//

#include <iostream>
#include "InfoScreen.h"

void InfoScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::RectangleShape border;
    border.setPosition(WIDTH * RECT_SIZE, 0);
    border.setSize({1, HEIGHT * RECT_SIZE});
    border.setFillColor(sf::Color::Black);

    target.draw(border, states);

    sf::Text text;
    text.setCharacterSize(20);
    text.setString("Next");
    text.setFillColor(sf::Color::Black);
    text.setFont(font);
    text.setPosition(WIDTH * RECT_SIZE + INFO_WIDTH / 2 - text.getLocalBounds().width/2, 20);

    target.draw(text, states);

    float shapeY = text.getPosition().y + text.getLocalBounds().height + 50;

    sf::RectangleShape rect;
    rect.setSize({RECT_SIZE, RECT_SIZE});
    rect.setFillColor(sf::Color::Black);

    int cntWidth = 0;
    int maxWidth = -1;
    int minX = 5;
    for(int i=0; i < 4; ++i){
        cntWidth = 0;
        for(int j = 0; j < 4; ++j){
            if(shapes[nextShapeId][0][i][j] == 1){
                ++cntWidth;
                if(j < minX){
                    minX = j;
                }
            }
        }
        if(cntWidth > maxWidth){
            maxWidth = cntWidth;
        }
    }

    int offsetX = (INFO_WIDTH - maxWidth * RECT_SIZE)/2 - minX * RECT_SIZE;

    for(int i=0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            if(shapes[nextShapeId][0][i][j] == 1){
                rect.setPosition({offsetX + j*RECT_SIZE + WIDTH * RECT_SIZE, shapeY + i * RECT_SIZE});
                target.draw(rect, states);
            }
        }
    }

    float scoresY = shapeY + RECT_SIZE * 4 + 50;

    text.setString("Lines");
    text.setPosition(WIDTH * RECT_SIZE + INFO_WIDTH / 2 - text.getLocalBounds().width/2, scoresY);
    target.draw(text, states);

    float linesY = scoresY + text.getLocalBounds().height + 20;

    text.setString(std::to_string(lines));
    text.setPosition(WIDTH * RECT_SIZE + INFO_WIDTH / 2 - text.getLocalBounds().width/2, linesY);
    target.draw(text, states);
}

InfoScreen::InfoScreen(int width, int height) {
    this->width = width;
    this->height = height;
}
