#include <iostream>
#include <memory>

#include "src/simulatie/Parser.h"
#include "src/simulatie/Simulator.h"
#include "src/simulatie/XMLParser.h"
#include "src/logs_and_errors/Logger.h"
#include "src/logs_and_errors/ErrorOutput.h"

using namespace std;

int main()
{
    // Setup logging system
    Logger logger(R"(C:\Users\Familie Goyvaerts\CLionProjects\PSE\output\errors.txt)",
                  false,                    // Geen console output
                  Logger::LogLevel::ERROR,      // Alleen errors
                  true);                        // Clear file eerst
    ErrorOutput errorOutput(logger);

    try {
        Parser::initialize();

        // Gebruik ErrorOutput constructor
        auto sim = std::make_unique<Simulator>(errorOutput);

        // Test eerst met je originele XML
        if (Parser::parseElements("../src/voorbeeldXML/voorbeeld.xml", sim.get(), errorOutput)) {
            sim->simulate(100);
        } else {
            errorOutput.logError("XML parsing gefaald");
            return 1;
        }

    } catch (const std::exception& e) {
        errorOutput.handleException(e, "main");
        return 1;
    } catch (...) {
        errorOutput.logError("Onbekende fout opgetreden in main");
        return 1;
    }

    return 0;
}



//int main()
//{
//    //    // ====== 4.1 Gui ====
//    //    cout << R"(                      ____  ____  _____    ____                        _  _
//    //  _____ _____ _____  |  _ \/ ___|| ____|  / ___|_ __ ___   ___ _ __   | || |    _____ _____ _____
//    // |_____|_____|_____| | |_) \___ \|  _|   | |  _| '__/ _ \ / _ \ '_ \  | || |_  |_____|_____|_____|
//    // |_____|_____|_____| |  __/ ___) | |___  | |_| | | | (_) |  __/ |_) | |__   _| |_____|_____|_____|
//    //                     |_|   |____/|_____|  \____|_|  \___/ \___| .__/     |_|
//    //                                                              |_|                                 )" << endl;
//    //
//    //    cout << "Start the simulation           [S]" << endl;
//    //    cout << "Show the names of the students [T]" << endl;
//    //    cout << "Exit the simulator             [X]" << endl;
//    //
//    //    char c;
//    //    do
//    //    {
//    //        cout << "Please enter a valid character: ";
//    //        cin >> c;
//    //    }
//    //    while (c != 'X' && c != 'T' && c != 'S');
//    //
//    //    if (c == 'X')
//    //        return 0;
//    //    if (c == 'T')
//    //    {
//    //        cout << "Abdellah El Moussaoui" << endl << "Eray Ayyildiz" << endl << "Yaman Haj Ahmad" << endl;
//    //        return 0;
//    //    }
//
//
//    // XMLParser xmlParser;
//    // const auto bannen = xmlParser.parseElements("../src/voorbeeldXML/voorbeeld12.xml");
//    // auto* simulator = new Simulator(bannen);
//    //
//    // if (!simulator->is_consistent)
//    // {
//    //     cerr << "Simulator consistency error" << endl;
//    //     return -1;
//    // }
//
//    // Initialiseer de Parser, Logger en ErrorOutput klasse
//    Parser::initialize();
//    Logger logger;
//    ErrorOutput errorOutput(logger);
//
//    logger.log("Application start");
//
//
//    auto* sim = new Simulator();
//    if (Parser::parseElements("../src/voorbeeldXML/voorbeeld12.xml", sim))
//    {
//        sim->simulate(10);
//    }
//
//    logger.log("Application finished");
//    //
//    //     for (const auto b : sim->getBanen())
//    //     {
//    //         b->sortVoertuigenByPosition();
//    //
//    //         sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, 200), b->getNaam(),
//    //                                 sf::Style::Close);
//    //
//    //         window.setFramerateLimit(60);
//    //         Game game = Game(&window);
//    //         game.loadMap("../output/baan_" + b->getNaam() + ".txt");
//    //
//    //     }
//
//    return 0;
//}
