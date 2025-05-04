//
// Created by Abdellah on 5/2/2025.
//

#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include "../src/elementen/Voertuig.h"



class Game
{
    sf::RenderWindow* window;
    bool paused = false;

public:
    Game(sf::RenderWindow* window);

    void update();

    static string correctImage(char c);

    void loadMap(const string& filename);

    void setPaused(bool paused);
    bool getPaused() const;
};


#endif //GAME_H
