//
// Created by AbEms on 3/12/2025.
//

#include <gtest/gtest.h>

#include "../../../specificatie_1/1_1_verkeerssituatie_inlezen/VerkeerssituatieInlezen.h"

class EmptyState : public testing::Test {
    void SetUp() override {
        vi.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/empty_file.xml");
        b_size = vi.getBanen().size();
        v_size = vi.getVoertuigen().size();
        ver_size = vi.getVerkeerslichten().size();
        vtg_size = vi.getVoertuiggeneratoren().size();
    }
protected:
    VerkeerssituatieInlezen vi;
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
    VerkeerssituatieInlezen vi;
};

TEST_F(WrongDataType, GoodSyntaxWrongDataType) {
    EXPECT_THROW(vi.parseXMLAndCreateObjects("../test/specificatie1/1_1_verkeerssituatie_inlezen/wrong_data_types.xml"), invalid_argument);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}