//
// Created by AbEms on 3/12/2025.
//

#include <fstream>
#include <gtest/gtest.h>

#include "../../../specificatie_1/simulation.h"
#include <cmath>

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
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml"));
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





// Test for vehicle position calculation
TEST_F(SimulationTESTS, BerekenPositieTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));

    // Initial values
    Voertuig* v = sim.getVoertuigen()[0];
    double initialPos = v->getPositie();
    double initialSpeed = v->getSnelheid();

    // Set test values
    v->setVersnelling(1.0); // Set acceleration

    // Calculate new position
    sim.berekenPositie(v);

    // Expected values using the formula from berekenPositie
    double expectedSpeed = initialSpeed + v->getVersnelling() * SIMULATIE_TIJD;
    double expectedPos = initialPos + expectedSpeed * SIMULATIE_TIJD + v->getVersnelling() * pow(SIMULATIE_TIJD, 2) / 2;

    // Check results
    EXPECT_NEAR(v->getSnelheid(), expectedSpeed, 0.001);
    EXPECT_NEAR(v->getPositie(), expectedPos, 0.001);
}

// Test for negative acceleration case in berekenPositie
TEST_F(SimulationTESTS, BerekenPositieNegativeAccelerationTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    // Initial values
    Voertuig* v = sim.getVoertuigen()[0];
    double initialPos = v->getPositie();
    double initialSpeed = v->getSnelheid();

    // Set negative acceleration large enough to bring speed to negative
    v->setVersnelling(-1000.0);

    // Calculate new position
    sim.berekenPositie(v);

    // Expected values for negative case where e < 0
    double expectedPos = initialPos - (initialSpeed * initialSpeed) / (2 * v->getVersnelling());

    // Check results
    EXPECT_NEAR(v->getSnelheid(), 0.0, 0.001); // Speed should be 0
    EXPECT_NEAR(v->getPositie(), expectedPos, 0.001);
}

// Test for traffic light update
TEST_F(SimulationTESTS, VerkeerslichtUpdateTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Verkeerslicht* licht = sim.getVerkeerslichten()[0];

    // Initial state
    bool initialRood = licht->isRood();
    bool initialGroen = licht->isGroen();

    // Update the traffic light
    licht->updateVerkeersLicht();

    // Check that the states have toggled
    EXPECT_EQ(licht->isRood(), !initialRood);
    EXPECT_EQ(licht->isGroen(), !initialGroen);
    EXPECT_EQ(licht->isRood(), !licht->isGroen()); // Should be opposite

    // Check that the time was reset
    EXPECT_LT(licht->getTijdSindsLaatsteVerandering(), 0);
}

// Test for vehicle slowdown in traffic light deceleration zone
TEST_F(SimulationTESTS, VoertuigInVertraagZoneTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Voertuig* v = sim.getVoertuigen()[0];
    Verkeerslicht* licht = sim.getVerkeerslichten()[0];

    // Position vehicle in deceleration zone
    v->setPositie(licht->getPositie() - VERTRAAG_AFSTAND + 5); // Just inside the zone

    // Test if vehicle is in deceleration zone
    EXPECT_TRUE(sim.isVoertuigInVertraagZone(v, licht));

    // Position vehicle outside deceleration zone
    v->setPositie(licht->getPositie() - VERTRAAG_AFSTAND-5); // Just outside the zone

    // Test if vehicle is not in deceleration zone
    EXPECT_FALSE(sim.isVoertuigInVertraagZone(v, licht));

    // Position vehicle in stop zone (which is not in deceleration zone)
    v->setPositie(licht->getPositie() - STOP_AFSTAND + 1);

    // Test that vehicle in stop zone is not in deceleration zone
    EXPECT_FALSE(sim.isVoertuigInVertraagZone(v, licht));
}

// Test for vehicle in stopping zone
TEST_F(SimulationTESTS, VoertuigInStopZoneTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Voertuig* v = sim.getVoertuigen()[0];
    Verkeerslicht* licht = sim.getVerkeerslichten()[0];

    // Position vehicle in stop zone (between STOP_AFSTAND/2 and STOP_AFSTAND)
    v->setPositie(licht->getPositie() - (STOP_AFSTAND * 0.75));

    // Test if vehicle is in stop zone
    EXPECT_TRUE(sim.isVoertuigInStopZone(v, licht));

    // Position vehicle outside stop zone
    v->setPositie(licht->getPositie() - STOP_AFSTAND - 1); // Just outside the zone

    // Test if vehicle is not in stop zone
    EXPECT_FALSE(sim.isVoertuigInStopZone(v, licht));

    // Position vehicle too close to light (less than STOP_AFSTAND/2)
    v->setPositie(licht->getPositie() - (STOP_AFSTAND * 0.25));

    // Test if vehicle is not in stop zone (too close)
    EXPECT_FALSE(sim.isVoertuigInStopZone(v, licht));
}

// Test for BerekenSnelheidNaVertraging method
TEST_F(SimulationTESTS, BerekenSnelheidNaVertragingTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Voertuig* v = sim.getVoertuigen()[0];

    // Store initial kvmax
    double initialKvmax = v->getKvmax();

    // Apply deceleration
    sim.BerekenSnelheidNaVertraging(v);

    // Check new kvmax is reduced by VERTRAAG_FACTOR
    EXPECT_NEAR(v->getKvmax(), MAX_SNELHEID * VERTRAAG_FACTOR, 0.001);
    EXPECT_LT(v->getKvmax(), initialKvmax);
}

// Test for BerekenSnelheidNaVersnelling method
TEST_F(SimulationTESTS, BerekenSnelheidNaVersnellingTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Voertuig* v = sim.getVoertuigen()[0];

    // First slow down the vehicle
    sim.BerekenSnelheidNaVertraging(v);
    double slowedKvmax = v->getKvmax();

    // Then restore normal speed
    sim.BerekenSnelheidNaVersnelling(v);

    // Check kvmax is restored to MAX_SNELHEID
    EXPECT_NEAR(v->getKvmax(), MAX_SNELHEID, 0.001);
    EXPECT_GT(v->getKvmax(), slowedKvmax);
}

// Test for isVoertuigOpBaan method
TEST_F(SimulationTESTS, IsVoertuigOpBaanTest) {
    EXPECT_TRUE(sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/basic_test.xml"));
    Voertuig* v = sim.getVoertuigen()[0];

    // Vehicle on road
    v->setPositie(250); // Middle of the 500-length road
    EXPECT_TRUE(sim.isVoertuigOpBaan(v));

    // Vehicle at end of road
    v->setPositie(500);
    EXPECT_TRUE(sim.isVoertuigOpBaan(v));

    // Vehicle beyond road
    v->setPositie(501);
    EXPECT_FALSE(sim.isVoertuigOpBaan(v));
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
    double expectedAcceleration = MAX_VERSNELLING * (1 - pow((v->getSnelheid() / v->getKvmax()), 4));

    // Check results
    EXPECT_NEAR(v->getVersnelling(), expectedAcceleration, 0.001);
}



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
