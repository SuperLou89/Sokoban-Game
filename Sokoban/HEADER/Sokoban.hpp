// Copyright 2022 Nicholas Bartow
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>

class Sokoban : public sf::Drawable {
 public:
    // Constructor for game board
    Sokoban() = default;
    ~Sokoban();
    enum player_directions {
        up,
        down,
        left,
        right
    };

    // Load correct sprite
    void loadSprite();
    void movePlayer(const player_directions&);
    void clear();
    const int& getSizeX() const { return size_x; }
    const int& getSizeY() const { return size_y; }
    bool isWon() const;
    void undo();
    friend void operator>>(std::istream &in, Sokoban &obj);
    friend std::ostream& operator<<(std::ostream& out, const Sokoban& obj);
    
 private:
    // Draw
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    char check_pos(int x, int y, const player_directions&) const;
    player_directions player_direction = down;
    int size_x, size_y, player_x, player_y;
    unsigned int boxes_on_targets, total_boxes;
    char **ptr = nullptr;
    sf::Sprite *sprite_draw = nullptr;
    sf::Texture** textures = nullptr;
    enum blocks {
        floor,
        wall,
        crate,
        target_spot,
        player_w,
        player_a,
        player_s,
        player_d
    };
    std::vector <sf::Vector2i> targets;
    struct moves {
        moves(const sf::Vector2i &old, const sf::Vector2i &newest, const bool &bp) : 
            player_posit_old(old),
            player_posit_new(newest),
            box_posit(bp)
            {}
        // Player location and previous location
        sf::Vector2i player_posit_old;
        sf::Vector2i player_posit_new;
        // track boxes
        bool box_posit;

    };
    std::stack <moves> history;
};


