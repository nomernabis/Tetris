//
// Created by pavel on 17.12.17.
//

#include <iostream>
#include "../Headers/Game.h"

Game::Game(){

    mainWindow.create(sf::VideoMode(WIDTH * RECT_SIZE + INFO_WIDTH, HEIGHT * RECT_SIZE), "Tetris");
    mainWindow.setFramerateLimit(60);
    font.loadFromFile("arial.ttf");

    controlBox = new ControlBox(mainWindow, "Tetris", font);
    infoScreen = new InfoScreen(INFO_WIDTH, mainWindow.getSize().y);
    infoScreen->setFont(font);

    setCommands();

    shape = new Shape(map);
    shape->interface = this;
    infoScreen->setNextShapeId(shape->getNextShape());
}

void Game::run() {
    m_running = true;
    float elapsed = 0;
    while (mainWindow.isOpen()){
        if(m_running){
            elapsed += clock.getElapsedTime().asSeconds();
            clock.restart().asSeconds();
            if (elapsed > 1) {
                elapsed = 0;
                shape->clear();
                shape->moveDown();
            }
        }
        handleEvents();
        shape->update();
        infoScreen->setNextShapeId(shape->getNextShape());
        countScores();
        draw();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while(mainWindow.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            mainWindow.close();
        }

        if(event.type == sf::Event::KeyPressed){
            if(!m_loose){
                if(event.key.code == sf::Keyboard::Space){
                    m_running = !m_running;
                }
            }
        }
        if(!m_running){
            controlBox->handleEvents(event);
        } else {
            shape->handleEvents(event);
        }
    }
}

void Game::draw() {

    mainWindow.clear(sf::Color::White);

    //drawMap
    sf::RectangleShape rect;
    rect.setSize({RECT_SIZE, RECT_SIZE});
    rect.setFillColor(sf::Color::Black);

    for(int i=0; i < HEIGHT; ++i){
        for(int j=0; j < WIDTH; ++j){
            if(map[i][j] != 0){
                rect.setPosition(j * RECT_SIZE, i * RECT_SIZE);
                mainWindow.draw(rect);
            }
        }
    }


    mainWindow.draw(*infoScreen);
    if(!m_running){
        if(m_loose){
            controlBox->setTitle("You Loose!");
        }
        mainWindow.draw(*controlBox);
    }

    mainWindow.display();
}


void Game::clearRow(int row) {
    for (int j = 0; j < WIDTH; ++j) {
        map[row][j] = 0;
    }
}

void Game::move(int from, int to) {
    for (int i = 0; i < WIDTH; ++i) {
        map[to][i] = map[from][i];
    }
    clearRow(from);
}

void Game::moveRows(int to) {
    for (int i = to - 1; i >= 0; --i) {
        move(i, i + 1);
    }
    clearRow(0);
}

void Game::countScores(){
    for (int i = 0; i < HEIGHT; ++i) {
        bool isFull = true;
        for (int j = 0; j < WIDTH; ++j) {
            if (map[i][j] != 2) {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            infoScreen->setLines(infoScreen->getLines() + 1);
            shape->clear();
            clearRow(i);
            moveRows(i);
            shape->init();
        }
    }
}

void Game::setCommands() {
    Command startCommand;
    startCommand.name = "Start";
    startCommand.action = [this](){
        m_running = true;
    };

    Command exitCommand;
    exitCommand.name = "Exit";
    exitCommand.action = [this](){
        mainWindow.close();
    };
    controlBox->addCommand(startCommand);
    controlBox->addCommand(exitCommand);
}

void Game::message() {
    //MessageInterface::message();
    m_running = false;
    m_loose = true;
}
