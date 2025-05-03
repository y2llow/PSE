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
    sf::View view;

public:
    Game(sf::RenderWindow* window);

    void update();

    static string correctImage(char c);

    void loadMap(const string& filename);
};


#endif //GAME_H
