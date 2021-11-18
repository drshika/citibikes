#include "../project/catch/catch.hpp"

#include <iostream>

TEST_CASE("Test Test Case") {
    std::cout << "testing..." << std::endl;
    REQUIRE(1 == 1);
}
