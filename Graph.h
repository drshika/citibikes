#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

class Graph {

public:
  struct Station {
    int station_id_;
    double latitude_;
    double longitude_;
  };

  struct Edge {
    int start_station_id_;
    int end_station_id_;
  };

  void insertVertex(Station station_to_add);

  void insertEdge(Station station_a, Station station_b);

  std::map<int, Station> getVertexMap() const;
  
  std::vector<Edge> getEdgeList() const;

  void addDataFromFile(std::string file_path);

private:
  std::map<int, Station> vertex_map_;
  std::vector<Edge> edge_list_;
};