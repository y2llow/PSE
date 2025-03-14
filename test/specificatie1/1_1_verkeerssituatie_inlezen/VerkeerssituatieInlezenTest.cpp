//
// Created by AbEms on 3/12/2025.
//

#include <gtest/gtest.h>

#include "../../../specificatie_1/simulation.h"

class EmptyState : public testing::Test {
    void SetUp() override {
        vi.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml");
        b_size = vi.getBanen().size();
        v_size = vi.getVoertuigen().size();
        ver_size = vi.getVerkeerslichten().size();
        vtg_size = vi.getVoertuiggeneratoren().size();
    }
protected:
    simulation vi;
    int b_size = 0, v_size = 0, ver_size = 0, vtg_size = 0;
};

TEST_F(EmptyState, EmptyXMLFileTest) {
    ASSERT_EQ(b_size, 0);
    ASSERT_EQ(v_size, 0);
    ASSERT_EQ(ver_size, 0);
    ASSERT_EQ(vtg_size, 0);
}

class WrongDataType : public testing::Test {
protected:
    simulation vi;
};

TEST_F(WrongDataType, GoodSyntaxWrongDataType) {
    EXPECT_THROW(vi.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/wrong_data_types.xml"), invalid_argument);
}

#include "../../../specificatie_1/simulation.h"
#include "../../../src/elementen/Voertuig.h"

// Test fixture voor de ToString functie
class ToStringTest : public testing::Test {
protected:
    simulation sim;

    void SetUp() override {
            sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/outputTESTfile.xml");
        }

    void TearDown() override {
        // Opruimen van dynamisch toegewezen geheugen
        for (auto voertuig : sim.getVoertuigen()) {
            delete voertuig;
        }
        sim.getVoertuigen().clear();
    }
};

// Test of de ToString functie de juiste uitvoer produceert
TEST_F(ToStringTest, OutputFormatTest) {
    // Redirect cout naar een stringstream om de uitvoer te kunnen testen
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    // Roep de ToString functie aan
    sim.ToString();

    // Herstel cout
    std::cout.rdbuf(oldCout);

    // Verwachte uitvoer
    std::string expectedOutput =
        "Tijd: 0\n"
        "Voertuig 1\n"
        "-> baan: Middelheimlaan\n"
        "-> positie: 20\n"
        "-> snelheid: 16.6\n"
        "Voertuig 2\n"
        "-> baan: Middelheimlaan\n"
        "-> positie: 0\n"
        "-> snelheid: 16.6\n";

    // Vergelijk de verwachte uitvoer met de werkelijke uitvoer
    ASSERT_EQ(buffer.str(), expectedOutput);
}

// Test of de ToString functie correct omgaat met een lege lijst van voertuigen
TEST_F(ToStringTest, EmptyVoertuigenListTest) {
    // Verwijder alle voertuigen uit de simulatie
    for (auto voertuig : sim.getVoertuigen()) {
        delete voertuig;
    }
    sim.getVoertuigen().clear();

    // Redirect cout naar een stringstream om de uitvoer te kunnen testen
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    // Roep de ToString functie aan
    sim.ToString();

    // Herstel cout
    std::cout.rdbuf(oldCout);

    // Verwachte uitvoer (alleen de tijd, geen voertuigen)
    std::string expectedOutput = "Tijd: 0\n";

    // Vergelijk de verwachte uitvoer met de werkelijke uitvoer
    ASSERT_EQ(buffer.str(), expectedOutput);
}

/*
// Test fixture voor de simulationRun functie
class SimulationRunTest : public testing::Test {
protected:
    simulation sim;

    void SetUp() override {
        sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/outputTESTfile.xml");
    }

    void TearDown() override {
        // Opruimen van dynamisch toegewezen geheugen
        for (auto voertuig : sim.getVoertuigen()) {
            delete voertuig;
        }
        sim.getVoertuigen().clear();

        for (auto baan : sim.getBanen()) {
            delete baan;
        }
        sim.getBanen().clear();
    }
};

// Test of de simulationRun functie de positie, snelheid en versnelling van voertuigen correct bijwerkt
TEST_F(SimulationRunTest, SimulationRunUpdatesVehicleState) {
    // Voer de simulatie uit
    sim.simulationRun();

    // Controleer de staat van de voertuigen na de simulatie
    for (Voertuig* voertuig : sim.getVoertuigen()) {
        // Controleer of de positie is bijgewerkt
        EXPECT_GT(voertuig->getPositie(), 0) << "Positie van voertuig " << voertuig->getId() << " is niet bijgewerkt.";

        // Controleer of de snelheid is bijgewerkt
        EXPECT_NEAR(voertuig->getSnelheid(), 16.6 + 1.44 * sim.getSimulationTime(), 0.01)
            << "Snelheid van voertuig " << voertuig->getId() << " is niet correct bijgewerkt.";

        // Controleer of de versnelling is bijgewerkt
        EXPECT_NEAR(voertuig->getVersnelling(), 1.44, 0.01)
            << "Versnelling van voertuig " << voertuig->getId() << " is niet correct bijgewerkt.";
    }
}
// Test of de simulationRun functie correct omgaat met een lege lijst van voertuigen
TEST_F(SimulationRunTest, SimulationRunWithEmptyVehicleList) {
    // Verwijder alle voertuigen uit de simulatie
    for (auto voertuig : sim.getVoertuigen()) {
        delete voertuig;
    }
    sim.getVoertuigen().clear();

    // Voer de simulatie uit
    EXPECT_NO_THROW(sim.simulationRun()) << "simulationRun gooit een uitzondering bij een lege lijst van voertuigen.";
}

// Test of de simulatie consistent blijft na het uitvoeren van simulationRun
TEST_F(SimulationRunTest, SimulationRunMaintainsConsistency) {
    // Voer de simulatie uit
    sim.simulationRun();

    // Controleer of de simulatie consistent blijft
    EXPECT_TRUE(sim.isConsistent()) << "De simulatie is niet consistent na het uitvoeren van simulationRun.";
}

// Test of de voertuigen correct worden gesorteerd op positie
TEST_F(SimulationRunTest, VehiclesAreSortedByPosition) {
    // Sorteer de voertuigen op positie
    sim.sortVoertuigenByPosition();

    // Controleer of de voertuigen correct zijn gesorteerd
    double previousPosition = -1;
    for (Voertuig* voertuig : sim.getVoertuigen()) {
        EXPECT_GE(voertuig->getPositie(), previousPosition)
            << "Voertuigen zijn niet correct gesorteerd op positie.";
        previousPosition = voertuig->getPositie();
    }
}

// Test of de simulatie tijd correct wordt bijgewerkt
TEST_F(SimulationRunTest, SimulationTimeIsUpdated) {
    double initialTime = sim.getSimulationTime();

    // Voer de simulatie uit
    sim.simulationRun();

    // Controleer of de simulatie tijd is bijgewerkt
    EXPECT_GT(sim.getSimulationTime(), initialTime)
        << "De simulatie tijd is niet bijgewerkt na het uitvoeren van simulationRun.";
}

*/

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}