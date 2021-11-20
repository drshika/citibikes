#include "Graph.h"

void Graph::insertEdge(Station a, Station b) {
  Edge edge;
  edge.start_station_id_ = a.station_id_;
  edge.end_station_id_ = b.station_id_;
  edge_list_.push_back(edge);
}

void Graph::insertVertex(Station station_to_add) {
  vertex_map_[station_to_add.station_id_] = station_to_add;
}

void Graph::addDataFromFile(std::string file_path) {
  std::string data;
  std::ifstream data_file(file_path);
  const std::regex separate_by_commas("[^,]*");
  while (std::getline(data_file, data)) {
    // remove spaces from line (and data)
    auto without_space = std::remove(data.begin(), data.end(), ' ');
    data.erase(without_space, data.end());
    std::cout << data << std::endl;

    // separate data points by commas
    std::sregex_iterator tester = std::sregex_iterator(data.begin(), data.end(), separate_by_commas);
    std::sregex_iterator end = std::sregex_iterator();

    // ensure the data has the correct number of data points
    if (std::distance(tester, end) != 30) {
      continue;
    }

    // find the relevant data points
    int counter = 0;
    const int kStartStationId = 6;
    //int start_station_id;
    for (auto i = tester; i != end; ++i) {
      if (counter == kStartStationId) {
        std::string test = (*i).str();
        std::cout << "start: " << test << std::endl;
      } 
      counter += 1;
    }

  }
}
