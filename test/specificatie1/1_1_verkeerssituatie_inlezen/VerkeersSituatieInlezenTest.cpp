//
// Created by AbEms on 3/12/2025.
//

#include <gtest/gtest.h>

#include "../../../specificatie_1/1_1_verkeerssituatie_inlezen/VerkeerssituatieInlezen.h"

class BeginState : public testing::Test {

protected:
    void SetUp() override {
        vi.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml");
        size = vi.get_banen().size();
    }

    VerkeersSituatieInlezen vi;
    int size;
};


TEST_F(BeginState, Exceptions) {
    ASSERT_EQ(size, 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}