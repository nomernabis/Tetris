//
// Created by pavel on 18.12.17.
//

#include <iostream>
#include "../Headers/Shape.h"


void Shape::init() {
    x = 0;
    y = 0;
    rotation = 0;
    if(currentShape == -1){
        currentShape = std::rand() % 7;
        nextShape = std::rand() % 7;
    } else {
        currentShape = nextShape;
        nextShape = std::rand() % 7;
    }

    if(!tryLocate()){
        if(interface != nullptr){
            interface->message();
        }
    }
}

bool Shape::tryLocate() {
    int x = 0, y = 0;

    for(int i=y; i < y + 4; ++i){
        for(int j=x; j < x + 4; ++j){
            if(shapes[currentShape][0][i - x][j - x] == 1 && *map[i][j] == 2){
                return false;
            }
        }
    }

    return true;
}
Shape::Shape(int(&map)[HEIGHT][WIDTH]) {
    this->map = &map;
    std::srand(unsigned(std::time(0)));
    init();
}

void Shape::handleEvents(sf::Event event) {
    clear();
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
            moveLeft();
        }
        if (event.key.code == sf::Keyboard::Right) {
            moveRight();
        }
        if (event.key.code == sf::Keyboard::Up) {
            rotate();
        }
        if (event.key.code == sf::Keyboard::Down) {
            moveDown();
        }
    }
}


void Shape::update() {

    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsX(j) && isInBoundsY(i)) {
                if (shapes[currentShape][rotation][i - y][j - x] == 1) {
                    (*map)[i][j] = shapes[currentShape][rotation][i - y][j - x];
                }
            }
        }
    }
}

void Shape::clear() {
    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsX(j) && isInBoundsY(i) && (*map)[i][j] == 1) {
                (*map)[i][j] = 0;
            }
        }
    }
}

void Shape::moveDown() {
    if (!checkCollisionDown()) {
        ++y;
    } else {
        fixShape();
        init();
    }
}

void Shape::moveLeft() {
    if (!checkCollisionX(-1)) {
        --x;
    }
}

void Shape::moveRight() {
    if (!checkCollisionX(1)) {
        ++x;
    }
}

bool Shape::checkCollisionX(int direction) {
    int newX = x + direction;
    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsY(i) && isInBoundsX(j) && shapes[currentShape][rotation][i - y][j - x] == 1) {
                if (isInBoundsX(j + direction)) {
                    if ((*map)[i][j + direction] == 2) {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Shape::checkCollisionDown() {
    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsY(i) && isInBoundsX(j) && shapes[currentShape][rotation][i - y][j - x] == 1) {
                if (isInBoundsY(i + 1)) {
                    if ((*map)[i + 1][j] == 2) {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

void Shape::fixShape() {
    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsX(j) && isInBoundsY(i) && shapes[currentShape][rotation][i - y][j - x] == 1) {
                (*map)[i][j] = 2;
            }
        }
    }

}

bool Shape::checkCollisionRotation(int x, int y) {
    int nextRotation = (rotation + 1) % 4;

    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (isInBoundsY(i) && isInBoundsX(j)) {
                if (shapes[currentShape][nextRotation][i - y][j - x] == 1) {
                    if ((*map)[i][j] == 2) {
                        return true;
                    }
                }
            } else {
                if (shapes[currentShape][nextRotation][i - y][j - x] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Shape::rotate() {
    if (!checkCollisionRotation(x, y)) {
        rotation = (rotation + 1) % 4;
    } else {
        std::vector<Position> positions = getIntersections();

        int maxIntersectedX = -1;
        int minIntersectedX = WIDTH;
        int maxFreeX = -1;
        int minFreeX = WIDTH;

        int maxIntersectedY = -1;
        int minIntersectedY = HEIGHT;
        int maxFreeY = -1;
        int minFreeY = HEIGHT;


        for (int i = 0; i < positions.size(); ++i) {
            Position position = positions.at(i);

            if (position.isIntersected) {
                if (position.x > maxIntersectedX) {
                    maxIntersectedX = position.x;
                }
                if (position.x < minIntersectedX) {
                    maxIntersectedX = position.x;
                }
                if (position.y > maxIntersectedY) {
                    maxIntersectedY = position.y;
                }
                if (position.y < minIntersectedY) {
                    maxIntersectedY = position.y;
                }
            } else {
                if (position.x < minFreeX) {
                    minFreeX = position.x;
                }
                if (position.x > maxFreeX) {
                    maxFreeX = position.x;
                }
                if (position.y < minFreeY) {
                    minFreeY = position.y;
                }
                if (position.y > maxFreeY) {
                    maxFreeY = position.y;
                }
            }
        }

        if (maxIntersectedX < minFreeX) {
            int tempX = x;
            while (tempX < minFreeX) {
                ++tempX;
                //check intersections
                if (!checkCollisionRotation(tempX, y)) {
                    x = tempX;
                    rotation = (rotation + 1) % 4;
                    return;
                }
            }
        }

        //down intersection
        if (minIntersectedY > maxFreeY) {
            int tempY = y;
            while (tempY > maxFreeY - 3) {
                --tempY;
                if (!checkCollisionRotation(x, tempY)) {
                    y = tempY;
                    rotation = (rotation + 1) % 4;
                    return;
                }
            }
        }

        if (minIntersectedX > maxFreeX) {
            int tempX = x;
            while (tempX > maxFreeX - 3) {
                --tempX;
                //check intersections
                if (!checkCollisionRotation(tempX, y)) {
                    x = tempX;
                    rotation = (rotation + 1) % 4;
                    return;
                }
            }
        }
    }
}

std::vector<Position> Shape::getIntersections() {
    int nextRotation = (rotation + 1) % 4;
    std::vector<Position> positions;
    for (int i = y; i < y + 4; ++i) {
        for (int j = x; j < x + 4; ++j) {
            if (shapes[currentShape][nextRotation][i - y][j - x] == 1) {
                Position position;
                position.x = j;
                position.y = i;
                if (isInBoundsY(i) && isInBoundsX(j)) {
                    position.isIntersected = ((*map)[i][j] == 2);
                } else {
                    position.isIntersected = true;
                }
                positions.push_back(position);
            }
        }
    }
    return positions;
}

