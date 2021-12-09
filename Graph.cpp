#include "Graph.h"
#include "DFS.h"

bool Graph::VertexData::isAdjacentVertex(VertexData* other_vertex) const {
  for (Edge* edge : adjacent_edges_) {
    if ((edge->start_vertex_ == other_vertex) || (edge->end_vertex_ == other_vertex)) {
      return true;
    }
  }
  return false;
}

// WARNING: if given vertex is not an endpoint of the edge this function will return the 
// start vertex
Graph::VertexData* Graph::Edge::getOtherVertex(VertexData* vertex) const {
  if (start_vertex_ != vertex) {
    return start_vertex_;
  } else {
    return end_vertex_;
  }
}

Graph::~Graph() {
   // std::cout << "destroying" << std::endl;
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
  std::list<Edge*> adjacent_edges_;
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
    insertEdge(vertexes_[start_station_id], vertexes_[end_station_id]);
  }
}

bool Graph::isConnected() {
  // graph with no vertexes is not connected
  if (vertexes_.size() == 0) return false;

  // complete dfs traversal

  DFS dfs = DFS(this, vertexes_.begin()->second);
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
  for (it = vertexes_.begin(); it != vertexes_.end(); it++) {
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
/*
Graph Graph::Dijkstras(Graph::VertexData* starting_vertex) {
  boost::heap::fibonacci_heap<VertexData*, boost::heap::compare<compareVertex>> priority_queue;
  std::map<int, VertexData*> previous_verticies_map;
  for (std::pair<int, Graph::VertexData*> vertex : vertexes_) {
    previous_verticies_map[vertex.first] = nullptr;
    vertex.second->distance_ = std::numeric_limits<double>::infinity();
    const VertexData* copy = vertex.second;
    priority_queue.push(copy);
  }
  starting_vertex->distance_ = 0;
  
  Graph minimum_spanning_tree;

  while (priority_queue.top() != starting_vertex) {
    break;
  }

  return Graph();
}
*/

void Graph::removeVertex(Graph::VertexData* to_remove) {
  if (to_remove == nullptr) {
    return;
  }
  // delete all adjacent edges of the vertex
  while (!(to_remove->adjacent_edges_.empty())) {
    edges_.remove(to_remove->adjacent_edges_.front());
    Graph::VertexData* other_vertex = to_remove->adjacent_edges_.front()->getOtherVertex(to_remove);
    other_vertex->adjacent_edges_.remove(to_remove->adjacent_edges_.front());
    delete to_remove->adjacent_edges_.front();
    to_remove->adjacent_edges_.pop_front();
  }

  // delete the vertex
  vertexes_.erase(to_remove->station_.id_);
  delete to_remove;
}


size_t Graph::size() const {
  return vertexes_.size();
}
/*
void Graph::getHamiltonianCycle(Graph::VertexData* current_vertex, Graph* full_graph, Graph* hamiltonian, 
    Graph::VertexData* starting_vertex, std::vector<Graph*> hamiltonians) {
      std::cout << "here: " << current_vertex->station_.id_ << " size: " << hamiltonian->size() << std::endl;
  if (hamiltonian->size() == full_graph->size()) {
    std::cout << "required met: " << std::endl;
    if (current_vertex->isAdjacentVertex(starting_vertex)) {
      hamiltonian->insertEdge(current_vertex, starting_vertex);
      hamiltonians.push_back(hamiltonian);
      printGraph(hamiltonian);
    }
  }
  for (Edge* edge : current_vertex->adjacent_edges_) {
    VertexData* other_vertex = edge->getOtherVertex(current_vertex);
    std::cout << "edge: " << current_vertex->station_.id_ << "->" << other_vertex->station_.id_ << std::endl;
    if (other_vertex->label != Graph::Label::kVisited) {
      std::cout << "new edge" << std::endl;
      other_vertex->label = Graph::Label::kVisited;
      hamiltonian->insertVertex(other_vertex->station_);
      hamiltonian->insertEdge(current_vertex, other_vertex);
      getHamiltonianCycle(other_vertex, full_graph, hamiltonian, starting_vertex, hamiltonians);
      other_vertex->label = Graph::Label::kUnexplored;
      hamiltonian->removeVertex(other_vertex);
    }
  }
}
*/
void Graph::printGraph(Graph* g) {
  for (Edge* edge : g->getEdgeList()) {
    std::cout << "edge: " << edge->start_vertex_->station_.id_ << "->" << edge->end_vertex_->station_.id_ << std::endl;
  }
}
