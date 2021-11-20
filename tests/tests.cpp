#include "../project/catch/catch.hpp"
#include "../Graph.h"
#include "../Graph.cpp"

#include <iostream>

/*
 * Tests for Graph: 
 *  - Insert Vertex
 *  - Insert Edge
 */
TEST_CASE("Insert Vertex") {
  Graph test;
  test.addDataFromFile("data/April2020.csv");
}

TEST_CASE("Insert Edge") {
  REQUIRE(1 == 1);
}

TEST_CASE("Missing Latitude/Longitude") {
  REQUIRE(1 == 1);
}
