//
// Created by pavel on 18.12.17.
//

#ifndef TETRIS_INFOSCREEN_H
#define TETRIS_INFOSCREEN_H


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include "../Utils/Constants.h"
#include "../Utils/Shapes.h"

class InfoScreen: public sf::Drawable{
private:
    int lines = 0;
    sf::RectangleShape rect;
    int width, height;
    sf::Font font;
    int nextShapeId = -1;
public:
    InfoScreen(int width, int height);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    int getWidth(){ return width;}
    int getHeight(){ return height;}
    void setLines(int lines){ this->lines = lines;}
    int getLines(){ return lines;}
    void setFont(sf::Font font) {this->font = font;}
    void setNextShapeId(int nextShape){
        nextShapeId = nextShape;
    }
};


#endif //TETRIS_INFOSCREEN_H
