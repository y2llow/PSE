//
// Created by Abdellah on 5/4/2025.
//

#include <fstream>
#include <gtest/gtest.h>

#include "../src/simulatie/Parser.h"
#include "../src/simulatie/Simulator.h"

#include <random>
#include <iterator>

std::mt19937& getRNG() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

template <typename Container>
auto randomElement(const Container& container) -> decltype(*container.begin()) {
    auto it = container.begin();
    std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
    std::advance(it, dist(getRNG()));
    return *it;
}

class VoertuigTest : public ::testing::Test
{
public:
    Simulator* sim = new Simulator();
    vector<string> only_one_voertuig_op_baan;

    void SetUp() override
    {
        only_one_voertuig_op_baan.emplace_back("../test/test-xml-files/only_auto_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("../test/test-xml-files/only_bus_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("../test/test-xml-files/only_ziekenwagen_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("../test/test-xml-files/only_brandweerwagen_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("../test/test-xml-files/only_politiecombi_op_baan.xml");
    }

    void TearDown() override
    {
        delete sim;
    }
};

// ====== If the baan is empty and there are no voertuigen in front of a voertuig, it should get to the max speed =====
TEST_F(VoertuigTest, VoertuigMaximumSnelheid)
{
    Parser::parseElements(randomElement(only_one_voertuig_op_baan), sim);
    sim->simulate(5000);
    const auto voertuig = sim->getBanen().front()->getVoertuigen().front();
    EXPECT_NEAR(voertuig->getSnelheid(), voertuig->getMaximaleSnelheid(), 0.001);
}

int main(int argc, char* argv[])
{
    // // Redirect cerr to NUL (Windows)
    // const std::ofstream null_stream("NUL");
    // std::streambuf* old_cerr = std::cerr.rdbuf(null_stream.rdbuf());
    //
    // Initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    int result = RUN_ALL_TESTS();

    // Restore cerr after tests
    // std::cerr.rdbuf(old_cerr);

    return result;
}
