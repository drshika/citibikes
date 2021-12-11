#include "DFS.h"
#include "Graph.h"

#include <cmath>
#include <limits.h>

bool Graph::VertexData::isAdjacentVertex(VertexData* other_vertex) const {
  for (Edge* edge : adjacent_edges_) {
    if ((edge->start_vertex_ == other_vertex) || (edge->end_vertex_ == other_vertex)) {
      return true;
    }
  }
  return false;
}

Graph::VertexData* Graph::Edge::getOtherVertex(VertexData* vertex) const {
  if (start_vertex_ != vertex) {
    return start_vertex_;
  } else {
    return end_vertex_;
  }
}

double Graph::Edge::getEdgeDistance() const {
  double latitude_difference = start_vertex_->station_.latitude_ - end_vertex_->station_.latitude_;
  double longitude_difference = start_vertex_->station_.longitude_ - end_vertex_->station_.longitude_;
  return std::sqrt((latitude_difference * latitude_difference) + (longitude_difference * longitude_difference));
}

Graph::~Graph() {
  destroy();
}

Graph::Graph(const Graph& other) {
  copy(other);
}

Graph& Graph::operator=(const Graph& rhs) {
  // only destroy and copy if not the same graph
  if (this != &rhs) {
    destroy();
    copy(rhs);
  }
  return *this;
}

void Graph::copy(const Graph& other) {
  for (std::pair<int, VertexData*> vertex : other.verticies_) {
   insertVertex(vertex.second->station_);
  }
  for (Edge* edge : other.edges_) {
    VertexData* vertex_one = verticies_[edge->start_vertex_->station_.id_];
    VertexData* vertex_two = verticies_[edge->end_vertex_->station_.id_];
    insertEdge(vertex_one, vertex_two);
  }
}

void Graph::destroy() {
  // delete each vertex's allocated adjacent edge list (not the edges)
  if (verticies_.empty()) {
    return;
  }
  for (std::pair<int, VertexData*> element : verticies_) {
   VertexData* to_delete = element.second;
   delete to_delete;
  }
  // delete each edge
  for (Edge* edge : edges_) {
    delete edge;
  } 
  if (largest_hamiltonian_ != nullptr) {
    delete largest_hamiltonian_;
  }
}

void Graph::insertVertex(Station station_to_add) {
  // don't allow repeated vertexes (or else there will be a memory leak)
  auto to_return_iter = verticies_.find(station_to_add.id_);
  if (to_return_iter != verticies_.end()) {
    return;
  }
  std::list<Edge*> adjacent_edges_;
  verticies_[station_to_add.id_] = new VertexData(station_to_add, adjacent_edges_);
}

Graph::VertexData* Graph::getVertex(int station_id) {
  auto to_return_iter = verticies_.find(station_id);
  if (to_return_iter == verticies_.end()) {
    return nullptr;
  } else {
    return to_return_iter->second;
  }
}

std::map<int, Graph::VertexData*> Graph::getVertexMap() const {
  return verticies_;
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
  std::list<Edge*> vertex_one_edges = vertex_one->adjacent_edges_;
  for (Edge* edge : vertex_one_edges) {
    if (edge->start_vertex_ == vertex_one || edge->end_vertex_ == vertex_one) {
      if (edge->start_vertex_ == vertex_two || edge->end_vertex_ == vertex_two) {
        delete new_edge;
        return;
      }
    }
  }
  vertex_one->adjacent_edges_.push_back(new_edge);
  vertex_two->adjacent_edges_.push_back(new_edge);
  edges_.push_back(new_edge);
  total_distance_ += new_edge->getEdgeDistance();
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

    // create vertexes (overlap accounted for in insert vertex)
    insertVertex(station_one);
    insertVertex(station_two);

    // add edge between the stations (overlap and self loop accounted for in insert edge)
    insertEdge(verticies_[start_station_id], verticies_[end_station_id]);
  }
}

bool Graph::isConnected() {
  // graph with no vertexes is not connected
  if (verticies_.size() == 0) return false;

  // complete dfs traversal
  DFS dfs = DFS(this, verticies_.begin()->second);
  for (auto it = dfs.begin(); it != dfs.end(); ++it) {
    // exit early if more than 1 connected components
    if (dfs.getNumConnectedComponents() != 1) return false;
  }

  return (dfs.getNumConnectedComponents() == 1);
}

int Graph::isEulerian() {
  // unconnected graph is not Eulerian
  if (!isConnected()) return 0;

  // count number of vertices with odd degree
  int odd_count = 0;
  std::map<int, VertexData*>::iterator it;
  for (it = verticies_.begin(); it != verticies_.end(); it++) {
    if (it->second->adjacent_edges_.size() % 2 == 1) {
      ++odd_count;
    }
  }

  // if odd count is more than 2, graph cannot be Eulerian
  if (odd_count > 2) return 0;

  // if odd count is 2, graph has Eulerian Path (is Semi-Eulerian)
  if (odd_count == 2) return 1;

  // odd count is 0, meaning graph has Eulerian Cycle
  // odd count cannot be 1 for undirected graph
  return 2;
}


Graph Graph::Dijkstras(Graph::VertexData* starting_vertex) {
  boost::heap::fibonacci_heap<VertexData*, boost::heap::compare<compareVertex>> priority_queue;

  std::map<int, VertexData*> previous_verticies_map;

  // Mark all verticies as unexplored, set all previous verticies to nullptr, 
  // and set all distances to +inf
  for (std::pair<int, Graph::VertexData*> vertex : verticies_) {
    vertex.second->label_ = Graph::Label::kUnexplored;
    previous_verticies_map[vertex.first] = nullptr;
    vertex.second->distance_ = std::numeric_limits<double>::infinity();
    vertex.second->handle = priority_queue.push(vertex.second);
  }

  Graph minimum_spanning_tree;
  // set starting vertex distance to 0 and update the priority queue
  starting_vertex->distance_ = 0;
  priority_queue.update(starting_vertex->handle, starting_vertex);

  while (priority_queue.empty() == false) {
    // insert current vertex into the MST and update the priority queue
    VertexData* current_vertex = priority_queue.top();
    priority_queue.pop();
    minimum_spanning_tree.insertVertex(current_vertex->station_);
    VertexData* created_vertex = minimum_spanning_tree.getVertex(current_vertex->station_.id_);
    // NOTE: this is done because previous vertex must correspond with the current graph (not MST graph)
    VertexData* previous_vertex = previous_verticies_map[current_vertex->station_.id_];

    if (previous_vertex != nullptr) {
      minimum_spanning_tree.insertEdge(created_vertex, previous_vertex);
    }

    for (Edge* edge : current_vertex->adjacent_edges_) {
      VertexData* other_vertex = edge->getOtherVertex(current_vertex);
      if (other_vertex->label_ == Graph::Label::kVisited) {
        continue;
      }
      // update vertex if this distance is smaller (but not if it is equal)
      if (edge->getEdgeDistance() + current_vertex->distance_ < other_vertex->distance_) {
        other_vertex->distance_ = edge->getEdgeDistance() + current_vertex->distance_;
        previous_verticies_map[other_vertex->station_.id_] = created_vertex;
        priority_queue.update(other_vertex->handle, other_vertex);
      }
    }
    current_vertex->label_ = Graph::Label::kVisited;
  }
  return minimum_spanning_tree;
}

void Graph::removeVertex(Graph::VertexData* to_remove) {
  if (to_remove == nullptr) {
    return;
  }
  // delete all adjacent edges of the vertex
  while (!(to_remove->adjacent_edges_.empty())) {
    total_distance_ -= to_remove->adjacent_edges_.front()->getEdgeDistance();
    edges_.remove(to_remove->adjacent_edges_.front());
    Graph::VertexData* other_vertex = to_remove->adjacent_edges_.front()->getOtherVertex(to_remove);
    other_vertex->adjacent_edges_.remove(to_remove->adjacent_edges_.front());
    delete to_remove->adjacent_edges_.front();
    to_remove->adjacent_edges_.pop_front();
  }
  // delete the vertex
  verticies_.erase(to_remove->station_.id_);
  delete to_remove;
}

size_t Graph::size() const {
  return verticies_.size();
}

Graph* Graph::getLargestHamiltonianCycle() {
  // label all verticies as unexplored
  for (std::pair<int, Graph::VertexData*> vertex : verticies_) {
    vertex.second->label_ = Graph::Label::kUnexplored;
  }
  
  Graph::VertexData* start = getVertex(verticies_.begin()->second->station_.id_);
  start->label_ = Graph::Label::kVisited;

  Graph* hamiltonian = new Graph();
  hamiltonian->insertVertex(start->station_);
  Graph::VertexData* hamiltonian_start = hamiltonian->getVertex(verticies_.begin()->second->station_.id_);

  getHamiltonianCycle(hamiltonian_start, hamiltonian, start, start, hamiltonian_start);

  delete hamiltonian;
  return largest_hamiltonian_;
}

void Graph::updateLargestHamiltonain(Graph* to_check) {
  if (largest_hamiltonian_ == nullptr) {
    largest_hamiltonian_ = new Graph(*to_check);

  } else if (to_check->getTotalDistance() > largest_hamiltonian_->getTotalDistance()) {
    delete largest_hamiltonian_;
    largest_hamiltonian_ = nullptr;
    largest_hamiltonian_ = new Graph(*to_check);
  } 
}

void Graph::getHamiltonianCycle(Graph::VertexData* current_hamiltonian_vertex, Graph* hamiltonian, 
    Graph::VertexData* current_graph_vertex, Graph::VertexData* start_vertex, 
    Graph::VertexData* hamiltonian_start) {
  if (hamiltonian->size() == size()) {
    // Only a hamiltonian Cycle if last vertex is adjacent to the start vertex
    if (current_graph_vertex->isAdjacentVertex(start_vertex)) {
      hamiltonian->insertEdge(hamiltonian_start, current_hamiltonian_vertex);
      updateLargestHamiltonain(hamiltonian);
    }
  }
  // backtrace
  for (Edge* edge : current_graph_vertex->adjacent_edges_) {
    VertexData* other_vertex = edge->getOtherVertex(current_graph_vertex);
    if (other_vertex->label_ != Graph::Label::kVisited) {
      other_vertex->label_ = Graph::Label::kVisited;
      hamiltonian->insertVertex(other_vertex->station_);
      VertexData* hamiltonian_other_vertex = hamiltonian->getVertex(other_vertex->station_.id_);
      hamiltonian->insertEdge(current_hamiltonian_vertex, hamiltonian_other_vertex);
      getHamiltonianCycle(hamiltonian_other_vertex, hamiltonian, other_vertex, start_vertex,
        hamiltonian_start);
      other_vertex->label_ = Graph::Label::kUnexplored;
      hamiltonian->removeVertex(hamiltonian_other_vertex);
    }
  }
}

Graph::VertexData* Graph::getNorthwestMost() {
  std::map<int, VertexData*>::iterator it;
  int latitude = -1;
  int longitude = INT_MAX;
  Graph::VertexData* to_return;

  for (it = verticies_.begin(); it != verticies_.end(); it++) { //
    Graph::Station * station = &it->second->station_;
    //northwest most means that the latitude is the greatest and the longitude is the smallest
    if (station->longitude_ <= longitude && station->latitude_ >= latitude) {
      latitude = station->latitude_;
      longitude = station->longitude_;
      to_return = it->second;
    }
  }
  return to_return;
}

Graph::VertexData* Graph::getSoutheastMost() {
  std::map<int, VertexData*>::iterator it;
  int latitude = INT_MAX;
  int longitude = -1;

  Graph::VertexData* to_return;
  //southeast most means that the latitude is the smallest and the longitude is the greatest     
  for (it = verticies_.begin(); it != verticies_.end(); it++) {
    Graph::Station* station = &it->second->station_;
    if (station->latitude_ <= latitude && station->longitude_ >= longitude) {
      latitude = station->latitude_;
      longitude = station->longitude_;
      to_return = it->second;
    }
  }
  return to_return;
}

double Graph::getTotalDistance() const {
  return total_distance_;
}
