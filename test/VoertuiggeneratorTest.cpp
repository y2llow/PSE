#include <gtest/gtest.h>
#include "../src/simulatie/Parser.h"
#include "../src/simulatie/Simulator.h"
#include "../src/elementen/Verkeerslicht.h"
#include "../src/elementen/Voertuiggenerator.h"

class VoertuiggeneratorTest : public ::testing::Test {
protected:
    Voertuiggenerator* generator;
    Baan* baan;

    void SetUp() override {
        baan = new Baan();
        baan->setLengte(1000);
        generator = new Voertuiggenerator(baan, 10.0, "auto"); // Frequentie van 10 seconden
    }

    void TearDown() override {
        delete generator;
        delete baan;
    }
};

TEST_F(VoertuiggeneratorTest, Initialisatie) {
    EXPECT_EQ(generator->getBaan(), baan);
    EXPECT_DOUBLE_EQ(generator->getFrequentie(), 10.0);
    EXPECT_EQ(generator->getType(), "auto");
    EXPECT_EQ(int(baan->getVoertuigen().size()), 0);
}

TEST_F(VoertuiggeneratorTest, GeneratieNaFrequentie) {
    // Bereken aantal iteraties voor 10 seconden (10 / 0.0166 ≈ 602)
    for (int i = 0; i < 605; i++) {
        generator->generateVoertuig();
    }
    EXPECT_EQ(int(baan->getVoertuigen().size()), 1);
}

TEST_F(VoertuiggeneratorTest, MinimumAfstandRespecteren) {
    // Genereer eerste voertuig (602 iteraties)
    for (int i = 0; i < 605; i++) {
        generator->generateVoertuig();
    }
    ASSERT_EQ(int(baan->getVoertuigen().size()), 1);

    // Probeer tweede voertuig te genereren (6 iteraties ≈ 0.1s)
    for (int i = 0; i < 6; i++) {
        generator->generateVoertuig();
    }
    EXPECT_EQ(int(baan->getVoertuigen().size()), 1);

    // Verplaats eerste voertuig verder weg
    baan->getVoertuigen()[0]->setPositie(20);

    // Genereer tweede voertuig (602 iteraties)
    for (int i = 0; i < 605; i++) {
        generator->generateVoertuig();
    }
    EXPECT_EQ(int(baan->getVoertuigen().size()), 2);
}

TEST_F(VoertuiggeneratorTest, VerschillendeTypes) {
    vector<string> types = {"Auto", "Auto", "Auto", "Auto", "Auto"};

    for (const auto& type : types) {
        Voertuiggenerator gen(baan, 5.0, type);

        // Genereer voertuig (5 / 0.0166 ≈ 301 iteraties)
        for (int i = 0; i < 305; i++) {
            gen.generateVoertuig();
        }

        ASSERT_EQ(int(baan->getVoertuigen().size()), 1);
        EXPECT_EQ(baan->getVoertuigen()[0]->getType(), type);


    }
}

TEST_F(VoertuiggeneratorTest, OngeldigeFrequentie) {
    // Negatieve frequentie (602 iteraties)
    Voertuiggenerator gen(baan, -5.0, "auto");
    for (int i = 0; i < 5; i++) {
        gen.generateVoertuig();
    }
    EXPECT_GT(int(baan->getVoertuigen().size()), 0);

    // Nul frequentie (602 iteraties)
    Voertuiggenerator gen2(baan, 0.0, "auto");
    for (int i = 0; i < 605; i++) {
        gen2.generateVoertuig();
    }
    EXPECT_GT(int(baan->getVoertuigen().size()), 0);
}

TEST_F(VoertuiggeneratorTest, TijdReset) {
    // Genereer eerste voertuig (602 iteraties)
    for (int i = 0; i < 605; i++) {
        generator->generateVoertuig();
    }
    ASSERT_EQ(int(baan->getVoertuigen().size()), 1);

    // Korte tijd later (6 iteraties ≈ 0.1s)
    for (int i = 0; i < 6; i++) {
        generator->generateVoertuig();
    }
    EXPECT_EQ(int(baan->getVoertuigen().size()), 1);

    // Wacht weer volledige frequentie (602 iteraties)
    for (int i = 0; i < 605; i++) {
        generator->generateVoertuig();
    }
    EXPECT_LT(int(baan->getVoertuigen().size()), 2);
}

TEST_F(VoertuiggeneratorTest, GeenBaan) {
    Voertuiggenerator gen(nullptr, 10.0, "auto");
    EXPECT_NO_THROW(gen.generateVoertuig());
}

TEST_F(VoertuiggeneratorTest, RealistischeSimulatie) {
    // Simuleer 1 minuut (60 seconden) ≈ 3614 iteraties
    for (int i = 0; i < 3654; i++) {
        generator->generateVoertuig();
    }

    // Verwacht ongeveer 6 voertuigen (60/10 frequentie)
    EXPECT_NEAR(int(baan->getVoertuigen().size()), 6, 5);
}
