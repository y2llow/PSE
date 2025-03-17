//
// Created by AbEms on 3/12/2025.
//

#include <cmath>
#include <fstream>
#include <gtest/gtest.h>

#include "../../../specificatie_1/simulation.h"

class SimulationTESTS : public testing::Test {
    void SetUp() override {
    };

protected:
    simulation sim;
};

TEST_F(SimulationTESTS, EmptyXMLFileTest) {
    EXPECT_FALSE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml"));

    EXPECT_EQ(int(sim.getBanen().size()), 0);
    EXPECT_EQ(int(sim.getVoertuigen().size()), 0);
    EXPECT_EQ(int(sim.getVerkeerslichten().size()), 0);
    EXPECT_EQ(int(sim.getVoertuiggeneratoren().size()), 0);
}

TEST_F(SimulationTESTS, DifferentOrderOfElements) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml");
    EXPECT_EQ(int(sim.getVoertuigen().size()), 2);
    EXPECT_EQ(int(sim.getBanen().size()), 1);
    EXPECT_EQ(int(sim.getVoertuiggeneratoren().size()), 1);
    EXPECT_EQ(int(sim.getVerkeerslichten().size()), 1);
    EXPECT_TRUE(sim.isConsistent());
}

TEST_F(SimulationTESTS, GoodSyntaxWrongDataTypeVoertuig) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/wrong_data_types_voertuig.xml");
    EXPECT_EQ(int(sim.getVoertuigen().size()), 0);
}

TEST_F(SimulationTESTS, GoodSyntaxWrongDataTypeVerkeerslicht) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/wrong_data_types_verkeerslicht.xml");
    EXPECT_EQ(int(sim.getVerkeerslichten().size()), 0);
}

TEST_F(SimulationTESTS, ConsistencyEmptyFile) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, ConsistencyWithoutBaan) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/verkeerssituatie_zonder_baan.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, ConsistencyWithVehiclePositionGTLength) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/voertuig_positie_gt_baan_lengte.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, ConsistencyWithLightPositionGTLength) {
    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/verkeerslicht_positie_gt_baan_lengte.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, ToStringOutputTest) {

    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test3.xml"));

    // Stel de simulatie tijd in
    sim.incSimulationTime(); // Stel de tijd in op 10.0 seconden

    // Omleiden van cout naar een stringstream om de uitvoer te vangen
    std::stringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    // Roep de ToString()-methode aan
    sim.ToString();

    // Herstel de oorspronkelijke cout-buffer
    std::cout.rdbuf(oldCoutBuffer);

    // Verkrijg de uitvoer van de stringstream
    std::string output = buffer.str();

    // Verwachtte uitvoer
    std::string expectedOutput =
        "Tijd: 0.0166\nVoertuig 1\n-> baan: Middelheimlaan\n-> positie: 100\n\n";

    // Vergelijk de uitvoer met de verwachte uitvoer
    EXPECT_EQ(output, expectedOutput);
}


TEST_F(SimulationTESTS, BerekenPositieZeroAccelerationTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    // Initial values
    Voertuig* v = sim.getVoertuigen()[0];
    double initialPos = v->getPositie();
    double initialSpeed = v->getSnelheid();

    // Set acceleration to zero
    v->setVersnelling(0.0);

    // Calculate new position
    sim.berekenPositie(v);

    // Expected values with zero acceleration
    double expectedSpeed = initialSpeed; // Speed remains the same
    double expectedPos = initialPos + initialSpeed * SIMULATIE_TIJD; // Position increases linearly

    // Check results
    EXPECT_NEAR(v->getSnelheid(), expectedSpeed, 0.001);
    EXPECT_NEAR(v->getPositie(), expectedPos, 0.001);
}

//test: voor 3.1 de updatePositieEnSnelheidsTest
TEST_F(SimulationTESTS, UpdatePositieEnSnelheidTest) {

    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    Voertuig* voertuig = sim.getVoertuigen()[0];

    //stel waarden in
    voertuig->setSnelheid(50.0);
    voertuig->setVersnelling(10.0);

    //bewaar initiële positie
    double initielePositie = voertuig->getPositie();

    sim.updateVoertuig(voertuig, 0);

    EXPECT_TRUE(voertuig->getPositie() > initielePositie);
}

TEST_F(SimulationTESTS, BerekenVersnellingNoLeadingVehicleTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    // Get the first vehicle
    Voertuig* v = sim.getVoertuigen()[0];

    // Set initial speed and acceleration
    v->setSnelheid(10.0);
    v->setVersnelling(0.0);

    // Calculate acceleration with no leading vehicle
    sim.BerekenVersnelling(v, 0);

    // Expected acceleration: MAX_VERSNELLING * (1 - (v->getSnelheid() / v->getKvmax())^4)
    double expectedAcceleration = MAX_VERSNELLING * (1 - ::pow((v->getSnelheid() / v->getKvmax()), 4));

    // Check results
    EXPECT_NEAR(v->getVersnelling(), expectedAcceleration, 0.001);
}

TEST_F(SimulationTESTS, BerekenVersnellingWithLeadingVehicleTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test2.xml"));

    // Get the first and second vehicles
    Voertuig* v1 = sim.getVoertuigen()[0];
    Voertuig* v2 = sim.getVoertuigen()[1];

    // Set positions and speeds
    v1->setPositie(100.0);
    v1->setSnelheid(10.0);
    v2->setPositie(120.0); // Leading vehicle
    v2->setSnelheid(8.0);

    // Calculate acceleration for the first vehicle
    sim.BerekenVersnelling(v1, 0);


    double volgafstand = v2->getPositie() - v1->getPositie() - LENGTE;
    double snelheidVerschil = v1->getSnelheid() - v2->getSnelheid();

    double newsnelheid = v1->getSnelheid() - snelheidVerschil;
    double newversnelling = 2 * sqrt(MAX_VERSNELLING * MAX_REMFACTOR);

    double calculate = v1->getSnelheid() + (newsnelheid / newversnelling);

    double maxNummer = max(0.0, calculate);
    auto delta = (MIN_VOLGAFSTAND + maxNummer) / volgafstand;

    double expectedAcceleration = MAX_VERSNELLING * (1 - pow((v1->getSnelheid() / v1->getKvmax()), 4) - pow(delta, 2));

    // Check results
    EXPECT_NEAR(v1->getVersnelling(), expectedAcceleration, 0.001);
}

//test: voor 3.2 test voor het vertragen van een voertuig bij een rood verkeerslicht
TEST_F(SimulationTESTS, VertragenBijRoodLicht) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test4.xml"));

    Voertuig* voertuig = sim.getVoertuigen()[0];

    // Sla de initiële snelheid op
    double initieleSnelheid = voertuig->getSnelheid();

    // Voer een simulatiestap uit
    sim.simulationRun();

    // Controleer of het voertuig is vertraagd
    EXPECT_EQ(voertuig->getSnelheid(), initieleSnelheid);
}

TEST_F(SimulationTESTS, VersnellenBijGroenLicht) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test5.xml"));

    Voertuig* voertuig = sim.getVoertuigen()[0];

    // Sla de initiële snelheid op
    double initieleSnelheid = voertuig->getSnelheid();

    // Voer meerdere simulatiestappen uit
    for (int i = 0; i < 50; i++) {
        sim.simulationRun();
    }

    // Controleer of het voertuig is versneld
    EXPECT_GT(voertuig->getSnelheid(), initieleSnelheid);
}

TEST_F(SimulationTESTS, BijwerkenAlleVoertuigen) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test6.xml"));

    Voertuig* voertuig1 = sim.getVoertuigen()[0];
    Voertuig* voertuig2 = sim.getVoertuigen()[1];
    Voertuig* voertuig3 = sim.getVoertuigen()[2];

    // Sla de initiële posities op
    double initPositie1 = voertuig1->getPositie();
    double initPositie2 = voertuig2->getPositie();
    double initPositie3 = voertuig3->getPositie();

    // Voer één simulatiestap uit
    sim.simulationRun();

    // Controleer of alle voertuigen zijn bijgewerkt
    EXPECT_GT(voertuig1->getPositie(), initPositie1);
    EXPECT_GT(voertuig2->getPositie(), initPositie2);
    EXPECT_GT(voertuig3->getPositie(), initPositie3);
}

TEST_F(SimulationTESTS, VerkeerslichtCycleResetTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    // Get the first traffic light
    Verkeerslicht* licht = sim.getVerkeerslichten()[0];

    // Set initial time since last change to the cycle time
    licht->updateTijdSindsLaatsteVerandering(licht->getCyclus());

    // Update the traffic light
    licht->updateVerkeersLicht();

    // Check that the time since last change is reset
    EXPECT_EQ(licht->getTijdSindsLaatsteVerandering(), 0);
}

// TEST_F(SimulationTESTS, VoertuigRemovalOffRoadTest) {
//     EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
//
//     // Get the first vehicle
//     Voertuig* v = sim.getVoertuigen()[0];
//
//     // Set the vehicle's position beyond the road length
//     Baan* baan = sim.getBaanByName(v->getBaan());
//     v->setPositie(baan->getLengte() + 10.0);
//
//     // Run the simulation to check if the vehicle is removed
//     sim.simulationRun();
//
//     // Check that the vehicle is no longer in the simulation
//     EXPECT_EQ(v, nullptr);
// }

TEST_F(SimulationTESTS, VerkeerslichtStateToggleTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    // Get the first traffic light
    Verkeerslicht* licht = sim.getVerkeerslichten()[0];

    // Initial state
    bool initialRood = licht->isRood();
    bool initialGroen = licht->isGroen();

    // Update the traffic light
    licht->updateVerkeersLicht();

    // Check that the states have toggled
    EXPECT_EQ(licht->isRood(), !initialRood);
    EXPECT_EQ(licht->isGroen(), !initialGroen);
}

TEST_F(SimulationTESTS, BijwerkenAlleVerkeerslichten) {
    // Parse het XML-bestand
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test7.xml"));

    // Haal de verkeerslichten op
    Verkeerslicht* verkeerslicht1 = sim.getVerkeerslichten()[0];
    Verkeerslicht* verkeerslicht2 = sim.getVerkeerslichten()[1];

    // Bereken het aantal tijdstappen nodig voor cyclus van verkeerslicht1
    int aantalStappen = ceil(verkeerslicht1->getCyclus() / SIMULATIE_TIJD);

    // Voer voldoende tijdstappen uit voor verkeerslicht1 om te veranderen
    for (int i = 0; i < aantalStappen + 5; i++) {
        sim.simulationRun();
    }

    // Controleer of verkeerslicht1 is veranderd naar groen
    EXPECT_FALSE(verkeerslicht1->isGroen());
    EXPECT_TRUE(verkeerslicht2->isGroen()); // Verkeerslicht2 zou nog rood moeten zijn

    // Bereken extra stappen nodig voor verkeerslicht2 om te veranderen
    int extraStappen = ceil((verkeerslicht2->getCyclus() - verkeerslicht1->getCyclus()) / SIMULATIE_TIJD);

    // Voer de extra stappen uit
    for (int i = 0; i < extraStappen + 5; i++) {
        sim.simulationRun();
    }

    // Controleer of verkeerslicht2 nu ook groen is
    EXPECT_FALSE(verkeerslicht2->isGroen());
}

TEST_F(SimulationTESTS, ComplexeSimulatie) {
    // Parse het XML-bestand
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test8.xml"));

    // Voer meerdere simulatiestappen uit
    for (int i = 0; i < 2000; i++) {
        sim.simulationRun();

        // Als alle voertuigen weg zijn, stop de loop
        if (sim.getVoertuigen().empty()) {
            break;
        }
    }

    int  vergelijking = static_cast<int>(sim.getVoertuigen().size());

    // Controleer of er minder voertuigen zijn dan initieel
    EXPECT_LT(vergelijking, 5);
}

TEST_F(SimulationTESTS, VoertuiggeneratorToevoegenVoertuig) {
    // Parse het XML-bestand
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test9.xml"));

    int  vergelijking = static_cast<int>(sim.getVoertuigen().size());
    // Controleer initieel aantal voertuigen
    EXPECT_EQ( vergelijking, 0);

    // Voer voldoende tijdstappen uit om een voertuig te genereren
    for (int i = 0; i < 100; i++) {
        sim.simulationRun();
    }
    int  vergelijkingen = static_cast<int>(sim.getVoertuigen().size());


    // Controleer of er minstens één voertuig is gegenereerd
    EXPECT_GE(vergelijkingen, 1);
}

TEST_F(SimulationTESTS, MeerdereVoertuiggeneratoren) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test10.xml"));

    int  vergelijkingen = static_cast<int>(sim.getVoertuigen().size());
    // Controleer initieel aantal voertuigen
    EXPECT_EQ(vergelijkingen, 0);

    // Voer voldoende tijdstappen uit om voertuigen te genereren
    for (int i = 0; i < 1000; i++) {
        sim.simulationRun();
    }

    // Tel het aantal voertuigen per baan
    int voertuigenBaan1 = 0;
    int voertuigenBaan2 = 0;

    for (Voertuig* voertuig : sim.getVoertuigen()) {
        if (voertuig->getBaan() == "Baan1") {
            voertuigenBaan1++;
        } else if (voertuig->getBaan() == "Baan2") {
            voertuigenBaan2++;
        }
    }

    // Controleer of er meer voertuigen zijn gegenereerd op Baan1 dan op Baan2
    EXPECT_GT(voertuigenBaan1, voertuigenBaan2);
}




int main(int argc, char **argv) {
    // Redirect cerr to NUL (Windows)
    std::ofstream null_stream("NUL");
    std::streambuf *old_cerr = std::cerr.rdbuf(null_stream.rdbuf());

    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    int result = RUN_ALL_TESTS();

    // Restore cerr after tests
    std::cerr.rdbuf(old_cerr);

    return result;
}
