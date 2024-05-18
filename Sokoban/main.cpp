// Copyright 2022 Nicholas Bartow
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "HEADER/Sokoban.hpp"

int main(int argc, char**argv) {
    Sokoban cn;
    // std::string fileName;
    // std::cin >> fileName;
    std::ifstream map(argv[1]);
    map >> cn;
    std::string windowTime;
    sf::Clock timer;
    sf::RenderWindow window(sf::VideoMode(cn.getSizeX() * 64,
                                cn.getSizeY() * 64), "Sokoban");
    sf::Event e;
    while (window.isOpen()) {
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            } else if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::R) {
                    map.seekg(0);
                    map >> cn;
                    timer.restart();
                } else if (e.key.code == sf::Keyboard::Z) {
                        cn.undo();
                    }
                if (!cn.isWon()) {
                    if (e.key.code == sf::Keyboard::S)
                        cn.movePlayer(cn.down);
                    else if (e.key.code == sf::Keyboard::W)
                        cn.movePlayer(cn.up);
                    else if (e.key.code == sf::Keyboard::A)
                        cn.movePlayer(cn.left);
                    else if (e.key.code == sf::Keyboard::D)
                        cn.movePlayer(cn.right);
                }
            }
        }
        if (!cn.isWon()) {
            windowTime = std::to_string(static_cast<int>
                                      (timer.getElapsedTime().asSeconds()));
            windowTime += " Sokoban";
            window.setTitle(windowTime);
        } else {
            windowTime = " WIN ";
            window.setTitle(windowTime);
        }
        window.clear();
        window.draw(cn);
        window.display();
    }
    return 0;
}
