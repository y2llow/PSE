#include <fstream>
#include <gtest/gtest.h>

#include "../src/simulatie/Parser.h"
#include "../src/simulatie/Simulator.h"

#include "../src/elementen/Bushalte.h"
#include "../src/elementen/Baan.h"


class BushalteTest : public ::testing::Test {
protected:
    void SetUp() override {
        baan = new Baan();
        bushalte = new Bushalte();
    }

    void TearDown() override {
        delete baan;
        delete bushalte;
    }

    Baan* baan;
    Bushalte* bushalte;
};

TEST_F(BushalteTest, Initialization) {
    EXPECT_EQ(bushalte->getBaan(), nullptr);
    EXPECT_EQ(bushalte->getPositie(), 0.0);
    EXPECT_EQ(bushalte->getWachttijd(), 0.0);
    EXPECT_TRUE(bushalte->getBaan() == nullptr);
}

TEST_F(BushalteTest, SetProperties) {
    bushalte->setBaan(baan);
    bushalte->setPositie(50.0);
    bushalte->setWachttijd(10.0);

    EXPECT_EQ(bushalte->getBaan(), baan);
    EXPECT_EQ(bushalte->getPositie(), 50.0);
    EXPECT_EQ(bushalte->getWachttijd(), 10.0);
}

class BusStoppingorWaitingTest : public ::testing::Test {
protected:
    void SetUp() override {
        sim = new Simulator();
        baan = new Baan();
        baan->setLengte(1000);
        baan->setNaam("Baan1");
        bushalte = new Bushalte();
        bushalte->setBaan(baan);
        bushalte->setPositie(500.0);
        bushalte->setWachttijd(0.16599);

        bus->setBaan(baan);
        baan->addVoertuig(bus);
        baan->addBushalte(bushalte);
        sim->addBaan(baan);
    }

    void TearDown() override {
        delete baan;
        delete bushalte;
    }
    Simulator* sim;
    Baan* baan;
    Bushalte* bushalte;
    Voertuig* bus = Voertuig::createVoertuig("bus");
    Bus* BUS;
};

TEST_F(BusStoppingorWaitingTest, BusSlowsDownWhenApproaching) {
    bus->setPositie(490.0); // Within VERTRAAG_AFSTAND
    bus->setSnelheid(bus->getMaximaleSnelheid());

    bushalte->stopBus();

    EXPECT_EQ(bus->getState(), State::SLOWINGDOWN);
    EXPECT_LT(bus->getKvmax(), bus->getMaximaleSnelheid());
}

TEST_F(BusStoppingorWaitingTest, BusStopsAtBushalte) {
    bus->setPositie(499.9); // Very close to bus stop
    bus->setSnelheid(5.0);

    bushalte->stopBus();

    EXPECT_EQ(bus->getState(), State::STOPPING);
    EXPECT_LT(bus->getVersnelling(), 0);
}

TEST_F(BusStoppingorWaitingTest, BusWaitsCorrectTime) {
    bus->setPositie(499.9); // Very close to bus stop
    bus->setKvmax(16.6);
    double positie1 = bus->getPositie();
    // Simulate waiting for half the required time
    sim->simulate(5);

    EXPECT_EQ(bus->getState(), State::STOPPING);
    EXPECT_EQ(bus->getPositie(), positie1);
    double positie2 = bus->getPositie();

    sim->simulate(7);
    EXPECT_EQ(bus->getState(), State::DRIVING);
    EXPECT_GT(bus->getPositie(), positie2);
}

class MultipleBusesTest : public ::testing::Test {
protected:
    void SetUp() override {
        sim = new Simulator();
        baan = new Baan();
        baan->setLengte(1000);
        bushalte = new Bushalte();
        bushalte->setBaan(baan);
        baan->addBushalte(bushalte);
        bushalte->setPositie(500.0);
        bushalte->setWachttijd(0.01649);

        bus1->setBaan(baan);
        bus1->setPositie(400.0);
        bus1->setSnelheid(10.0);

        bus2->setBaan(baan);
        bus2->setPositie(300.0);
        bus2->setSnelheid(10.0);

        baan->addVoertuig(bus1);
        baan->addVoertuig(bus2);
        sim->addBaan(baan);
    }

    void TearDown() override {
        delete baan;
        delete bushalte;
        delete bus1;
        delete bus2;
        delete sim;
    }

    Baan* baan;
    Bushalte* bushalte;
    Voertuig* bus1 = Voertuig::createVoertuig("bus");
    Voertuig* bus2 = Voertuig::createVoertuig("bus");
    Simulator* sim;
};

TEST_F(MultipleBusesTest, OnlyOneBusStopsAtATime) {
    // First bus approaches
    sim->simulate(1);
    EXPECT_EQ(bus1->getState(), State::DRIVING);
    EXPECT_EQ(bus2->getState(), State::DRIVING);

    // First bus stops
    bus1->setPositie(499.6);
    sim->simulate(1);
    EXPECT_EQ(bus1->getState(), State::STOPPING);

    // Second bus should slow down while first is at bus stop
    bus2->setPositie(496.0);
    sim->simulate(1);
    EXPECT_LT(bus2->getVersnelling(), 0);
}

TEST_F(MultipleBusesTest, secondStopsToo) {
    // First bus approaches
    sim->simulate(1);
    EXPECT_EQ(bus1->getState(), State::DRIVING);
    EXPECT_EQ(bus2->getState(), State::DRIVING);

    // First bus stops
    bus1->setPositie(499.6);
    bus2->setPositie(490);
    double pos1 = bus1->getPositie(); double pos1_2 = bus2->getVersnelling();
    sim->simulate(1);
    double pos2 = bus1->getPositie(); double pos2_2 = bus2->getVersnelling();

    EXPECT_EQ(bus1->getState(), State::STOPPING);
    EXPECT_EQ(pos1,pos2);
    EXPECT_GT(pos1_2,pos2_2);
    sim->simulate(1);

    bus1->setPositie(550);
    bus2->setVersnelling(15);
    sim->simulate(76);
    EXPECT_EQ(bus1->getState(), State::DRIVING);
    EXPECT_EQ(bus2->getState(), State::STOPPING);
}