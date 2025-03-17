//
// Created by AbEms on 3/12/2025.
//

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
    sim.parseXMLAndCreateObjects(
        "../test/specificatie1/1_1_verkeerssituatie_inlezen/verschillende_volgorde_van_elementen.xml");
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
    sim.parseXMLAndCreateObjects(
        "../test/specificatie1/1_1_verkeerssituatie_inlezen/wrong_data_types_verkeerslicht.xml");
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
    sim.parseXMLAndCreateObjects(
        "../test/specificatie1/1_1_verkeerssituatie_inlezen/voertuig_positie_gt_baan_lengte.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, ConsistencyWithLightPositionGTLength) {
    sim.parseXMLAndCreateObjects(
        "../test/specificatie1/1_1_verkeerssituatie_inlezen/verkeerslicht_positie_gt_baan_lengte.xml");
    ASSERT_FALSE(sim.isConsistent());
}

TEST_F(SimulationTESTS, VoertuigWordtVerwijderd) {
    // De voertuig moet verwijderd worden als het buidten de baan is

    sim.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/voertuig_verwijderen.xml");
    int initial_size = sim.getVoertuigen().size();

    for (int i = 0; i < 10; i++) {
        sim.simulationRun();
    }

    int new_size = sim.getVoertuigen().size();

    ASSERT_EQ(new_size, initial_size - 1);
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
