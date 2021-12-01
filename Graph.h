#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <vector>

/**
 * Class representing a Graph
 */
class Graph {

public:
  struct Edge;

  /**
   * Struct storing a Station's Data
   */
  struct Station {
    // int storing the unique id of the station
    int id_;
    // double storing the latitude of the station
    double latitude_;
    // double storing the longitude of the station
    double longitude_;
    // Default Constructor (for the compiler)
    Station() {
      id_ = -1;
      latitude_ = -1;
      longitude_ = -1;
    }
    /**
     * Station Constructor
     * 
     * @param id int storing the unique id of the station
     * @param latitiude double storing the latitude of the station
     * @param longitude double storing the longitude of the staiton
     */
    Station(int id, double latitude, double longitude) {
      id_ = id;
      latitude_ = latitude;
      longitude_ = longitude;
    }
};

  /**
   * Struct storing data for each Vertex in the Graph
   */
  struct VertexData {
    // Station storing the station that the Vertex Represents
    Station station_;
    // list of edges adjacent to the vertex
    std::list<Edge*>* adjacent_edges_;

    /**
     * Constructor
     *
     * @param station a Station representing the station that the vertex represents in the graph
     * @param adjacent_edges a pointer to a list storing pointers to the edges that are adjacent to the vertex
     */
    VertexData(Station station, std::list<Edge*>* adjacent_edges) {
      station_ = station;
      adjacent_edges_ = adjacent_edges;
    }
  };

  /**
   * Struct representing an Edge in the Graph
   */
  struct Edge {
    // Pointer storing the starting vertex of the edge
    VertexData* start_vertex_;
    // Pointer storing the ending vertex of the edge
    VertexData* end_vertex_;

    /**
     * Constructor
     *
     * @param start_vertex Pointer to the starting vertex of the edge
     * @param end_vertex Pointer to the ending vertex of the edge
     */
    Edge(VertexData* start_vertex, VertexData* end_vertex) {
      start_vertex_ = start_vertex;
      end_vertex_ = end_vertex;
    }

    /**
     * Overloaded Equality Operator for an Edge
     * Two edges are equal if they have the same verticies (order does not matter)
     * 
     * @param rhs a reference to the edge to compare this edge with
     * @return a bool that is true if the two edges are equal
     */
    bool operator==(const Edge& rhs) const {
      // graph is not directed so order of endpoints does not matter
      return ((start_vertex_->station_.id_ == rhs.start_vertex_->station_.id_) && (end_vertex_->station_.id_ == rhs.end_vertex_->station_.id_)) 
          ||((start_vertex_->station_.id_ == rhs.end_vertex_->station_.id_) && (end_vertex_->station_.id_ == rhs.start_vertex_->station_.id_));
    }
  };


  /**
   * Default Constuctor
   */
  Graph() {}

  /**
   * Destructor
   */
  ~Graph();

  /**
   * Copy Constructor
   *
   * @param to_copy a reference to a Graph to create a new Graph from
   */
  Graph(const Graph& to_copy);

  //Graph& operator=(const Graph& rhs);

  /**
   * Deletes all allocated memory
   */
  void destroy();

  /**
   * Copies all data from another graph into this graph
   *
   * @param to_copy a reference to a graph to copy data from
   */
  void copy(const Graph& to_copy);

  /**
   * Inserts a Vertex into the Graph
   * Accounts for repeated Verticies
   *
   * @param station_to_add a Station to add to the graph
   */
  void insertVertex(Station station_to_add);

  /**
   * Inserts an Edge into the Graph
   * Accounts for repeated edges and self-loops
   *
   * @param vertex_one a pointer to a vertex representing the starting vertex of the edge
   * @param vertex_two a pointer to a vertex representing the ending vertex of the edge
   */
  void insertEdge(VertexData* vertex_one, VertexData* vertex_two);

  /**
   * Adds data from the given file to the Graph
   *
   * @param file_path a string representing the path to the data file in relation to the .cpp file
   */
  void addDataFromFile(std::string file_path);

  /**
   * Retrives the vertex represetnting the station with the given station id
   *
   * @param station_id an int representing the id of the station to get the vertex of
   * @return a pointer to the vertex representing the station with the given id
   */
  VertexData* getVertex(int station_id);

  /**
   * Retrieves the vertex map of the graph
   * 
   * @return a map representing the vertex map of the graph (Key: station id, Value: Vertex of station with the given key's id)
   */
  std::map<int, VertexData*> getVertexMap() const;

  /**
   * Retrives the edge list of the graph
   *
   * @return a list of edges representing all of the edges in the graph
   */
  std::list<Edge*> getEdgeList() const;

private:
  /**
   * A map representing the verticies in the graph
   * Key: int representing a station id
   * Value: Vertex corresponding to the station with the given station id
   */
  std::map<int, VertexData*> vertexes_;

  /**
   * List of Pointers to all the edges in the graph
   */
  std::list<Edge*> edges_;
};