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

  // Use DFS to calculate the number of stations and print them
  std::cout << "DFS traversal:" << std::endl;
  DFS dfs = DFS(graph, graph->getVertexMap().begin()->second);
  size_t num_stations = 0;
  for (auto it = dfs.begin(); it != dfs.end(); ++it) {
    std::cout << (*it)->station_.id_ << " ";
    num_stations += 1;
  }
  std::cout << std::endl;
  std::cout << "Number of Stations: " << num_stations << std::endl;

  // find the nortwest most and southeast most station
  Graph::VertexData* starting_vertex = graph->getNorthwestMost();
  std::cout << "northwest most station: " << starting_vertex->station_.id_ << std::endl;
  Graph::VertexData* ending_vertex = graph->getSoutheastMost();
  std::cout << "southeast most station: " << ending_vertex->station_.id_ << std::endl;

  // start Dijkstras from the nortwest most vertex
  std::pair<Graph, std::map<int, Graph::VertexData*>> dijkstras_result = graph->Dijkstras(starting_vertex);
  Graph dijkstras_dag = dijkstras_result.first;
  std::cout << "degrees latitude across new york: " << dijkstras_dag.getTotalDistance() << std::endl;

  // find shortest path accross NYC using Dijkstra's result
  std::map<int, Graph::VertexData*> previous_verticies_map = dijkstras_result.second;
  Graph::VertexData* previous = previous_verticies_map[ending_vertex->station_.id_];
  std::vector<int> stations_to_travel;
  stations_to_travel.insert(stations_to_travel.begin(), ending_vertex->station_.id_);
  while (previous != nullptr) {
    stations_to_travel.insert(stations_to_travel.begin(), previous->station_.id_);
    previous = previous_verticies_map[previous->station_.id_];
  }
 // stations_to_travel.insert(stations_to_travel.begin(), starting_vertex->station_.id_);

  // print out the shortest path across NYC produced by Dijkstra's
  std::cout << "shortest station path across NYC: " << std::endl;
  for (const int kStationId : stations_to_travel) {
    std::cout << kStationId << " ";
  }
  std::cout << std::endl;
  return 0;
}
