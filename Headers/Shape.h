//
// Created by pavel on 18.12.17.
//

#ifndef TETRIS_SHAPE_H
#define TETRIS_SHAPE_H

#include "../Utils/Shapes.h"
#include "../Utils/Constants.h"
#include "MessageInterface.h"
#include <SFML/Graphics.hpp>
#include <ctime>

struct Position {
    int x;
    int y;
    bool isIntersected;
};
class Shape {
private:
    int x,y;
    int (*map)[HEIGHT][WIDTH];
    int currentShape = -1, nextShape;
    int rotation;

    sf::Clock clock;
    float elapsed = 0;


    void moveLeft();
    void moveRight();
    bool isInBoundsX(int x) { return x >= 0 && x < WIDTH; }
    bool isInBoundsY(int y) { return y >= 0 && y < HEIGHT; }
    bool checkCollisionX(int direction);
    bool checkCollisionDown();
    void fixShape();
    void rotate();
    bool checkCollisionRotation(int x, int y);
    std::vector<Position> getIntersections();
    bool tryLocate();
public:
    MessageInterface* interface;
    void clear();
    void init();
    void moveDown();
    void update();
    explicit Shape(int(&map)[HEIGHT][WIDTH]);
    void handleEvents(sf::Event event);
    int getCurrentShape(){ return currentShape;}
    int getNextShape(){ return nextShape;}
    void setCurrentShape(int curShape){
        currentShape = curShape;
    }
    void setNextShape(int nextShape){
        this->nextShape = nextShape;
    }
};


#endif //TETRIS_SHAPE_H
