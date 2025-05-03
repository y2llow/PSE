#include <iostream>

#include "src/simulatie/Parser.h"
#include "src/simulatie/Simulator.h"
#include "utils/Game.h"

using namespace std;


int main() {

    // Om de ingelezen verkeerssituatie te kunnen simuleren moet de informatie consistent zijn
    // if (!sim->isConsistent()) {
    //     cerr << "Het verkeerssituatie is inconsistent" << endl;
    //     return 1;
    // }

    auto* sim = new Simulator();
    if (Parser::parseElements("../src/voorbeeldXML/voorbeeld2.xml", sim))
    {
        sim->simulate(10000);
    }



    for (const auto b : sim->getBanen())
    {
        b->sortVoertuigenByPosition();

        sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, 200), b->getNaam(),
                                sf::Style::Close);

        window.setFramerateLimit(60);
        Game game = Game(&window);
        game.loadMap("../output/baan_" + b->getNaam() + ".txt");
    }

    return 0;
}
