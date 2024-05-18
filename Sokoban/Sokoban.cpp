// Copyright 2022 Nicholas Bartow
#include "HEADER/Sokoban.hpp"
#include <string>
#include <fstream>
#include <exception>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


Sokoban::~Sokoban() {
    clear();
    delete this->sprite_draw;
    this->sprite_draw = nullptr;
    // Delete automatically checks if ptr is null
    if (this->textures != nullptr) {
        for (int i = 0; i < 8; i++) {
            delete this->textures[i];
            this->textures[i] = nullptr;
        }
        delete[] this->textures;
        this->textures = nullptr;
    }
}

bool Sokoban::isWon() const {
    if (total_boxes < targets.size())
        return boxes_on_targets == total_boxes;
    // Second case
    return boxes_on_targets == targets.size();
}

char Sokoban::check_pos(int x, int y,
                        const player_directions& direction) const {
    if (direction == down) {
        if (y == size_y -1)
            return 'x';
        ++y;
    } else if (direction == up) {
        if (y == 0)
            return 'x';
        --y;
    } else if (direction == left) {
        if (x == 0)
            return 'x';
        --x;
    } else if (direction == right) {
        if (x == size_x-1)
            return 'x';
        ++x;
    }

    if (ptr[y][x] == '#')
        return 'x';
    else if (ptr[y][x] == 'O')
        return 'b';
    else
        return 'g';
    return 'x';
}

void Sokoban::movePlayer(const player_directions& direction) {
    char c = check_pos(player_x, player_y, direction);
    int new_player_y = player_y, new_player_x = player_x;
    if (direction == down)
        ++new_player_y;
    else if (direction == up)
        --new_player_y;
    else if (direction == left)
        --new_player_x;
    else if (direction == right)
        ++new_player_x;

    if (c == 'x') {
        return;
    } else if (c == 'g') {
        if (std::find(targets.begin(), targets.end(),
            sf::Vector2i(player_x, player_y)) != targets.end())
            ptr[player_y][player_x] = '*';
        else
            ptr[player_y][player_x] = '.';
        history.emplace(moves(sf::Vector2i(player_x, player_y),
                        sf::Vector2i(new_player_x, new_player_y), false));
        player_y = new_player_y;
        player_x = new_player_x;
        ptr[player_y][player_x] = '@';
    } else if (c == 'b') {
        if (check_pos(new_player_x, new_player_y, direction) == 'g') {
            int new_box_x = new_player_x, new_box_y = new_player_y;
            if (direction == down)
                ++new_box_y;
            else if (direction == up)
                --new_box_y;
            else if (direction == left)
                --new_box_x;
            else if (direction == right)
                ++new_box_x;

            // Where box is going
            if (std::find(targets.begin(), targets.end(),
                sf::Vector2i(new_box_x, new_box_y)) != targets.end())
                ++boxes_on_targets;
            // Where player is
            // // implying the player pushed the box off the target,
            // // being right next to the target
            if (std::find(targets.begin(), targets.end(),
                sf::Vector2i(new_player_x, new_player_y)) != targets.end())
                --boxes_on_targets;
            ptr[new_box_y][new_box_x] = 'O';
            ptr[new_player_y][new_player_x] = '@';
            if (std::find(targets.begin(), targets.end(),
                sf::Vector2i(player_x, player_y)) != targets.end())
                ptr[player_y][player_x] = '*';
            else
                ptr[player_y][player_x] = '.';
            history.emplace(moves(sf::Vector2i(player_x, player_y),
                    sf::Vector2i(new_player_x, new_player_y), true));
            player_y = new_player_y;
            player_x = new_player_x;
        }
    }
    player_direction = direction;
    // std::cout << history.top().player_posit_old.x << ", "
        // << history.top().player_posit_old.y << std::endl;
    // std::cout << history.top().player_posit_new.x << ", "
    //  << history.top().player_posit_new.y << std::endl;
    // std::cout << history.top().box_posit << std::endl;
}

void Sokoban::loadSprite() {
    if (this->textures == nullptr) {
        this->sprite_draw = new sf::Sprite;
        this->textures = new sf::Texture*[8];
        for (int i = 0; i < 8; i++) {
            this->textures[i] = new sf::Texture;
        }
        this->textures[floor]->loadFromFile("ground_01.png");  // .
        this->textures[wall]->loadFromFile("block_06.png");  // #
        this->textures[crate]->loadFromFile("crate_03.png");  // O
        this->textures[target_spot]->loadFromFile("ground_04.png");  // *
        this->textures[player_w]->loadFromFile("player_08.png");  //@
        this->textures[player_a]->loadFromFile("player_17.png");
        this->textures[player_s]->loadFromFile("player_05.png");
        this->textures[player_d]->loadFromFile("player_20.png");
    }
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (this->getSizeX() == 0)
        return;
    sprite_draw->setTextureRect({ 0, 0, 64, 64 });
    sprite_draw->setTexture(*this->textures[wall]);
    for (int i = 0; i < this->getSizeY(); i++) {
        for (int j = 0; j < this->getSizeX(); j++) {
            if (this->ptr[i][j] == '.') {
                sprite_draw->setTexture(*this->textures[floor]);
            } else if (this->ptr[i][j] == '#') {
                sprite_draw->setTexture(*this->textures[floor]);
                sprite_draw->setPosition((j * 64), (i * 64));
                target.draw(*sprite_draw);
                sprite_draw->setTexture(*this->textures[wall]);
            } else if (this->ptr[i][j] == 'O') {
                if (std::find(targets.begin(), targets.end(),
                    sf::Vector2i(j, i)) != targets.end())
                    sprite_draw->setTexture(*this->textures[target_spot]);
                else
                    sprite_draw->setTexture(*this->textures[floor]);
                sprite_draw->setPosition((j * 64), (i * 64));
                target.draw(*sprite_draw);
                sprite_draw->setTexture(*this->textures[crate]);
            } else if (this->ptr[i][j] == '*') {
                sprite_draw->setTexture(*this->textures[target_spot]);
            } else if (this->ptr[i][j] == '@') {
                if (std::find(targets.begin(), targets.end(),
                    sf::Vector2i(player_x, player_y)) != targets.end())
                    sprite_draw->setTexture(*this->textures[target_spot]);
                else
                    sprite_draw->setTexture(*this->textures[floor]);
                sprite_draw->setPosition((j * 64), (i * 64));
                target.draw(*sprite_draw);
                if (player_direction == down)
                    sprite_draw->setTexture(*this->textures[player_s]);
                else if (player_direction == up)
                    sprite_draw->setTexture(*this->textures[player_w]);
                else if (player_direction == left)
                    sprite_draw->setTexture(*this->textures[player_d]);
                else if (player_direction == right)
                    sprite_draw->setTexture(*this->textures[player_a]);
            } else {
                throw std::invalid_argument("Character not recognized");
            }
            sprite_draw->setPosition((j * 64), (i * 64));
            target.draw(*sprite_draw);
        }
    }
}

void operator>>(std::istream &in, Sokoban &obj) {
    obj.clear();
    std::string input;
    char holder;

    // Input check first
    in >> input;
    obj.size_x = std::stoul(input);
    if (obj.size_x < 3 || obj.size_x > 15)
        throw std::invalid_argument(
            "Map Size X must be between 3 and 15");
    in >> input;
    obj.size_y = std::stoul(input);
    if (obj.size_y < 3 || obj.size_y > 15)
        throw std::invalid_argument(
            "Map Size Y must be between 3 and 15");

    // creating the board
    obj.ptr = new char* [obj.size_y];
    for (int i = 0; i < obj.size_y; i++) {
        obj.ptr[i] = new char[obj.size_x];
    }
    obj.loadSprite();
    // filling the board
    for (int i = 0; i < obj.size_x * obj.size_y; i++) {
        in >> holder;
        if (holder == ' ' || holder == '\n') {
            continue;
        } else if (holder == '*')  {
            obj.targets.push_back({i % obj.size_x, i / obj.size_x});
            // std::cout << "Target recorded: (" <<  i % obj.size_x
                // << ", " << i / obj.size_x << ") " << std::endl;
        } else if (holder == 'O') {
            ++obj.total_boxes;
        } else if (holder == '@') {
            obj.player_x = i % obj.size_x;
            obj.player_y = i / obj.size_x;
        } else if (holder != '.' && holder != '#') {
            throw std::invalid_argument(
                   "Gameboard characters do not match standard");
        }
        obj.ptr[i / obj.size_x][i % obj.size_x] = holder;
    }
}

// test ostream operator
std::ostream& operator<<(std::ostream& out, const Sokoban& obj) {
    if (obj.size_x == 0) {
        out << " Sokoban Object has no size" << std::endl;
        return out;
    }
    out << obj.size_x << ", " << obj.size_y << std::endl;
    out << obj.player_x << ", " << obj.player_y << std::endl;
    for (int i = 0; i < obj.size_y; i++) {
        for (int j = 0; j < obj.size_x; j++)
            out << obj.ptr[i][j];
        out << std::endl;
    }
    return out;
}

void Sokoban::clear() {
    if (this->ptr != nullptr) {
        for (int i = 0; i < this->size_y; i++) {
            delete[] this->ptr[i];
            this->ptr[i] = nullptr;
        }
        delete[] this->ptr;
        this->ptr = nullptr;
    }
    this->size_x = this->size_y = 0;
    player_x = player_y = 0;
    boxes_on_targets = total_boxes = 0;
    player_direction = down;
    targets.clear();
    while (!history.empty()) {
        history.pop();
    }
}

void Sokoban::undo() {
    if (history.empty()) {
        player_direction = down;
        return;
    } else {
        const moves& m = history.top();
        if (std::find(targets.begin(), targets.end(),
            sf::Vector2i(m.player_posit_new.x, m.player_posit_new.y))
                != targets.end())
            ptr[m.player_posit_new.y][m.player_posit_new.x] = '*';
        else
            ptr[m.player_posit_new.y][m.player_posit_new.x] = '.';
        ptr[m.player_posit_old.y][m.player_posit_old.x] = '@';
        player_x = m.player_posit_old.x;
        player_y = m.player_posit_old.y;

        if (m.player_posit_new.y == m.player_posit_old.y - 1)
            player_direction = down;
        else if (m.player_posit_new.y == m.player_posit_old.y + 1)
            player_direction = up;
        else if (m.player_posit_new.x == m.player_posit_old.x - 1)
            player_direction = right;
        else if (m.player_posit_new.x == m.player_posit_old.x + 1)
            player_direction = left;
    if (m.box_posit) {
        sf::Vector2i new_box_posit;
        new_box_posit = m.player_posit_new;
        if (player_direction == down)
            --new_box_posit.y;
        else if (player_direction == up)
            ++new_box_posit.y;
        else if (player_direction == left)
            ++new_box_posit.x;
        else if (player_direction == right)
            --new_box_posit.x;
        if (std::find(targets.begin(), targets.end(),
            sf::Vector2i(new_box_posit.x, new_box_posit.y))
                != targets.end()) {
            ptr[new_box_posit.y][new_box_posit.x] = '*';
            --boxes_on_targets;
        } else {
            ptr[new_box_posit.y][new_box_posit.x] =  '.';
        }
        if (std::find(targets.begin(), targets.end(),
            sf::Vector2i(m.player_posit_new.x, m.player_posit_new.y))
                != targets.end())
            ++boxes_on_targets;
        ptr[m.player_posit_new.y][m.player_posit_new.x] = 'O';
    }
        history.pop();
    }
}

/*
int main() {
    Sokoban cn;
    std::ifstream map("map1.txt");
    map >> cn;
    std::string windowTime;
    sf::Clock timer;
    sf::RenderWindow window(sf::VideoMode(cn.getSizeX() * 64,
                                cn.getSizeY() * 64), "Sokoban");
    sf::Event e;
    while (window.isOpen()) {
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        windowTime = std::to_string(static_cast<int>
                                      (timer.getElapsedTime().asSeconds()));
        windowTime += " Sokoban";
        window.setTitle(windowTime);
        window.clear();
        window.draw(cn);
        window.display();
    }
    return 0;
}  */
