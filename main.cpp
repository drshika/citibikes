#include "Graph.h"
#include "Graph.cpp"
#include "DFS.h"
#include "DFS.cpp"

#include <cmath>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  // vector containing paths to the data files
  const std::vector<std::string> kDataFilePaths = {"data/April2020.csv", "data/April2021.csv", "data/August2020.csv", "data/December2020.csv",
      "data/February2020.csv", "data/February2021.csv", "data/January2020.csv", "data/January2021.csv", "data/July2020.csv", "data/June2020.csv",
      "data/March2020.csv", "data/March2021.csv", "data/May2020.csv", "data/November2020.csv", "data/October2020.csv", "data/September2020.csv"};

  Graph* graph = new Graph();
  // read data into the graph
  std::cout << "creating graph" << std::endl;
  for (const std::string& kFilePath : kDataFilePaths) {
    std::cout << "adding data from " << kFilePath << std::endl;
    graph->addDataFromFile(kFilePath);
  }
  std::cout << "graph created successfully" << std::endl;

  std::cout << "\ngraph results" << std::endl;
  // check if the graph is connected
  std::cout << "graph is connected: " << graph->isConnected() << std::endl;
  std::cout << "graph is eulerian: " << graph->isEulerian() << std::endl;

  Graph::VertexData* starting_vertex = graph->getNorthwestMost();
  std::cout << "northwest most station: " << starting_vertex->station_.id_ << std::endl;
  std::cout << "southeast most station: " << graph->getSoutheastMost()->station_.id_ << std::endl;

  // start Dijkstras from the nortwest most vertex
  Graph dijkstras_dag = graph->Dijkstras(starting_vertex);
  std::cout << "degrees latitude across new york: " << dijkstras_dag.getTotalDistance() << std::endl;

  return 0;
}
