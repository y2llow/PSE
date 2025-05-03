#include <iostream>

#include "src/simulatie/Parser.h"
#include "src/simulatie/Simulator.h"
#include <SFML/Graphics.hpp>

#include "utils/Game.h"

using namespace std;


int main() {

    // Om de ingelezen verkeerssituatie te kunnen simuleren moet de informatie consistent zijn
    // if (!sim->isConsistent()) {
    //     cerr << "Het verkeerssituatie is inconsistent" << endl;
    //     return 1;
    // }

    const auto sim = new Simulator();
    if (Parser::parseElements("../tests/voorbeeld/invoer_inlezen.xml", sim))
    {
        sim->simulate(5000);
    }

    sf::RenderWindow window(sf::VideoMode(200, 200), "b->getNaam()",
                        sf::Style::Close);

    // for (const auto b : sim->getBanen())
    // {
    //     b->sortVoertuigenByPosition();
    //
    //     sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, 200), b->getNaam(),
    //                             sf::Style::Close);
    //     Game game = Game(&window);
    //     game.loadMap("../output/baan_" + b->getNaam() + ".txt");
    // }
    // delete sim;


    return 0;
}
