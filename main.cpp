#include "Graph.h"
#include "Graph.cpp"
#include "DFS.h"
#include "DFS.cpp"
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  const std::vector<std::string> kDataFilePaths = {"data/April2020.csv", "data/April2021.csv", "data/August2020.csv", "data/December2020.csv",
      "data/February2020.csv", "data/February2021.csv", "data/January2020.csv", "data/January2021.csv", "data/July2020.csv", "data/June2020.csv",
      "data/March2020.csv", "data/March2021.csv", "data/May2020.csv", "data/November2020.csv", "data/October2020.csv", "data/September2020.csv"};

  Graph* graph = new Graph();
  std::cout << "creating graph" << std::endl;
  for (const std::string& kFilePath : kDataFilePaths) {
    std::cout << "adding data from " << kFilePath << std::endl;
    graph->addDataFromFile(kFilePath);
  }
  std::cout << "graph created successfully" << std::endl;
  
  std::cout << "result is eulerian: " << graph->isEulerian() << std::endl;
  std::cout << "result is connected" << graph->isConnected() << std::endl;
  return 0;
}