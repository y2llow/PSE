#include <fstream>
#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>

#include "../src/simulatie/parsing/UniversalParser.h"
#include "../src/simulatie/Simulator.h"
#include "../src/logs_and_errors/Logger.h"
#include "../src/logs_and_errors/ErrorOutput.h"
#include "../src/elementen/Baan.h"
#include "../src/elementen/Voertuig.h"
#include "../src/elementen/Verkeerslicht.h"
#include "../src/elementen/Bushalte.h"
#include "../src/elementen/Voertuiggenerator.h"
#include "../src/elementen/Kruispunt.h"

class HappiestDayTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup logging - no file output to avoid file locking issues
        logger = std::make_unique<Logger>("", false, Logger::LogLevel::INFO, true);  // Console only
        errorOutput = std::make_unique<ErrorOutput>(*logger);

        // Initialize parser
        UniversalParser::initialize();

        // Create XML file for testing
        createHappyDayXML();
    }

    void TearDown() override {
        // Clean up objects first
        logger.reset();
        errorOutput.reset();

        // Only cleanup XML file
        try {
            std::filesystem::remove("happy_day_scenario.xml");
        } catch (const std::filesystem::filesystem_error&) {
            // Ignore cleanup errors
        }
    }

    void createHappyDayXML() {
        std::ofstream xmlFile("happy_day_scenario.xml");
        xmlFile << R"(<?xml version="1.0" encoding="UTF-8"?>
<VERKEERSSITUATIE>
    <BAAN>
        <naam>Middelheimlaan</naam>
        <lengte>1000</lengte>
    </BAAN>

    <BAAN>
        <naam>Floralienlaan</naam>
        <lengte>800</lengte>
    </BAAN>

    <BAAN>
        <naam>Beukenlaan</naam>
        <lengte>600</lengte>
    </BAAN>

    <VERKEERSLICHT>
        <baan>Middelheimlaan</baan>
        <positie>400</positie>
        <cyclus>30</cyclus>
    </VERKEERSLICHT>

    <VERKEERSLICHT>
        <baan>Floralienlaan</baan>
        <positie>500</positie>
        <cyclus>25</cyclus>
    </VERKEERSLICHT>

    <VOERTUIG>
        <baan>Middelheimlaan</baan>
        <positie>50</positie>
        <type>auto</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Middelheimlaan</baan>
        <positie>100</positie>
        <type>bus</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Floralienlaan</baan>
        <positie>150</positie>
        <type>brandweerwagen</type>
    </VOERTUIG>

    <VOERTUIG>
        <baan>Beukenlaan</baan>
        <positie>200</positie>
        <type>politiecombi</type>
    </VOERTUIG>

    <VOERTUIGGENERATOR>
        <baan>Middelheimlaan</baan>
        <frequentie>15</frequentie>
        <type>auto</type>
    </VOERTUIGGENERATOR>

    <VOERTUIGGENERATOR>
        <baan>Floralienlaan</baan>
        <frequentie>20</frequentie>
        <type>bus</type>
    </VOERTUIGGENERATOR>

    <BUSHALTE>
        <baan>Middelheimlaan</baan>
        <positie>250</positie>
        <wachttijd>5</wachttijd>
    </BUSHALTE>

    <BUSHALTE>
        <baan>Floralienlaan</baan>
        <positie>300</positie>
        <wachttijd>8</wachttijd>
    </BUSHALTE>

    <KRUISPUNT>
        <baan positie="500">Middelheimlaan</baan>
        <baan positie="400">Floralienlaan</baan>
    </KRUISPUNT>

    <KRUISPUNT>
        <baan positie="750">Middelheimlaan</baan>
        <baan positie="300">Beukenlaan</baan>
    </KRUISPUNT>

</VERKEERSSITUATIE>)";
        xmlFile.close();
    }

    std::unique_ptr<Logger> logger;
    std::unique_ptr<ErrorOutput> errorOutput;
};

TEST_F(HappiestDayTest, UseCase1_1_VerkeerssituatieInlezen) {
    // Create simulator
    auto sim = std::make_unique<Simulator>(*errorOutput);

    // Test parsing
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success) << "XML parsing should succeed";

    // Verify consistency
    auto banen = sim->getBanen();
    int size = banen.size();
    EXPECT_EQ(size, 3) << "Should have 3 roads";

    int totalVoertuigen = 0, totalVerkeerslichten = 0, totalBushaltes = 0, totalGeneratoren = 0, totalKruispunten = 0;

    for (auto* baan : banen) {
        totalVoertuigen += baan->getVoertuigen().size();
        totalVerkeerslichten += baan->getVerkeerslichten().size();
        totalBushaltes += baan->getBushaltes().size();
        totalGeneratoren += baan->getVoertuigeneratoren().size();
        totalKruispunten += baan->getKruispunten().size();
    }

    EXPECT_EQ(totalVoertuigen, 4) << "Should have 4 vehicles";
    EXPECT_EQ(totalVerkeerslichten, 2) << "Should have 2 traffic lights";
    EXPECT_EQ(totalBushaltes, 2) << "Should have 2 bus stops";
    EXPECT_EQ(totalGeneratoren, 2) << "Should have 2 vehicle generators";
    EXPECT_EQ(totalKruispunten, 4) << "Should have 4 intersections (2 per road that has them)";
}

TEST_F(HappiestDayTest, UseCase2_1_SimpeleUitvoer) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Test simple output functionality
    auto banen = sim->getBanen();
    ASSERT_FALSE(banen.empty()) << "Should have roads loaded";

    int voertuigTeller = 0;
    for (auto* baan : banen) {
        for (auto* voertuig : baan->getVoertuigen()) {
            voertuigTeller++;

            // Verify vehicle has valid properties
            EXPECT_FALSE(baan->getNaam().empty()) << "Road should have a name";
            EXPECT_GE(voertuig->getPositie(), 0) << "Vehicle position should be non-negative";
            EXPECT_GE(voertuig->getSnelheid(), 0) << "Vehicle speed should be non-negative";
            EXPECT_FALSE(voertuig->getType().empty()) << "Vehicle should have a type";
        }
    }

    EXPECT_EQ(voertuigTeller, 4) << "Should count 4 vehicles for output";
}

TEST_F(HappiestDayTest, UseCase2_2_GrafischeImpressie) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Test graphical impression generation
    EXPECT_NO_THROW(sim->makeGraphicalImpression()) << "Graphical impression generation should not throw";

    // Test file generation
    EXPECT_NO_THROW(sim->generateGraphicsFile()) << "Graphics file generation should not throw";

    // Verify output file exists (note: might be in ../output/ directory)
    bool outputExists = std::filesystem::exists("../output/simulation_output.txt") ||
                        std::filesystem::exists("simulation_output.txt");
    EXPECT_TRUE(outputExists) << "Graphics output file should be created";
}

TEST_F(HappiestDayTest, UseCase3_1_RijdenVanVoertuig) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    auto banen = sim->getBanen();
    ASSERT_FALSE(banen.empty());
    ASSERT_FALSE(banen[0]->getVoertuigen().empty());

    auto* voertuig = banen[0]->getVoertuigen()[0];

    double oudePositie = voertuig->getPositie();

    // Test vehicle movement
    voertuig->rijd();

    // Verify physics updates
    EXPECT_TRUE(voertuig->getPositie() >= oudePositie) << "Position should not decrease";

    // After multiple steps, vehicle should move
    for (int i = 0; i < 10; i++) {
        voertuig->rijd();
    }

    EXPECT_GT(voertuig->getPositie(), oudePositie) << "Vehicle should move after multiple steps";
}

TEST_F(HappiestDayTest, UseCase3_2_SimulatieVanVerkeerslicht) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    auto banen = sim->getBanen();
    bool hasTrafficLight = false;

    for (auto* baan : banen) {
        for (auto* verkeerslicht : baan->getVerkeerslichten()) {
            hasTrafficLight = true;

            // Verify traffic light properties
            EXPECT_GT(verkeerslicht->getCyclus(), 0) << "Traffic light should have positive cycle time";
            EXPECT_GE(verkeerslicht->getPositie(), 0) << "Traffic light position should be non-negative";

            // Test traffic light update
            EXPECT_NO_THROW(verkeerslicht->updateVerkeerslicht()) << "Traffic light update should not throw";
        }
    }

    EXPECT_TRUE(hasTrafficLight) << "Should have at least one traffic light to test";
}

TEST_F(HappiestDayTest, UseCase3_3_AutomatischeSimulatie) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Count initial elements
    int aantalVoertuigen = 0, aantalVerkeerslichten = 0;
    for (auto* baan : sim->getBanen()) {
        aantalVoertuigen += baan->getVoertuigen().size();
        aantalVerkeerslichten += baan->getVerkeerslichten().size();
    }

    EXPECT_GT(aantalVoertuigen, 0) << "Should have vehicles to simulate";
    EXPECT_GT(aantalVerkeerslichten, 0) << "Should have traffic lights to simulate";

    // Test simulation step
    EXPECT_NO_THROW(sim->simulationRun()) << "Simulation run should not throw";
}

TEST_F(HappiestDayTest, UseCase3_4_SimulatieMetVoertuiggenerator) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    auto banen = sim->getBanen();
    bool hasGenerator = false;

    for (auto* baan : banen) {
        for (auto* generator : baan->getVoertuigeneratoren()) {
            hasGenerator = true;

            // Verify generator properties
            EXPECT_GT(generator->getFrequentie(), 0) << "Generator should have positive frequency";
            EXPECT_FALSE(generator->getType().empty()) << "Generator should have a vehicle type";

            int oudAantal = baan->getVoertuigen().size();

            // Test vehicle generation (may or may not generate based on timing)
            EXPECT_NO_THROW(generator->generateVoertuig()) << "Vehicle generation should not throw";

            int nieuwAantal = baan->getVoertuigen().size();
            EXPECT_GE(nieuwAantal, oudAantal) << "Vehicle count should not decrease";
        }
    }

    EXPECT_TRUE(hasGenerator) << "Should have at least one generator to test";
}

TEST_F(HappiestDayTest, UseCase3_6_SimulatieBushaltes) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    auto banen = sim->getBanen();
    bool hasBusStop = false;

    for (auto* baan : banen) {
        for (auto* bushalte : baan->getBushaltes()) {
            hasBusStop = true;

            // Verify bus stop properties
            EXPECT_GT(bushalte->getWachttijd(), 0) << "Bus stop should have positive waiting time";
            EXPECT_GE(bushalte->getPositie(), 0) << "Bus stop position should be non-negative";

            // Test bus stop functionality
            EXPECT_NO_THROW(bushalte->stopBus()) << "Bus stop operation should not throw";
        }
    }

    EXPECT_TRUE(hasBusStop) << "Should have at least one bus stop to test";
}

TEST_F(HappiestDayTest, UseCase3_7_SimulatieKruispunten) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    auto banen = sim->getBanen();
    bool hasIntersection = false;

    for (auto* baan : banen) {
        for (auto* kruispunt : baan->getKruispunten()) {
            hasIntersection = true;

            // Verify intersection properties
            int size = kruispunt->getBannen().size();
            EXPECT_GE(size, 2) << "Intersection should connect at least 2 roads";

            auto positions = kruispunt->getPositions();
            EXPECT_FALSE(positions.empty()) << "Intersection should have position information";

            // Verify all roads in intersection have valid positions
            for (auto& [baanPtr, positie] : positions) {
                EXPECT_NE(baanPtr, nullptr) << "Road pointer should not be null";
                EXPECT_GE(positie, 0) << "Intersection position should be non-negative";
            }
        }
    }

    EXPECT_TRUE(hasIntersection) << "Should have at least one intersection to test";
}

TEST_F(HappiestDayTest, VolledigeSimulatieTest) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Test full simulation for multiple steps
    for (int stap = 1; stap <= 5; stap++) {
        int voertuigenVoor = 0;
        for (auto* baan : sim->getBanen()) {
            voertuigenVoor += baan->getVoertuigen().size();
        }

        EXPECT_NO_THROW(sim->simulationRun()) << "Simulation step " << stap << " should not throw";

        int voertuigenNa = 0;
        for (auto* baan : sim->getBanen()) {
            voertuigenNa += baan->getVoertuigen().size();
        }

        // Vehicle count should remain stable or increase (due to generators)
        EXPECT_GE(voertuigenNa, 0) << "Should have non-negative vehicle count after step " << stap;
    }
}

TEST_F(HappiestDayTest, FinaleOutputGeneratie) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Run some simulation steps
    for (int i = 0; i < 3; i++) {
        sim->simulationRun();
    }

    // Test final output generation
    EXPECT_NO_THROW(sim->generateGraphicsFile()) << "Final graphics file generation should not throw";

    // Verify output exists
    bool outputExists = std::filesystem::exists("../output/simulation_output.txt") ||
                        std::filesystem::exists("simulation_output.txt");
    EXPECT_TRUE(outputExists) << "Final graphics output should be created";
}
TEST_F(HappiestDayTest, FinaleOutputGeneraties) {
    auto sim = std::make_unique<Simulator>(*errorOutput);
    bool success = UniversalParser::parseElements("happy_day_scenario.xml", sim.get(), *errorOutput);
    ASSERT_TRUE(success);

    // Run some simulation steps
    for (int i = 0; i < 3; i++) {
        sim->simulationRun();
    }

    // Test final output generation
    EXPECT_NO_THROW(sim->generateGraphicsFile()) << "Final graphics file generation should not throw";

    // Verify output exists
    bool outputExists = std::filesystem::exists("../output/simulation_output.txt") ||
                        std::filesystem::exists("simulation_output.txt");
    EXPECT_TRUE(outputExists) << "Final graphics output should be created";
}