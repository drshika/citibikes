#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <list>

class Graph {

public:
  struct Edge;

  struct Station {
    int id_;
    double latitude_;
    double longitude_;
    // Default Constructor (for the compiler)
    Station() {
      id_ = -1;
      latitude_ = -1;
      longitude_ = -1;
    }
    Station(int id, double latitude, double longitude) {
      id_ = id;
      latitude_ = latitude;
      longitude_ = longitude;
    }
};

  struct VertexData {
    Station station_;
    std::list<Edge*>* adjacent_edges_;

    VertexData(Station station, std::list<Edge*>* adjacent_edges) {
      station_ = station;
      adjacent_edges_ = adjacent_edges;
    }
  };

  struct Edge {
    VertexData* start_vertex_;
    VertexData* end_vertex_;

    Edge(VertexData* start_vertex, VertexData* end_vertex) {
      start_vertex_ = start_vertex;
      end_vertex_ = end_vertex;
    }

    bool operator==(const Edge& rhs) const {
      // graph is not directed so order of endpoints does not matter
      return ((start_vertex_->station_.id_ == rhs.start_vertex_->station_.id_) && (end_vertex_->station_.id_ == rhs.end_vertex_->station_.id_)) 
          ||((start_vertex_->station_.id_ == rhs.end_vertex_->station_.id_) && (end_vertex_->station_.id_ == rhs.start_vertex_->station_.id_));
    }
  };


  Graph() {}

  ~Graph();

  Graph(const Graph& to_copy);

  //Graph& operator=(const Graph& rhs);

  void destroy();

  void copy(const Graph& to_copy);

  void insertVertex(Station station_to_add);

  void insertEdge(VertexData* vertex_one, VertexData* vertex_two);

  void addDataFromFile(std::string file_path);

  VertexData* getVertex(int station_id);

  std::map<int, VertexData*> getVertexMap() const;

  std::list<Edge*> getEdgeList() const;

private:
  std::map<int, VertexData*> vertexes_;
  std::list<Edge*> edges_;
};
