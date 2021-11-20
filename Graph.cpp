#include "Graph.h"

std::map<int, Graph::Station> Graph::getVertexMap() const {
  return vertex_map_;
}

std::vector<Graph::Edge> Graph::getEdgeList() const {
  return edge_list_;
}

void Graph::insertEdge(Station a, Station b) {
  Edge edge;
  edge.start_station_id_ = a.station_id_;
  edge.end_station_id_ = b.station_id_;
  // ensure no self loops formed
  if (a.station_id_ == b.station_id_) {
    return;
  }
  // ensure no duplicate edges are created 
  if (std::find(edge_list_.begin(), edge_list_.end(), edge) == edge_list_.end()) {
    edge_list_.push_back(edge);
  }
}

void Graph::insertVertex(Station station_to_add) {
  vertex_map_[station_to_add.station_id_] = station_to_add;
}

void Graph::addDataFromFile(std::string file_path) {
  std::string data;
  std::ifstream data_file(file_path);
  const std::regex separate_by_commas("[^,]*");
  bool is_first_line = true;

  while (std::getline(data_file, data)) {
    // skip first line of data (with headers)
    if (is_first_line) {
      is_first_line = false;
      continue;
    }

    // remove spaces from line (and data)
    auto without_space = std::remove(data.begin(), data.end(), ' ');
    data.erase(without_space, data.end());

    // separate data points by commas
    std::sregex_iterator tester = std::sregex_iterator(data.begin(), data.end(), separate_by_commas);
    std::sregex_iterator end = std::sregex_iterator();

    // ensure the data has the correct number of data points (2 * num rows data)
    if (std::distance(tester, end) != 30) {
      continue;
    }

    int data_counter = 0;
    // variables storing the locations int the iterator of the necessary data points
    const int kStartStationId = 6;
    const int kStartLatitude = 10;
    const int kStartLongitude = 12;
    const int kEndStationId = 14;
    const int kEndLatitude = 18;
    const int kEndLongitude = 20;
    // variables storing the necessary data points
    int start_station_id;
    double start_station_latitude;
    double start_station_longitude;
    int end_station_id;
    double end_station_latitude;
    double end_station_longitude;

    for (auto i = tester; i != end; ++i) {
      if (data_counter == kStartStationId) {
        std::string start_station_id_str = (*i).str();
        std::stringstream stringstream(start_station_id_str);
        stringstream >> start_station_id;

      } else if (data_counter == kStartLatitude) {
        std::string latitude = (*i).str();
        std::stringstream stringstream(latitude);
        stringstream >> start_station_latitude;

      } else if (data_counter == kStartLongitude) {
        std::string longitude = (*i).str();
        std::stringstream stringstream(longitude);
        stringstream >> start_station_longitude;

      } else if (data_counter == kEndStationId) {
        std::string end_station_id_str = (*i).str();
        std::stringstream stringstream(end_station_id_str);
        stringstream >> end_station_id;

      } else if (data_counter == kEndLatitude) {
        std::string end_latitude = (*i).str();
        std::stringstream stringstream(end_latitude);
        stringstream >> end_station_latitude;

      } else if (data_counter == kEndLongitude) {
        std::string end_longitude = (*i).str();
        std::stringstream stringstream(end_longitude);
        stringstream >> end_station_longitude;
      }
      data_counter += 1;
    }
    // create stations
    Station station_one;
    station_one.station_id_ = start_station_id;
    station_one.latitude_ = start_station_latitude;
    station_one.longitude_ = start_station_longitude;

    Station station_two;
    station_two.station_id_ = end_station_id;
    station_two.latitude_ = end_station_latitude;
    station_two.longitude_ = end_station_longitude;

    // add station vertexes to the graph if they don't exist yet
    if (vertex_map_.find(start_station_id) == vertex_map_.end()) {
      insertVertex(station_one);
    }
    if (vertex_map_.find(end_station_id) == vertex_map_.end()) {
      insertVertex(station_two);
    }
    // add edge between the stations (overlap and self loop accounted for in insert edge)
    insertEdge(station_one, station_two);
  }
}
