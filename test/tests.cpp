#include <gtest/gtest.h>

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
