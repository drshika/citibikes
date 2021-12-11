#pragma once

#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <vector>

#include <boost/heap/fibonacci_heap.hpp>

/**
 * Class representing a Graph
 */
class Graph {

public:
  /**
    * This enumeration stores labels for edges and verticies in the traversal
    * 
    * kVisited: The Vertex Has been Visited
    * kUnexplored: The edge or vertex is unexplored
    * kDiscovery: The edge is a discovery edge
    * kBack: The edge is a back edge
    */
  enum Label {kVisited, kUnexplored, kDiscovery, kBack};

  struct Edge;
  struct VertexData;

  /**
   * This struct provides the comparison for the boost fibonacci heap
   */
  struct compareVertex {
    bool operator() (VertexData* vertex_one, VertexData* vertex_two) const {
      return vertex_one->distance_ >= vertex_two->distance_;
    }
  };

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
    std::list<Edge*> adjacent_edges_;

    // Optional label for use in Graph Traversals (default is Unexplored)
    Label label_ = kUnexplored;

    // Optional double for use in Dijkstra's to track distance
    double distance_ = std::numeric_limits<double>::infinity();

    /**
     * Constructor
     *
     * @param station a Station representing the station that the vertex represents in the graph
     * @param adjacent_edges a pointer to a list storing pointers to the edges that are adjacent to the vertex
     */
    VertexData(Station station, std::list<Edge*> adjacent_edges) {
      station_ = station;
      adjacent_edges_ = adjacent_edges;
    }

    /**
     * Checks if the given Vertex is Adjacent to this Vertex
     *
     * @param other_vertex VertexData* of the Vertex to check
     * @return bool that is true if the given vertex is adjacent to this vertex
     */
    bool isAdjacentVertex(VertexData* other_vertex) const;

    // Allows for easy updates to the boost fibonacci heap
    boost::heap::fibonacci_heap<VertexData*, boost::heap::compare<compareVertex>>::handle_type handle;
  };

  /**
   * Struct representing an Edge in the Graph
   */
  struct Edge {
    // Pointer storing the starting vertex of the edge
    VertexData* start_vertex_;
    // Pointer storing the ending vertex of the edge
    VertexData* end_vertex_;

    // Optional label for use in Graph Traversals (default is Unexplored)
    Label label_ = kUnexplored;

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

    /**
     * Gets the Vertex at the other end of the edge from the given vertex
     *
     * WARNING: If the given vertex is not an endpoint of the edge, this 
     *    function will return the start_vertex_ value
     *
     * @param VertexData* of the vertex of the edge to not be returned
     * @return the vertex at the other end of the edge from the given vertex
     */
    VertexData* getOtherVertex(VertexData* vertex) const;

    /**
     * Gets the distance between the verticies at the end of each edge
     *
     * @return the distance between the verticies at the end of each edge
     */
    double getEdgeDistance() const;
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

  Graph& operator=(const Graph& rhs);

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
   * Accounts for repeated verticies
   *
   * @param station_to_add a Station to add to the graph
   */
  void insertVertex(Station station_to_add);

  /**
   * Inserts an Edge into the Graph
   * Accounts for self-loops
   * NOTE: does not account for duplicate edges
   *
   * @param vertex_one a pointer to a vertex representing the starting vertex of the edge
   * @param vertex_two a pointer to a vertex representing the ending vertex of the edge
   */
  void insertEdge(VertexData* vertex_one, VertexData* vertex_two);

  /**
   * Inserts an Edge into the Graph
   * Accounts for repeated edges and self-loops (NOTE: this is time expensive)
   * 
   * @param vertex_one a pointer to a vertex representing the starting vertex of the edge
   * @param vertex_two a pointer to a vertex representing the ending vertex of the edge
   */
  void insertEdgeFromData(VertexData* vertex_one, VertexData* vertex_two);

  /**
   * Removes the given vertex from the graph
   *
   * @param to_remove a vertex to remove from the graph
   */
  void removeVertex(VertexData* to_remove);

  /**
   * Adds data from the given file to the Graph
   *
   * @param file_path a string representing the path to the data file in relation to the .cpp file
   */
  void addDataFromFile(std::string file_path);

  /**
   * Retrives the vertex representing the station with the given station id
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

  /**
   * Retrives the combined distance of all edges in the graph
   *
   * @return the combinded distance of all edges in the graph
   */
  double getTotalDistance() const;

  /**
   * Determines if graph is connected
   *
   * @return true if the graph is connected, false otherwise
   */
  bool isConnected();

  /**
   * Determines if the graph is not Eulerian, or if the graph has a Eulerian path or cycle
   *
   * @return 0 if not Eulerian, 1 if has a Eulerian path, 2 if has a Eulerian cycle
   */
  int isEulerian(); 

  /**
   * Finds the minimum spanning tree of the Graph using Dijkstras Algorithm
   *
   * @param starting_vertex a pointer to the vertex to start Dijkstra's from
   * @return a Graph representing the MST produced by running Dijkstras from the 
   *    given vertex
   */
  std::pair<Graph, std::map<int, VertexData*>> Dijkstras(VertexData* starting_vertex);

  /**
   * Find the largest (most distance covered) hamiltonian cycle in the graph
   *
   * @return a Graph representing the largest Hamiltonian Cycle in the graph
   */
  Graph* getLargestHamiltonianCycle();

  /**
   * Updates the Largest Hamiltonian Cycle if the given graph is larger (covers
   * more distance)
   *
   * @param to_check Graph* to check if it is larger than the current largest
   *    Hamiltonian Cycle
   */
  void updateLargestHamiltonain(Graph* to_check);
  
  /**
   * Helper Function for Largest Hamiltonian Cycle 
   * 
   * @param current_hamiltonian_vertex a VertexData* representing the current vertex in
   *    the hamiltonian cycle
   * @param hamiltonian a Graph storing the hamiltonian cycle
   * @param current_graph_vertex a VertexData* representing the current vertex in the graph
   * @param start_vertex a VertexData* representing the starting vertex in the graph
   * @param hamiltonian_start a VertexData* representing the starting vertex in the 
   *    hamiltonian cycle
   */
  void getHamiltonianCycle(Graph::VertexData* current_hamiltonian_vertex, Graph* hamiltonian, 
      Graph::VertexData* current_graph_vertex, Graph::VertexData* start_vertex,
      Graph::VertexData* hamiltonian_start);

  /**
   * Retrieves the number of verticies in the graph
   *
   * @return a size_t representing the number of verticies in the graph
   */
  size_t size() const;

  /**
   * Helper to return the northwest most station on the map
   * 
   * @return a vertex largest latitude and smallest longitude
   */
  VertexData* getNorthwestMost();

  /**
   * Helper to return the southeast most station on the map
   * 
   * @return a vertex with the smallest latitude and largest longitude
   */
  VertexData* getSoutheastMost();


private:
  /**
   * A map representing the verticies in the graph
   * Key: int representing a station id
   * Value: Vertex corresponding to the station with the given station id
   */
  std::map<int, VertexData*> verticies_;

  /**
   * List of Pointers to all the edges in the graph
   */
  std::list<Edge*> edges_;

  // Graph to store the largest hamiltonian cycle in the graph
  Graph* largest_hamiltonian_ = nullptr;

  // variable to keep track of the total distance (weight of all graph edges combined)
  double total_distance_ = 0;
};
