#include <iostream>
#include <vector>

int iShape[4][4][4] = {
        {
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0},
                {0, 0, 0, 0}
        },
        {
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0},
                {0, 0, 1, 0}
        },
        {
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 1, 1},
                {0, 0, 0, 0}
        },
        {
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0},
                {0, 1, 0, 0}
        }
};

int map[10][10] = {};

struct Position{
    int x;
    int y;
    bool isIntersected;
};
class Shape {
private:
    int rotation = 0;
    int x, y;
    int(*shape)[4][4][4];

    bool checkCollisionX(int direction) {
        int newX = x + direction;
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (isInBounds(i) && isInBounds(j) && (*shape)[rotation][i - y][j - x] == 1) {
                    if (isInBounds(j + direction)) {
                        if (map[i][j + direction] == 2) {
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

    bool checkCollisionDown() {
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (isInBounds(i) && (*shape)[rotation][i - y][j - x] == 1) {
                    if (isInBounds(i + 1)) {
                        if (map[i + 1][j] == 2) {
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

    void fixShape() {
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (isInBounds(i) && isInBounds(j) && map[i][j] == 1) {
                    map[i][j] = 2;
                }
            }
        }
    }

    bool isInBounds(int x) {
        return x >= 0 && x < 10;
    }

    bool checkCollisionRotation(int x, int y) {
        int nextRotation = (rotation + 1) % 4;

        for(int i=y;  i < y + 4; ++i){
            for(int j=x; j < x + 4; ++j){
                if(isInBounds(i) && isInBounds(j)){
                    if((*shape)[nextRotation][i - y][j - x] == 1){
                        if(map[i][j] == 2){
                            return true;
                        }
                    }
                } else {
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<Position> getIntersections(){
        int nextRotation = (rotation + 1) % 4;
        std::vector<Position> positions;
        for(int i = y; i < y + 4; ++i){
            for(int j = x; j < x + 4; ++j){
                if((*shape)[nextRotation][i - y][j - x] == 1){
                    Position position;
                    position.x = j;
                    position.y = i;
                    if(isInBounds(i) && isInBounds(j)){
                        position.isIntersected = (map[i][j] == 2);
                    } else {
                        position.isIntersected = true;
                    }
                    positions.push_back(position);
                }
            }
        }
        return positions;
    }

public:
    Shape() {
        x = 0;
        y = 0;
        shape = &iShape;
    }

    void moveDown() {
        if (!checkCollisionDown()) {
            ++y;
        } else {
            draw();
            fixShape();
            x = 0;
            y = 0;
        }
    }

    void moveLeft() {
        if (!checkCollisionX(-1)) {
            --x;
        }
    }

    void moveRight() {
        if (!checkCollisionX(1)) {
            ++x;
        }
    }

    void draw() {
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if ((*shape)[rotation][i - y][j - x] == 1) {
                    map[i][j] = (*shape)[rotation][i - y][j - x];
                }
            }
        }
    }

    void clear() {
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (map[i][j] == 1) {
                    map[i][j] = 0;
                }
            }
        }
    }

    void rotate() {
        if (!checkCollisionRotation(x, y)) {
            rotation = (rotation + 1) % 4;
        } else {
            std::vector<Position> positions = getIntersections();

            int maxIntersectedX = -1;
            int minIntersectedX = 10;
            int maxFreeX = -1;
            int minFreeX = 10;

            int maxIntersectedY = -1;
            int minIntersectedY = 10;
            int maxFreeY = -1;
            int minFreeY = 10;


            for(int i=0; i < positions.size(); ++i){
                Position position = positions.at(i);

                std::cout << "x y " << position.x << " " << position.y
                          << " " << position.isIntersected << std::endl;
                if(position.isIntersected){
                    if(position.x > maxIntersectedX){
                        maxIntersectedX = position.x;
                    }
                    if(position.x < minIntersectedX){
                        maxIntersectedX = position.x;
                    }
                    if(position.y > maxIntersectedY){
                        maxIntersectedY = position.y;
                    }
                    if(position.y < minIntersectedY){
                        maxIntersectedY = position.y;
                    }
                } else {
                    if(position.x < minFreeX){
                        minFreeX = position.x;
                    }
                    if(position.x > maxFreeX){
                        maxFreeX = position.x;
                    }
                    if(position.y < minFreeY){
                        minFreeY = position.y;
                    }
                    if(position.y > maxFreeY){
                        maxFreeY = position.y;
                    }
                }
            }

            if(maxIntersectedX < minFreeX){
                int tempX = x;
                while (tempX < minFreeX){
                    ++tempX;
                    //check intersections
                    if(!checkCollisionRotation(tempX, y)){
                        x = tempX;
                        rotation = (rotation + 1) % 4;
                        return;
                    }
                }
            }

            //down intersection
            if(minIntersectedY > maxFreeY){
                int tempY = y;
                while (tempY > maxFreeY - 3){
                    --tempY;
                    if(!checkCollisionRotation(x, tempY)){
                        y = tempY;
                        rotation = (rotation + 1) % 4;
                        return;
                    }
                }
            }

            if(minIntersectedX > maxFreeX){
                int tempX = x;
                while (tempX > maxFreeX - 3){
                    --tempX;
                    //check intersections
                    if(!checkCollisionRotation(tempX, y)){
                        x = tempX;
                        rotation = (rotation + 1) % 4;
                        return;
                    }
                }
            }
        }
    }
};

void drawMap() {
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << map[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    const int moveLeft = 1;
    const int moveDown = 2;
    const int moveRight = 3;
    const int rotate = 4;

    Shape shape;
    int command = 0;

    while (std::cin >> command) {
        shape.clear();
        switch (command) {
            case moveLeft:
                shape.moveLeft();
                break;
            case moveDown:
                shape.moveDown();
                break;
            case moveRight:
                shape.moveRight();
                break;
            case rotate:
                shape.rotate();
                break;
            default:
                break;
        }
        shape.draw();
        drawMap();
    }
    return 0;
}