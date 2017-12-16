#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "shapes.h"

const int WIDTH = 10;
const int HEIGHT = 20;
const int RECT_SIZE = 30;


int map[WIDTH][HEIGHT] = {};

void clearRow(int row) {
    for (int j = 0; j < WIDTH; ++j) {
        map[row][j] = 0;
    }
}

void move(int from, int to) {
    for (int i = 0; i < WIDTH; ++i) {
        map[to][i] = map[from][i];
    }
    clearRow(from);
}

void moveRows(int to) {
    for (int i = to - 1; i >= 0; --i) {
        move(i, i + 1);
    }
    clearRow(0);
}

void clearFullRows() {
    for (int i = 0; i < HEIGHT; ++i) {
        bool isFull = true;
        for (int j = 0; j < WIDTH; ++j) {
            if (map[i][j] == 0) {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            clearRow(i);
            moveRows(i);
        }
    }
}

void drawPauseBox(sf::RenderWindow& window){
    const int BOX_WIDTH = WIDTH * RECT_SIZE - 50;
    const int BOX_HEIGHT = HEIGHT * RECT_SIZE - 200;

    int x = (WIDTH * RECT_SIZE - BOX_WIDTH) / 2;
    int y = (HEIGHT * RECT_SIZE - BOX_HEIGHT) / 2;

    sf::Font arialFont;
    if (!arialFont.loadFromFile("arial.ttf")) {
        //print some error
    }

    sf::RectangleShape pauseRect;
    pauseRect.setPosition({x, y});
    pauseRect.setSize({BOX_WIDTH, BOX_HEIGHT});
    pauseRect.setFillColor(sf::Color::Black);
    pauseRect.setOutlineColor(sf::Color::White);

    sf::Text titleText;
    titleText.setFont(arialFont);
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("PAUSE");
    titleText.setPosition({x + BOX_WIDTH / 2 - titleText.getLocalBounds().width / 2,
                           y + 24});

    window.draw(pauseRect);

    window.draw(titleText);

    titleText.setString("RESUME");
    titleText.setCharacterSize(20);

    titleText.setPosition({x + BOX_WIDTH / 2 - titleText.getLocalBounds().width / 2,
                           y + 150});
    window.draw(titleText);


    titleText.setString("REPLAY");
    titleText.setCharacterSize(20);

    titleText.setPosition({x + BOX_WIDTH / 2 - titleText.getLocalBounds().width / 2,
                           y + 200});

    window.draw(titleText);

    titleText.setString("EXIT");
    titleText.setCharacterSize(20);

    titleText.setPosition({x + BOX_WIDTH / 2 - titleText.getLocalBounds().width / 2,
                           y + 250});

    window.draw(titleText);

}

struct Position {
    int x;
    int y;
    bool isIntersected;
};

class Shape {
private:
    int rotation = 0;
    int x, y;
    int(*shape)[4][4][4];
    sf::RenderWindow* window;
    void setShape() {
        int rand = std::rand();
        std::cout << "random " << rand << std::endl;

        if (nextShape != -1) {
            shape = &shapes[nextShape];
        } else {
            shape = &shapes[rand % 7];
        }
        nextShape = rand % 7;

    }

    bool checkCollisionX(int direction) {
        int newX = x + direction;
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (isInBoundsY(i) && isInBoundsX(j) && (*shape)[rotation][i - y][j - x] == 1) {
                    if (isInBoundsX(j + direction)) {
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
                if (isInBoundsY(i) && (*shape)[rotation][i - y][j - x] == 1) {
                    if (isInBoundsY(i + 1)) {
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
                if (isInBoundsY(i) && isInBoundsX(j) && map[i][j] == 1) {
                    map[i][j] = 2;
                }
            }
        }
    }

    bool isInBoundsX(int x) {
        return x >= 0 && x < WIDTH;
    }

    bool isInBoundsY(int y) {
        return y >= 0 && y < HEIGHT;
    }

    bool checkCollisionRotation(int x, int y) {
        int nextRotation = (rotation + 1) % 4;

        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if (isInBoundsY(i) && isInBoundsX(j)) {
                    if ((*shape)[nextRotation][i - y][j - x] == 1) {
                        if (map[i][j] == 2) {
                            return true;
                        }
                    }
                } else {
                    if ((*shape)[nextRotation][i - y][j - x] == 1) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    std::vector<Position> getIntersections() {
        int nextRotation = (rotation + 1) % 4;
        std::vector<Position> positions;
        for (int i = y; i < y + 4; ++i) {
            for (int j = x; j < x + 4; ++j) {
                if ((*shape)[nextRotation][i - y][j - x] == 1) {
                    Position position;
                    position.x = j;
                    position.y = i;
                    if (isInBoundsY(i) && isInBoundsX(j)) {
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
    int nextShape = -1;


    Shape(sf::RenderWindow& window) {
        x = 0;
        y = 0;
        setShape();
        this->window = &window;
    }

    void drawMap(){
        sf::RectangleShape rectangleShape;
        rectangleShape.setFillColor(sf::Color::Black);
        rectangleShape.setSize({RECT_SIZE, RECT_SIZE});
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if (map[i][j] != 0) {
                    rectangleShape.setPosition(j * RECT_SIZE, i * RECT_SIZE);
                    window->draw(rectangleShape);
                }
            }
        }


        int(*nextShape)[4][4][4] = &shapes[this->nextShape];


        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if ((*nextShape)[0][i][j] == 1) {
                    rectangleShape.setPosition({window->getSize().x - 150 + RECT_SIZE * j,
                                                50 + RECT_SIZE * i});
                    window->draw(rectangleShape);
                }
            }
        }
    }
    void moveDown() {
        if (!checkCollisionDown()) {
            ++y;
        } else {
            draw();
            fixShape();
            setShape();
            clearFullRows();
            x = 0;
            y = 0;
            rotation = 0;
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
            int minIntersectedX = WIDTH;
            int maxFreeX = -1;
            int minFreeX = WIDTH;

            int maxIntersectedY = -1;
            int minIntersectedY = HEIGHT;
            int maxFreeY = -1;
            int minFreeY = HEIGHT;


            for (int i = 0; i < positions.size(); ++i) {
                Position position = positions.at(i);

                std::cout << "x y " << position.x << " " << position.y
                          << " " << position.isIntersected << std::endl;
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
};

void drawGraphics() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * RECT_SIZE + 150, HEIGHT * RECT_SIZE),
                            "SFML works!");
    Shape shape(window);

    sf::Clock clock;
    int elapsedTime = 0;
    int update_time = 1000;

    sf::RectangleShape border;
    border.setSize({1, HEIGHT * RECT_SIZE});
    border.setPosition(window.getSize().x - 150, 0);
    border.setFillColor(sf::Color::Black);


    bool isOnPause = false;


    sf::Font arialFont;
    if (!arialFont.loadFromFile("arial.ttf")) {
       //print some error
    }

    sf::Text text;
    text.setFont(arialFont);
    text.setString("Next");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setPosition({window.getSize().x - 75 - text.getLocalBounds().width / 2, 20});

    while (window.isOpen()) {

        elapsedTime = clock.getElapsedTime().asMilliseconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            shape.clear();
            if (event.type == sf::Event::Closed)
                window.close();


            if(event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Space) {
                    isOnPause = !isOnPause;
                    clock.restart();
                    window.clear(sf::Color::Green);
                    shape.drawMap();
                    drawPauseBox(window);
                    window.display();
                }
            }

            if(!isOnPause){
                switch (event.type) {
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Down) {
                            update_time = 100;
                        }
                        break;
                    case sf::Event::KeyReleased:
                        if (event.key.code == sf::Keyboard::Left) {
                            shape.moveLeft();
                        }
                        if (event.key.code == sf::Keyboard::Right) {
                            shape.moveRight();
                        }
                        if (event.key.code == sf::Keyboard::Up) {
                            shape.rotate();
                        }
                        if (event.key.code == sf::Keyboard::Down) {
                            update_time = 1000;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        if(!isOnPause){
            if (elapsedTime > update_time) {
                clock.restart();
                shape.clear();
                shape.moveDown();
            }
            shape.draw();
            window.clear(sf::Color::White);

            //draw map
            shape.drawMap();

            window.draw(border);
            window.draw(text);
            window.display();
        }
    }
}

int main() {
    std::srand(unsigned(std::time(0)));

    drawGraphics();
    return 0;
}