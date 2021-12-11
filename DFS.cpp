#include "DFS.h"

DFS::DFS(Graph* graph, Graph::VertexData* first_vertex) {
  vertex_map_ = graph->getVertexMap();
  graph_ = graph;

  // set all verticies as unexplored
  for (std::pair<int, Graph::VertexData*> vertex : vertex_map_) {
    vertex.second->label_ = Graph::kUnexplored;
  }

  std::list<Graph::Edge*> edges = graph_->getEdgeList();
  // set all edges as unexplored
  for (Graph::Edge* edge : edges) {
    edge->label_ = Graph::kUnexplored;
  }
  // mark the starting vertex as visited and add it to the stack
  first_vertex->label_ = Graph::kVisited;
  stack_.push(first_vertex);
  index_in_map_ = 1;
  num_connected_components = 1;
}

DFS::Iterator::Iterator(DFS* graph_dfs) {
  dfs_ = graph_dfs;
}

Graph::VertexData* DFS::Iterator::operator*() {
  return dfs_->peek();
}

DFS::Iterator& DFS::Iterator::operator++() {
  Graph::VertexData* current_vertex = dfs_->peek();
  dfs_->pop();

  int current_station_id = current_vertex->station_.id_;
  for (Graph::Edge* edge : current_vertex->adjacent_edges_) {
    Graph::VertexData* other_vertex = nullptr;
    if (edge->start_vertex_->station_.id_ != current_station_id) {
      other_vertex = edge->start_vertex_;
      //since self loops are impossible in this implementation this is ok
    } else {
      other_vertex = edge->end_vertex_;
    }
    if (other_vertex->label_ == Graph::kUnexplored) {
      other_vertex->label_ = Graph::kVisited;
      edge->label_ = Graph::kDiscovery;
      dfs_->add(other_vertex);

    } else if (edge->label_ == Graph::kUnexplored) {
      edge->label_ = Graph::kBack;
    }
  }
  return *this;
}

bool DFS::empty() {
  if (stack_.empty()) {
    checkVerticiesAllExplored();
  }
  return stack_.empty();
}

void DFS::checkVerticiesAllExplored() {
  if (index_in_map_ == vertex_map_.size()) {
    return;
  }
 for (auto it = vertex_map_.begin(); it != vertex_map_.end(); ++it) {
    if ((*it).second->label_ == Graph::kUnexplored) {
      (*it).second->label_ = Graph::kVisited;
       stack_.push((*it).second);
       index_in_map_ += 1;
       num_connected_components += 1;
       break;
    } 
    index_in_map_ += 1;
  }
}

bool DFS::Iterator::operator!=(const DFS::Iterator &other) {
  bool is_this_null = false;
  bool is_other_null = false;

  if (dfs_ == NULL) {
    is_this_null = true;
  } 

  if (other.dfs_ == NULL) {
    is_other_null= true;
  }
  
  if (!is_this_null) {
    is_this_null = dfs_->empty();
  }
  if (!is_other_null) {
    is_other_null = other.dfs_->empty();
  }

  if (is_this_null && is_other_null) {
    return false;
  } else if (!is_this_null && !is_other_null) {
    return dfs_ != other.dfs_;
  } else {
    return true;
  }
}

DFS::Iterator DFS::begin() {
  return DFS::Iterator(this);
}

DFS::Iterator DFS::end() {
  return DFS::Iterator();
}

void DFS::add(Graph::VertexData* vertex) {
  stack_.push(vertex);
}

Graph::VertexData* DFS::peek() const {
  return stack_.top();
}

void DFS::pop() {
  stack_.pop();
}

size_t DFS::getNumConnectedComponents() {
  return num_connected_components;
}
