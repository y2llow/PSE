#include <iostream>

#include "src/simulatie/Parser.h"
#include "src/simulatie/Simulator.h"
#include "utils/Game.h"

using namespace std;


int main()
{
    // ====== 4.1 Gui ====
    cout << R"(                      ____  ____  _____    ____                        _  _
  _____ _____ _____  |  _ \/ ___|| ____|  / ___|_ __ ___   ___ _ __   | || |    _____ _____ _____
 |_____|_____|_____| | |_) \___ \|  _|   | |  _| '__/ _ \ / _ \ '_ \  | || |_  |_____|_____|_____|
 |_____|_____|_____| |  __/ ___) | |___  | |_| | | | (_) |  __/ |_) | |__   _| |_____|_____|_____|
                     |_|   |____/|_____|  \____|_|  \___/ \___| .__/     |_|
                                                              |_|                                 )" << endl;

    cout << "Start the simulation           [S]" << endl;
    cout << "Show the names of the students [T]" << endl;
    cout << "Exit the simulator             [X]" << endl;

    char c;
    do
    {
        cout << "Please enter a valid character: ";
        cin >> c;
    }
    while (c != 'X' && c != 'T' && c != 'S');

    if (c == 'X')
        return 0;
    if (c == 'T')
    {
        cout << "Abdellah El Moussaoui" << endl << "Eray Ayyildiz" << endl << "Yaman Haj Ahmad" << endl;
        return 0;
    }

    // Initialiseer de Parser klasse
    Parser::initialize();
    auto* sim = new Simulator();
    if (Parser::parseElements("../src/voorbeeldXML/voorbeeld11.xml", sim))
    {
        sim->simulate(5000);
    }

//
//     for (const auto b : sim->getBanen())
//     {
//         b->sortVoertuigenByPosition();
//
//         sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, 200), b->getNaam(),
//                                 sf::Style::Close);
//
//         window.setFramerateLimit(60);
//         Game game = Game(&window);
//         game.loadMap("../output/baan_" + b->getNaam() + ".txt");
//
//     }

    return 0;
}
