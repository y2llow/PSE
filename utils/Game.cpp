//
// Created by Abdellah on 5/2/2025.
//

#include "Game.h"

#include <fstream>
#include <iostream>


Game::Game(sf::RenderWindow* window)
{
    this->window = window;
}

string Game::correctImage(const char c)
{
    switch (c)
    {
    case '=':
        return "../resources/lane.png";

    case 'A':
        return "../resources/auto.png";

    case 'B':
        return "../resources/bus.png";

    case 'P': return "../resources/politiecombi.png";

    case 'I': return "../resources/brandweerwagen.png";

    case 'Z': return "../resources/ziekenwagen.png";

    default: return "../resources/player.png";
    }
}

void Game::loadMap(const string& filename)
{
    // Load a texture from an image file
    ifstream contents{filename};

    if (!contents.is_open())
    {
        // An error happened while trying to open the file!
        window->close();
        cout << "Unable to open file" << endl;
        return;
    }

    sf::Texture* baan_texture = new sf::Texture();
    if (!baan_texture->loadFromFile("../resources/lane.png"))
        cerr << "COULDN'T LOAD MAP FILE" << endl;

    sf::Texture* auto_texture= new sf::Texture();
    auto_texture->loadFromFile("../resources/auto.png");

    sf::Texture* bus_texture= new sf::Texture();
    bus_texture->loadFromFile("../resources/bus.png");

    sf::Texture* firetruck_texture= new sf::Texture();
    firetruck_texture->loadFromFile("../resources/firetruck.png");

    sf::Texture* ziekenwagen_texture= new sf::Texture();
    ziekenwagen_texture->loadFromFile("../resources/ambulance.png");

    sf::Texture* red_texture= new sf::Texture();
    red_texture->loadFromFile("../resources/red-light.png");

    sf::Texture* green_texture= new sf::Texture();
    green_texture->loadFromFile("../resources/green-light.png");

    sf::Texture* bushalte_texture= new sf::Texture();
    bushalte_texture->loadFromFile("../resources/bushalte.png");

    sf::Texture* police_texture= new sf::Texture();
    police_texture->loadFromFile("../resources/police.png");


    // The lines of the file resources/map.txt will be saved in this variable
    string line;

    vector<sf::RectangleShape> sprites;

    window->clear(sf::Color::Cyan);

    float sprite_size = 100.f;

    sf::View view;
    view.setSize(sf::VideoMode::getDesktopMode().width, 200);
    view.setCenter(90 * sprite_size, 50);
    window->setView(view); // Apply the view to the window

    while (getline(contents, line))
    {
        int first_vehicle = line.length();
        for (int i = line.length() / 2 - 1; i >= 0; i--)
        {
            if (line[i] != '=' && (line[i] != 'G' && line[i] != 'R'))
            {
                first_vehicle = i;
                break;
            }
        }

        sf::View view;
        view.setSize(sf::VideoMode::getDesktopMode().width, 200);
        view.setCenter(first_vehicle * sprite_size, 50); // Center it on position 2500, adjust as needed
        window->setView(view); // Apply the view to the window

        string lights_bushaltes(line.substr(line.length() / 2));

        for (int i = first_vehicle - 20; i < first_vehicle + 20; i++)
        {
            vector<sf::Texture*> texture;

            switch (lights_bushaltes[i])
            {
            case 'R':
                texture.push_back(red_texture);
                break;
            case 'G':
                texture.push_back(green_texture);
                break;
            case '|':
                texture.push_back(bushalte_texture);
                break;
            default:
                break;
            }

            switch (line[i])
            {
            case 'B':
                texture.push_back(bus_texture);
                break;
            case 'A':
                texture.push_back(auto_texture);
                break;
            case 'Z':
                texture.push_back(ziekenwagen_texture);
                break;
            case 'I':
                texture.push_back(firetruck_texture);
                break;
            case 'P':
                texture.push_back(police_texture);
                break;
            default:
                texture.push_back(baan_texture);
                break;
            }


            for (auto& t : texture)
            {
                sf::RectangleShape sprite(sf::Vector2f(sprite_size, sprite_size));
                sprite.setTexture(t);
                sprite.setPosition(i * sprite_size, 50.f); // (x, y) position on screen
                window->draw(sprite);
            }

            sf::Event event;
            while (window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window->close();
                    return;
                }
            }
        }


        window->display();
        window->clear(sf::Color::Cyan);
    }


    window->close();
}
