#include "Graph.h"
  
Graph::~Graph() {
  destroy();
}

Graph::Graph(const Graph& other) {
  copy(other);
}
/*
Graph& Graph::operator=(const Graph& rhs) {
  // only destroy and copy if not the same graph
  std::cout << "HELLO" << std::endl;
  if (this != &rhs) {
    destroy();
    std::cout << "here" << std::endl;
    copy(rhs);
  }
  return *this;
}*/

void Graph::copy(const Graph& other) {
  for (std::pair<int, VertexData*> vertex : other.vertexes_) {
   insertVertex(vertex.second->station_);
  }
  for (Edge* edge : other.edges_) {
    VertexData* vertex_one = vertexes_[edge->start_vertex_->station_.id_];
    VertexData* vertex_two = vertexes_[edge->end_vertex_->station_.id_];
    insertEdge(vertex_one, vertex_two);
  }
}

void Graph::destroy() {
  // delete each vertex's allocated adjacent edge list (not the edges)
  for (std::pair<int, VertexData*> element : vertexes_) {
   VertexData* to_delete = element.second;
   delete to_delete->adjacent_edges_;
   delete to_delete;
  }
  // delete each edge
  for (Edge* edge : edges_) {
    delete edge;
  } 
}

void Graph::insertVertex(Station station_to_add) {
  // don't allow repeated vertexes (or else there will be a memory leak)
  auto to_return_iter = vertexes_.find(station_to_add.id_);
  if (to_return_iter != vertexes_.end()) {
    return;
  }
  std::list<Edge*>* adjacent_edges_ = new std::list<Edge*>();
  vertexes_[station_to_add.id_] = new VertexData(station_to_add, adjacent_edges_);
}


Graph::VertexData* Graph::getVertex(int station_id) {
  auto to_return_iter = vertexes_.find(station_id);
  if (to_return_iter == vertexes_.end()) {
    return nullptr;
  } else {
    return to_return_iter->second;
  }
}

std::map<int, Graph::VertexData*> Graph::getVertexMap() const {
  return vertexes_;
}

std::list<Graph::Edge*> Graph::getEdgeList() const {
  return edges_;
}

void Graph::insertEdge(VertexData* vertex_one, VertexData* vertex_two) {
  // ensure no self-loops are formed
  if (vertex_one == vertex_two) {
    return;
  }
  Edge* new_edge = new Edge(vertex_one, vertex_two);
  // ensure no duplicate edges are created
  std::list<Edge*>* vertex_one_edges = vertex_one->adjacent_edges_;
  for (Edge* edge : *vertex_one_edges) {
    if (edge->start_vertex_ == vertex_one || edge->end_vertex_ == vertex_one) {
      if (edge->start_vertex_ == vertex_two || edge->end_vertex_ == vertex_two) {
        delete new_edge;
        return;
      }
    }
  }
  vertex_one->adjacent_edges_->push_back(new_edge);
  vertex_two->adjacent_edges_->push_back(new_edge);
  edges_.push_back(new_edge);
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
    Station station_one = Station(start_station_id, start_station_latitude, start_station_longitude);
    Station station_two = Station(end_station_id, end_station_latitude, end_station_longitude);

    // add station vertexes to the graph if they don't exist yet
    if (vertexes_.find(start_station_id) == vertexes_.end()) {
      insertVertex(station_one);
    }
    if (vertexes_.find(end_station_id) == vertexes_.end()) {
      insertVertex(station_two);
    }
    // add edge between the stations (overlap and self loop accounted for in insert edge)
    insertEdge(vertexes_[start_station_id], vertexes_[end_station_id]);
  }
}
