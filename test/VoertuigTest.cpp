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
        only_one_voertuig_op_baan.emplace_back("xml/VoertuigTests/only_auto_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("xml/VoertuigTests/only_bus_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("xml/VoertuigTests/only_ziekenwagen_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("xml/VoertuigTests/only_brandweerwagen_op_baan.xml");
        only_one_voertuig_op_baan.emplace_back("xml/VoertuigTests/only_politiecombi_op_baan.xml");
    }

    void TearDown() override
    {
        delete sim;
    }
};

// ====== If the baan is empty and there are no voertuigen in front of a voertuig, it should get to the max speed =====
TEST_F(VoertuigTest, VoertuigMaximumSnelheid) {
    // Test each vehicle type configuration
    for (const auto& test_file : only_one_voertuig_op_baan) {
        // Reset simulator for each test
        delete sim;
        sim = new Simulator();

        // Parse the test file
        ASSERT_TRUE(Parser::parseElements(test_file, sim))
                                    << "Failed to parse test file: " << test_file;

        // Verify initial conditions
        ASSERT_EQ(int(sim->getBanen().size()), 1) << "Should have exactly one road";
        ASSERT_EQ(int(sim->getBanen()[0]->getVoertuigen().size()), 1)
                                    << "Should have exactly one vehicle on road";

        const auto baan = sim->getBanen()[0];
        const auto voertuig = baan->getVoertuigen()[0];

        // Verify initial speed is 0
        EXPECT_DOUBLE_EQ(voertuig->getSnelheid(), 0.0)
                            << "Vehicle should start at 0 speed";

        // Run simulation long enough to reach max speed
        sim->simulate(2000);

        // Verify final speed
        const double expected_speed = voertuig->getMaximaleSnelheid();
        const double actual_speed = voertuig->getSnelheid();

        EXPECT_NEAR(actual_speed, expected_speed, 0.1)
                            << "Vehicle in " << test_file << " didn't reach max speed. Expected: "
                            << expected_speed << ", Actual: " << actual_speed;

        // Additional verification - should be accelerating the whole time
        EXPECT_GT(voertuig->getVersnelling(), 0)
                            << "Vehicle should still have positive acceleration";
    }
}

TEST_F(VoertuigTest, VoertuigCreatieEnProperties) {
    // Test alle voertuigtypes
    vector<string> types = {"auto", "bus", "brandweerwagen", "politiecombi", "ziekenwagen"};

    for (const auto& type : types) {
        Voertuig* v = Voertuig::createVoertuig(type);

        // Test type-specifieke properties
        EXPECT_GT(v->getLengte(), 0) << type << " heeft ongeldige lengte";
        EXPECT_GT(v->getMaximaleSnelheid(), 0) << type << " heeft ongeldige max snelheid";
        EXPECT_GT(v->getMaximaleVersnelling(), 0) << type << " heeft ongeldige versnelling";
        EXPECT_GT(v->getMaximaleRemfactor(), 0) << type << " heeft ongeldige remfactor";

        // Test prioriteitsvoertuigen
        if (type == "brandweerwagen" || type == "ziekenwagen" || type == "politiecombi") {
            EXPECT_TRUE(v->isPrioriteitsVoertuig()) << type << " zou prioriteitsvoertuig moeten zijn";
        } else {
            EXPECT_FALSE(v->isPrioriteitsVoertuig()) << type << " zou geen prioriteitsvoertuig moeten zijn";
        }

        delete v;
    }

    // Test ongeldig type
    testing::internal::CaptureStderr();
    Voertuig* v = Voertuig::createVoertuig("onbekend");
    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(v->getType(), "Auto") << "Ongeldig type zou default auto moeten gebruiken";
    delete v;
}

TEST_F(VoertuigTest, VersnellingsGedrag) {
    Voertuig* v = Voertuig::createVoertuig("auto");
    Baan* b = new Baan();
    b->setLengte(1000);
    v->setBaan(b);
    b->addVoertuig(v);

    // Test initiële versnelling
    EXPECT_DOUBLE_EQ(v->getVersnelling(), 0);

    // Zet in beweging
    v->setState(State::DRIVING);
    v->rijd();

    // Simuleer enkele stappen
    for (int i = 0; i < 10; i++) {
        double prev_speed = v->getSnelheid();
        v->rijd();
        EXPECT_GT(v->getSnelheid(), prev_speed) << "Snelheid zou moeten toenemen";
        EXPECT_GT(v->getVersnelling(), 0) << "Versnelling zou positief moeten zijn";
    }

    delete b;
    delete v;
}

TEST_F(VoertuigTest, RemGedrag) {
    Voertuig* v = Voertuig::createVoertuig("auto");
    Baan* b = new Baan();
    b->setLengte(1000);
    v->setBaan(b);
    b->addVoertuig(v);
    sim->addBaan(b);

    // Bereik eerst snelheid
    v->setState(State::DRIVING);
    sim->simulate(1000);

    // Activeer remmen
    double prev_speed = v->getSnelheid();
    v->setVersnelling(-10);
    EXPECT_LT(v->getVersnelling(), 0) << "Versnelling zou negatief moeten zijn";
    v->rijd();
    EXPECT_LT(v->getSnelheid(), prev_speed) << "Snelheid zou moeten afnemen";

    delete b;
    delete v;
}

TEST_F(VoertuigTest, VolgAfstandGedrag) {
    // Maak 2 voertuigen opzelfde baan
    Voertuig* v1 = Voertuig::createVoertuig("auto");
    Voertuig* v2 = Voertuig::createVoertuig("auto");
    Baan* b = new Baan();
    b->setLengte(1000);

    v1->setBaan(b);
    v2->setBaan(b);
    v1->setPositie(100);
    v2->setPositie(150); // v2 zit voor v1
    b->addVoertuig(v2);
    b->addVoertuig(v1);

    // Zet beide in beweging
    v1->setState(State::DRIVING);
    v2->setState(State::DRIVING);

    // Simuleer
    for (int i = 0; i < 20; i++) {
        double prev_accel = v1->getVersnelling();
        v1->rijd();
        v1->rijd();

        // Controleer of volgvoertuig aanpast aan voorligger
        double afstand = v2->getPositie() - v1->getPositie() - v1->getLengte();
        if (afstand < v1->getMinimaleVolgafstand() * 2) {
            EXPECT_LT(v1->getVersnelling(), prev_accel) << "Versnelling zou moeten afnemen bij kleine afstand";
        }
    }

    delete b;
    delete v1;
    delete v2;
}

TEST_F(VoertuigTest, KruispuntGedrag) {
    // Maak 2 banen met kruispunt
    Baan* b1 = new Baan();
    Baan* b2 = new Baan();
    b1->setLengte(1000);
    b2->setLengte(1000);

    // Voeg kruispunt toe op positie 500
    b1->addKruispunt(500, b2);
    b2->addKruispunt(500, b1);

    // Maak voertuig dat naar kruispunt rijdt
    Voertuig* v = Voertuig::createVoertuig("auto");
    v->setBaan(b1);
    v->setPositie(400);
    v->setSnelheid(20);
    v->setState(State::DRIVING);
    b1->addVoertuig(v);

    // Simuleer tot aan kruispunt
    while (v->getPositie() < 500) {
        v->Kruispunt();
        EXPECT_EQ(v->getBaan(), b1) << "Nog niet op kruispunt, zou opzelfde baan moeten blijven";
    }

    // Na kruispunt - zou van baan kunnen veranderen
    v->Kruispunt();
    bool changed = (v->getBaan() == b2);

    // Controleer of positie correct is aangepast
    if (changed) {
        EXPECT_EQ(v->getPositie(), 500) << "Positie zou moeten overeenkomen met kruispuntpositie op nieuwe baan";
        EXPECT_EQ(int(b2->getVoertuigen().size()), 1) << "Voertuig zou op nieuwe baan moeten staan";
        EXPECT_EQ(int(b1->getVoertuigen().size()), 0) << "Voertuig zou van oude baan verwijderd moeten zijn";
    } else {
        EXPECT_GT(v->getPositie(), 500) << "Voertuig zou door moeten rijden";
    }

    delete b1;
    delete b2;
    delete v;
}

TEST_F(VoertuigTest, VerwijderingAanEindBaan) {
    Voertuig* v = Voertuig::createVoertuig("auto");
    Voertuig* v1 = Voertuig::createVoertuig("auto");
    Baan* b = new Baan();
    b->setLengte(100);
    v->setBaan(b);
    v1->setBaan(b);
    v1->setPositie(0);
    v->setPositie(99);
    v->setSnelheid(20); // Zal einde baan bereiken in 1 stap
    v->setState(State::DRIVING);
    b->addVoertuig(v);
    b->addVoertuig(v1);

    EXPECT_EQ(int(b->getVoertuigen().size()), 2);

    delete b;
    delete v;
}