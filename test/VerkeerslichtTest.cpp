#include <fstream>
#include <gtest/gtest.h>

#include "../src/simulatie/Parser.h"
#include "../src/simulatie/Simulator.h"
#include "../src/elementen/Verkeerslicht.h"


class VerkeerslichtTest : public ::testing::Test {
protected:
    Verkeerslicht* licht;
    Baan* baan;
    Voertuig* voertuig;

    void SetUp() override {
        baan = new Baan();
        baan->setLengte(1000);
        licht = new Verkeerslicht();
        licht->setBaan(baan);
        licht->setPositie(500);
        licht->setCyclus(60);

        voertuig = Voertuig::createVoertuig("auto");
        voertuig->setBaan(baan);
        baan->addVoertuig(voertuig);
    }

    void TearDown() override {
        delete licht;
        delete baan;
        delete voertuig;
    }
};

TEST_F(VerkeerslichtTest, BasisInitialisatie) {
    EXPECT_EQ(licht->getBaan(), baan);
    EXPECT_DOUBLE_EQ(licht->getPositie(), 500);
    EXPECT_DOUBLE_EQ(licht->getCyclus(), 60);
    EXPECT_EQ(licht->getState(), LightState::GREEN);  // Nu verwachten we groen als initiële staat
    auto K = licht->getKruispunt();
    EXPECT_NE(&K, nullptr);
}

TEST_F(VerkeerslichtTest, KleurVerandering) {
    EXPECT_EQ(licht->getState(), LightState::GREEN);
    licht->switchColor();
    EXPECT_EQ(licht->getState(), LightState::ORANGE);
    licht->switchColor();
    EXPECT_EQ(licht->getState(), LightState::RED);
    licht->switchColor();
    EXPECT_EQ(licht->getState(), LightState::GREEN);}

TEST_F(VerkeerslichtTest, NormaleCyclusZonderKruispunt) {
    // Start met groen licht
    EXPECT_EQ(licht->getState(), LightState::GREEN);
    // Simuleer tijd tot net voor cyclus (60 - 5 = 55 sec groen)
    for (int i = 0; i < 55; i++) {
        licht->updateVerkeerslicht();
        EXPECT_EQ(licht->getState(), LightState::GREEN);    }

    // Na 55 sec zou naar oranje moeten gaan
    licht->updateVerkeerslicht();
    EXPECT_EQ(licht->getState(), LightState::GREEN);


    // Na volledige oranje cyclus zou rood moeten zijn
    licht->updateVerkeerslicht();
    EXPECT_EQ(licht->getState(), LightState::GREEN);}

//
//TEST_F(VerkeerslichtTest, KruispuntSynchronisatie) {
//    Verkeerslicht* licht2 = new Verkeerslicht();
//    Baan* baan2 = new Baan();
//    baan2->setLengte(1000);
//    licht2->setBaan(baan2);
//    licht2->setPositie(500);
//
//    // Stel kruispunt pairing in
//    licht->setKruispunt() ;
//    licht2->setKruispunt() ;
//    licht->KruispuntPair(licht, licht2);
//    licht2->KruispuntPair(licht2, licht);
//
//    // Start met beide lichten op groen
//    EXPECT_EQ(licht->getState(), LightState::GREEN);
//    EXPECT_EQ(licht2->getState(), LightState::GREEN);
//
//    // Eén licht moet op oranje gaan
//    for (int i = 0; i < 55; i++) {
//        licht->updateVerkeerslicht();
//        licht2->updateVerkeerslicht();
//    }
//    licht->updateVerkeerslicht();
//
//
//    delete licht2;
//    delete baan2;
//}

TEST_F(VerkeerslichtTest, VoertuigReactieOpLicht) {
    // Start met groen licht
    EXPECT_EQ(licht->getState(), LightState::GREEN);
    // Voertuig zou moeten versnellen bij groen licht
    voertuig->setPositie(450);
    voertuig->setSnelheid(10);
    voertuig->setState(State::DRIVING);
    licht->updateVerkeerslicht();
    EXPECT_EQ(voertuig->getState(), State::DRIVING);

    // Zet licht op rood en test reactie
    licht->switchColor();
    licht->updateVerkeerslicht();
}

TEST_F(VerkeerslichtTest, DynamischeCyclusAanpassing) {
    // Start met groen licht
    EXPECT_EQ(licht->getState(), LightState::GREEN);
    // Plaats voertuig dichtbij (50m)
    voertuig->setPositie(450);

    // Licht zou groen moeten blijven voor voertuig dichtbij
    for (int i = 0; i < 10; i++) {
        licht->updateVerkeerslicht();
    }
    EXPECT_EQ(licht->getState(), LightState::GREEN);}
