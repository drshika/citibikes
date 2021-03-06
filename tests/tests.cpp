#include "../project/catch/catch.hpp"
#include "../DFS.h"
#include "../DFS.cpp"
#include "../Graph.h"
#include "../Graph.cpp"


bool areStationsEqual(Graph::Station station_one, Graph::Station station_two) {
  return (station_one.id_ == station_two.id_) && (station_one.latitude_ == station_two.latitude_) 
      && (station_one.longitude_ == station_two.longitude_);
}

/**
 * Test Edge Equality Operator
 */
TEST_CASE("Two Different Edges", "[edgeEquality]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);
  Graph::Station test_station_c = Graph::Station(3, 0, 0);

  std::list<Graph::Edge*> empty_list;

  Graph::VertexData* vertex_a_data = new Graph::VertexData(test_station_a, empty_list);
  Graph::VertexData* vertex_b_data = new Graph::VertexData(test_station_b, empty_list);
  Graph::VertexData* vertex_c_data = new Graph::VertexData(test_station_c, empty_list);

  Graph::Edge* edge_one = new Graph::Edge(vertex_a_data, vertex_b_data);
  Graph::Edge* edge_two = new Graph::Edge(vertex_a_data, vertex_c_data);

  REQUIRE(!(*edge_one == *edge_two));

  // delete allocated memory for valgrind testing
  delete vertex_a_data;
  delete vertex_b_data;
  delete vertex_c_data;
  delete edge_one;
  delete edge_two;
}

TEST_CASE("Edges with Same Start and Endpoints", "[edgeEquality]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);

  std::list<Graph::Edge*> empty_list;

  Graph::VertexData* vertex_a_data = new Graph::VertexData(test_station_a, empty_list);
  Graph::VertexData* vertex_b_data = new Graph::VertexData(test_station_b, empty_list);

  Graph::Edge* edge_one = new Graph::Edge(vertex_a_data, vertex_b_data);
  Graph::Edge* edge_two = new Graph::Edge(vertex_a_data, vertex_b_data);

  REQUIRE((*edge_one == *edge_two));

  // delete allocated memory for valgrind testing
  delete vertex_a_data;
  delete vertex_b_data;
  delete edge_one;
  delete edge_two;
}

TEST_CASE ("Edges with Same Endpoints in Different Order", "[edgeEquality]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);

  std::list<Graph::Edge*> empty_list;

  Graph::VertexData* vertex_a_data = new Graph::VertexData(test_station_a, empty_list);
  Graph::VertexData* vertex_b_data = new Graph::VertexData(test_station_b, empty_list);

  Graph::Edge* edge_one = new Graph::Edge(vertex_a_data, vertex_b_data);
  Graph::Edge* edge_two = new Graph::Edge(vertex_b_data, vertex_a_data);

  REQUIRE((*edge_one == *edge_two));

  // delete allocated memory for valgrind testing
  delete vertex_a_data;
  delete vertex_b_data;
  delete edge_one;
  delete edge_two;
}

/**
 * Tests for the Big 3
 */
TEST_CASE("Destructor", "[valgrind][big3]") {
  // create graph to copy
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);
  Graph::Station test_station_c = Graph::Station(2, 0, 0);

  Graph* test_graph = new Graph();
  test_graph->insertVertex(test_station_a);
  test_graph->insertVertex(test_station_b);
  test_graph->insertVertex(test_station_c);

  Graph::VertexData* vertex_a = test_graph->getVertex(0);
  Graph::VertexData* vertex_b = test_graph->getVertex(1);
  Graph::VertexData* vertex_c = test_graph->getVertex(2);

  test_graph->insertEdge(vertex_a, vertex_b);
  test_graph->insertEdge(vertex_a, vertex_c);
  test_graph->insertEdge(vertex_b, vertex_c);

  delete test_graph;
  SUCCEED("Destructor Didn't Crash");
}

TEST_CASE("Copy Constructor", "[valgrind][big3]") {
  // create graph to copy
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);

  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_b);

  Graph::VertexData* vertex_a = test_graph.getVertex(0);
  Graph::VertexData* vertex_b = test_graph.getVertex(1);

  test_graph.insertEdge(vertex_a, vertex_b);

  Graph* copied_graph = new Graph(test_graph);
  // check all vertexes and edges were copied over
  REQUIRE(copied_graph->getVertexMap().size() == 2);
  REQUIRE(copied_graph->getEdgeList().size() == 1);

  // check stations copied correctly
  REQUIRE(areStationsEqual(test_station_a, copied_graph->getVertex(0)->station_));
  REQUIRE(areStationsEqual(test_station_b, copied_graph->getVertex(1)->station_));

  // check edge copied correctly
  Graph::Edge* expected_edge = new Graph::Edge(vertex_a, vertex_b);
  Graph::Edge* resulting_edge = copied_graph->getEdgeList().front();
  REQUIRE((*resulting_edge == *expected_edge));

  // check modifying copied graph doesn't modify other graph
  copied_graph->insertVertex(Graph::Station(2, 0, 0));
  REQUIRE(copied_graph->getVertexMap().size() == 3);
  REQUIRE(test_graph.getVertexMap().size() == 2);

  // delete allocated memeory for valgrind
  delete copied_graph;
  delete expected_edge;
}

/*
 * Tests for Graph Implementation: 
 *  - Insert Vertex
 *  - Insert Edge
 */
TEST_CASE("Insert One Vertex", "[graphImplementation][insertVertex]") {
  Graph::Station test_station = Graph::Station(0, 0, 0);
  Graph test_graph;
  test_graph.insertVertex(test_station);
  
  Graph::VertexData* vertex_data = test_graph.getVertex(0);
  // check the vertex got added
  REQUIRE(vertex_data != nullptr);
  // check that the vertex stores the right information
  REQUIRE(areStationsEqual(test_station, vertex_data->station_));
}

TEST_CASE("Insert Multiple Vertexes", "[graphImplementation]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);
  Graph::Station test_station_c = Graph::Station(2, 0, 0);
  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_b);
  test_graph.insertVertex(test_station_c);

  std::map<int, Graph::VertexData*> resulting_verticies = test_graph.getVertexMap();
  // check that the right amount of verticies were added
  REQUIRE(resulting_verticies.size() == 3);

  // check the correct data was added into the map
  std::vector<Graph::Station> stations = {test_station_a, test_station_b, test_station_c};
  for (Graph::Station station : stations) {
    REQUIRE(areStationsEqual(station, (test_graph.getVertex(station.id_))->station_));
  }
}


TEST_CASE("Duplicate Vertex", "[valgrind][graphImplementation][insertVertex]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_a2 = Graph::Station(0, 0, 0);
  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_a2);

  std::map<int, Graph::VertexData*> resulting_verticies = test_graph.getVertexMap();
  // check that the right amount of verticies were added
  REQUIRE(resulting_verticies.size() == 1);

  REQUIRE(areStationsEqual(test_station_a, (test_graph.getVertex(test_station_a.id_))->station_));
}

TEST_CASE("Test Add One Edge", "[graphImplementation][addEdge]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);

  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_b);

  Graph::VertexData* vertex_a = test_graph.getVertex(0);
  Graph::VertexData* vertex_b = test_graph.getVertex(1);

  test_graph.insertEdge(vertex_a, vertex_b);

  std::list<Graph::Edge*> resulting_edges = test_graph.getEdgeList();

  REQUIRE(resulting_edges.size() == 1);

  Graph::Edge* expected_edge = new Graph::Edge(vertex_a, vertex_b);
  Graph::Edge* resulting_edge = resulting_edges.front();

  REQUIRE((*resulting_edge == *expected_edge));

  // test vertex's edge lists were updated 
  REQUIRE(vertex_a->adjacent_edges_.size() == 1);
  REQUIRE(vertex_b->adjacent_edges_.size() == 1);

  // test vertex edge lists correctly updated
  REQUIRE((*(vertex_a->adjacent_edges_.front()) == *expected_edge));
  REQUIRE((*(vertex_b->adjacent_edges_.front()) == *expected_edge));

  // delete allocated memory for valgrind
  delete expected_edge;
}

TEST_CASE("Test Add Multiple Edges", "[graphImplementation][addEdge]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);
  Graph::Station test_station_c = Graph::Station(2, 0, 0);

  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_b);
  test_graph.insertVertex(test_station_c);

  Graph::VertexData* vertex_a = test_graph.getVertex(0);
  Graph::VertexData* vertex_b = test_graph.getVertex(1);
  Graph::VertexData* vertex_c = test_graph.getVertex(2);

  test_graph.insertEdge(vertex_a, vertex_b);
  test_graph.insertEdge(vertex_a, vertex_c);
  test_graph.insertEdge(vertex_b, vertex_c);

  std::list<Graph::Edge*> resulting_edges = test_graph.getEdgeList();

  REQUIRE(resulting_edges.size() == 3);

  Graph::Edge* expected_edge_a = new Graph::Edge(vertex_a, vertex_b);
  Graph::Edge* expected_edge_b = new Graph::Edge(vertex_a, vertex_c);
  Graph::Edge* expected_edge_c = new Graph::Edge(vertex_b, vertex_c);
  std::vector<Graph::Edge*> expected_edges = {expected_edge_a, expected_edge_b, expected_edge_c};

  for (Graph::Edge* edge : expected_edges) {
    Graph::Edge* resulting_edge = resulting_edges.front();
    REQUIRE((*resulting_edge == *edge));
    resulting_edges.pop_front();
  }

    // test vertex's edge lists were updated 
  REQUIRE(vertex_a->adjacent_edges_.size() == 2);
  REQUIRE(vertex_b->adjacent_edges_.size() == 2);
  REQUIRE(vertex_c->adjacent_edges_.size() == 2);

  // delete allocated memory for valgrind
  delete expected_edge_a;
  delete expected_edge_b;
  delete expected_edge_c;
}

TEST_CASE("Duplicate Edge", "[valgrind][graphImplementation][addEdge]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);
  Graph::Station test_station_b = Graph::Station(1, 0, 0);

  Graph test_graph;
  test_graph.insertVertex(test_station_a);
  test_graph.insertVertex(test_station_b);

  Graph::VertexData* vertex_a = test_graph.getVertex(0);
  Graph::VertexData* vertex_b = test_graph.getVertex(1);

  test_graph.insertEdgeFromData(vertex_a, vertex_b);
  test_graph.insertEdgeFromData(vertex_a, vertex_b);

  std::list<Graph::Edge*> resulting_edges = test_graph.getEdgeList();

  REQUIRE(resulting_edges.size() == 1);

  Graph::Edge* expected_edge = new Graph::Edge(vertex_a, vertex_b);
  Graph::Edge* resulting_edge = resulting_edges.front();

  REQUIRE((*resulting_edge == *expected_edge));

    // test vertex's edge lists were updated 
  REQUIRE(vertex_a->adjacent_edges_.size() == 1);
  REQUIRE(vertex_b->adjacent_edges_.size() == 1);

  // test vertex edge lists correctly updated
  REQUIRE((*(vertex_a->adjacent_edges_.front()) == *expected_edge));
  REQUIRE((*(vertex_b->adjacent_edges_.front()) == *expected_edge));

  // delete allocated memory for valgrind
  delete expected_edge;
}

TEST_CASE("Self Loops", "[valgrind][graphImplementation][addEdge]") {
  Graph::Station test_station_a = Graph::Station(0, 0, 0);

  Graph test_graph;
  test_graph.insertVertex(test_station_a);

  Graph::VertexData* vertex_a = test_graph.getVertex(0);

  test_graph.insertEdge(vertex_a, vertex_a);

  std::list<Graph::Edge*> resulting_edges = test_graph.getEdgeList();

  REQUIRE(resulting_edges.size() == 0);

  // test vertex edge list empty
  REQUIRE(vertex_a->adjacent_edges_.empty());
}

/**
 * Test Reading Data into graph from file
 */

 TEST_CASE("One file", "[dataFromFile]") {
   Graph with_data;
   with_data.addDataFromFile("tests/test_data/test_dat1.csv");
   // check correct amount of verticies and edges were created
   REQUIRE(with_data.getVertexMap().size() == 3);
   REQUIRE(with_data.getEdgeList().size() == 2);
   //check correct vertex data was read in
   Graph::Station station_a = Graph::Station(0, 0, 0);
   Graph::Station station_b = Graph::Station(1, 1, 1);
   Graph::Station station_c = Graph::Station(2, 2, 2);

   Graph::VertexData* first_vertex = with_data.getVertex(0);
   Graph::VertexData* second_vertex = with_data.getVertex(1);
   Graph::VertexData* third_vertex = with_data.getVertex(2);
   
   // test station data
   REQUIRE(areStationsEqual(station_a, first_vertex->station_));
   REQUIRE(areStationsEqual(station_b, second_vertex->station_));
   REQUIRE(areStationsEqual(station_c, third_vertex->station_));

   // test edge data
   Graph::Edge expected_edge_a = Graph::Edge(first_vertex, second_vertex);
   Graph::Edge expected_edge_b = Graph::Edge(second_vertex, third_vertex);
   std::list<Graph::Edge*> recieved_edges = with_data.getEdgeList();
   
   REQUIRE((expected_edge_a == *(recieved_edges.front())));
   recieved_edges.pop_front();
   REQUIRE((expected_edge_b == *(recieved_edges.front())));   

   // test vertex edges
   REQUIRE(first_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(first_vertex->adjacent_edges_.front()) == expected_edge_a));

   std::list<Graph::Edge*> second_vertex_edges = second_vertex->adjacent_edges_;
   REQUIRE(second_vertex_edges.size() == 2);
   REQUIRE((*(second_vertex_edges.front()) == expected_edge_a));
   second_vertex_edges.pop_front();
   REQUIRE((*(second_vertex_edges.front()) == expected_edge_b));   

   REQUIRE(third_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(third_vertex->adjacent_edges_.front()) == expected_edge_b));
 }

 TEST_CASE("Multiple files", "[dataFromFile]") {
   Graph with_data;
   with_data.addDataFromFile("tests/test_data/test_dat1.csv");
   with_data.addDataFromFile("tests/test_data/test_dat2.csv");
   // check correct amount of verticies and edges were created
   REQUIRE(with_data.getVertexMap().size() == 4);
   REQUIRE(with_data.getEdgeList().size() == 3);
   //check correct vertex data was read in
   Graph::Station station_a = Graph::Station(0, 0, 0);
   Graph::Station station_b = Graph::Station(1, 1, 1);
   Graph::Station station_c = Graph::Station(2, 2, 2);
   Graph::Station station_d = Graph::Station(3, 3, 3);

   Graph::VertexData* first_vertex = with_data.getVertex(0);
   Graph::VertexData* second_vertex = with_data.getVertex(1);
   Graph::VertexData* third_vertex = with_data.getVertex(2);
   Graph::VertexData* fourth_vertex = with_data.getVertex(3);
   
   // test station data
   REQUIRE(areStationsEqual(station_a, first_vertex->station_));
   REQUIRE(areStationsEqual(station_b, second_vertex->station_));
   REQUIRE(areStationsEqual(station_c, third_vertex->station_));
   REQUIRE(areStationsEqual(station_d, fourth_vertex->station_));

   // test edge data
   Graph::Edge expected_edge_a = Graph::Edge(first_vertex, second_vertex);
   Graph::Edge expected_edge_b = Graph::Edge(second_vertex, third_vertex);
   Graph::Edge expected_edge_c = Graph::Edge(first_vertex, fourth_vertex);
   std::list<Graph::Edge*> recieved_edges = with_data.getEdgeList();
   
   REQUIRE((expected_edge_a == *(recieved_edges.front())));
   recieved_edges.pop_front();
   REQUIRE((expected_edge_b == *(recieved_edges.front())));   
   recieved_edges.pop_front();
   REQUIRE((expected_edge_c == *(recieved_edges.front())));   

   // test vertex edges
   std::list<Graph::Edge*> first_vertex_edges = first_vertex->adjacent_edges_;
   REQUIRE(first_vertex_edges.size() == 2);
   REQUIRE((*(first_vertex_edges.front()) == expected_edge_a));
   first_vertex_edges.pop_front();
   REQUIRE((*(first_vertex_edges.front()) == expected_edge_c));  

   std::list<Graph::Edge*> second_vertex_edges = second_vertex->adjacent_edges_;
   REQUIRE(second_vertex_edges.size() == 2);
   REQUIRE((*(second_vertex_edges.front()) == expected_edge_a));
   second_vertex_edges.pop_front();
   REQUIRE((*(second_vertex_edges.front()) == expected_edge_b));   

   REQUIRE(third_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(third_vertex->adjacent_edges_.front()) == expected_edge_b));

   REQUIRE(fourth_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(fourth_vertex->adjacent_edges_.front()) == expected_edge_c));
 }

TEST_CASE("Repeated Edges", "[valgrind][dataFromFile]") {
   Graph with_data;
   with_data.addDataFromFile("tests/test_data/repeating_edges.csv");
   // check correct amount of verticies and edges were created
   REQUIRE(with_data.getVertexMap().size() == 2);
   REQUIRE(with_data.getEdgeList().size() == 1);

   //check correct vertex data was read in
   Graph::Station station_a = Graph::Station(0, 0, 0);
   Graph::Station station_b = Graph::Station(1, 1, 1);

   Graph::VertexData* first_vertex = with_data.getVertex(0);
   Graph::VertexData* second_vertex = with_data.getVertex(1);
   
   // test station data
   REQUIRE(areStationsEqual(station_a, first_vertex->station_));
   REQUIRE(areStationsEqual(station_b, second_vertex->station_));

   // test edge data
   Graph::Edge expected_edge_a = Graph::Edge(first_vertex, second_vertex);
   REQUIRE((expected_edge_a == *(with_data.getEdgeList().front())));

   // test vertex edges
   REQUIRE(first_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(first_vertex->adjacent_edges_.front()) == expected_edge_a));
   REQUIRE(second_vertex->adjacent_edges_.size() == 1);
   REQUIRE((*(second_vertex->adjacent_edges_.front()) == expected_edge_a));
}

/**
 * Test DFS Traversal
 */

TEST_CASE("Test Basic DFS Traversal", "[valgrind][DFS]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/traversal1_dat.csv");
  DFS test_traversal = DFS(test_graph, test_graph->getVertex(0));

  std::vector<int> expected_stations = {4, 3, 2, 1, 0};
  for (auto it = test_traversal.begin(); it != test_traversal.end(); ++it) {
    REQUIRE((*it)->station_.id_ == expected_stations.back());
    expected_stations.pop_back();
  }
  REQUIRE(test_traversal.getNumConnectedComponents() == 1);
  delete test_graph;
}

TEST_CASE("Test DFS Traversal", "[DFS]") {
   Graph* test_graph = new Graph();
   test_graph->addDataFromFile("tests/test_data/traversal2_dat.csv");
   DFS test_traversal = DFS((test_graph), (test_graph->getVertex(0)));

   std::vector<int> expected_values = {1, 4, 2, 3, 0};
   for (auto it = test_traversal.begin(); it != test_traversal.end(); ++it) {
     REQUIRE((*it)->station_.id_ == expected_values.back());
     expected_values.pop_back();
   }
  REQUIRE(test_traversal.getNumConnectedComponents() == 1);
   delete test_graph;
}

TEST_CASE("Test Basic DFS With Multiple Connected Components", "[valgrind][DFS]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/traversal3_dat.csv");

  DFS test_traversal = DFS(test_graph, test_graph->getVertex(0));

  std::vector<int> expected_values = {4, 3, 2, 1, 0};
  for (auto it = test_traversal.begin(); it != test_traversal.end(); ++it) {
    REQUIRE((*it)->station_.id_ == expected_values.back());
    expected_values.pop_back();
  }
  REQUIRE(test_traversal.getNumConnectedComponents() == 2);
  delete test_graph;
}

TEST_CASE("Test DFS With Multiple Connnnected Components", "[DFS]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/traversal4_dat.csv");

  DFS test_traversal = DFS(test_graph, test_graph->getVertex(0));

  std::vector<int> expected_values = {5, 4, 3, 2, 1, 0};
  for (auto it = test_traversal.begin(); it != test_traversal.end(); ++it) {
    REQUIRE((*it)->station_.id_ == expected_values.back());
    expected_values.pop_back();
  }
  REQUIRE(test_traversal.getNumConnectedComponents() == 2);
  delete test_graph;
}

/**
 * Test for connectivity and euler path/circuit
 */
TEST_CASE("Unconnected Zero Degree Non-Eulerian Graph",
          "[valgrind][checkConnected][checkEulerian]") {
  Graph non_eulerian;

  REQUIRE_FALSE(non_eulerian.isConnected());
  REQUIRE(non_eulerian.isEulerian() == 0);
}

TEST_CASE("Euler Path Plus Unconnected Vertex", "[valgrind][checkConnected][checkEulerian]") {
  Graph::Station test_station_0 = Graph::Station(0, 0, 0);
  Graph::Station test_station_1 = Graph::Station(1, 0, 0);
  Graph::Station test_station_2 = Graph::Station(2, 0, 0);
  Graph::Station test_station_3 = Graph::Station(3, 0, 0);
  Graph::Station test_station_4 = Graph::Station(4, 0, 0);
  Graph::Station test_station_5 = Graph::Station(5, 0, 0);

  Graph non_eulerian;
  non_eulerian.insertVertex(test_station_0);
  non_eulerian.insertVertex(test_station_1);
  non_eulerian.insertVertex(test_station_2);
  non_eulerian.insertVertex(test_station_3);
  non_eulerian.insertVertex(test_station_4);
  non_eulerian.insertVertex(test_station_5);

  Graph::VertexData *vertex_0 = non_eulerian.getVertex(0);
  Graph::VertexData *vertex_1 = non_eulerian.getVertex(1);
  Graph::VertexData *vertex_2 = non_eulerian.getVertex(2);
  Graph::VertexData *vertex_3 = non_eulerian.getVertex(3);
  Graph::VertexData *vertex_4 = non_eulerian.getVertex(4);

  non_eulerian.insertEdge(vertex_0, vertex_1);
  non_eulerian.insertEdge(vertex_2, vertex_1);
  non_eulerian.insertEdge(vertex_0, vertex_3);
  non_eulerian.insertEdge(vertex_3, vertex_4);
  // vertex from test_station_5 not connected to graph

  REQUIRE_FALSE(non_eulerian.isConnected());
  REQUIRE(non_eulerian.isEulerian() == 0);
}

TEST_CASE("One Eulerian Path", "[valgrind][checkConnected][checkEulerian]") {
  Graph::Station test_station_0 = Graph::Station(0, 0, 0);
  Graph::Station test_station_1 = Graph::Station(1, 0, 0);  
  Graph::Station test_station_2 = Graph::Station(2, 0, 0);
  Graph::Station test_station_3 = Graph::Station(3, 0, 0);
  Graph::Station test_station_4 = Graph::Station(4, 0, 0);

  Graph eulerian_path;
  eulerian_path.insertVertex(test_station_0);
  eulerian_path.insertVertex(test_station_1);
  eulerian_path.insertVertex(test_station_2);
  eulerian_path.insertVertex(test_station_3);
  eulerian_path.insertVertex(test_station_4);

  Graph::VertexData *vertex_0 = eulerian_path.getVertex(0);
  Graph::VertexData *vertex_1 = eulerian_path.getVertex(1);
  Graph::VertexData *vertex_2 = eulerian_path.getVertex(2);
  Graph::VertexData *vertex_3 = eulerian_path.getVertex(3);
  Graph::VertexData *vertex_4 = eulerian_path.getVertex(4);

  eulerian_path.insertEdge(vertex_0, vertex_1);
  eulerian_path.insertEdge(vertex_0, vertex_2);
  eulerian_path.insertEdge(vertex_2, vertex_1);
  eulerian_path.insertEdge(vertex_0, vertex_3);
  eulerian_path.insertEdge(vertex_3, vertex_4);

  REQUIRE(eulerian_path.isConnected());
  REQUIRE(eulerian_path.isEulerian() == 1);
}

TEST_CASE("Multiple Eulerian Paths", "[valgrind][checkConnected][checkEulerian]") {
  Graph::Station test_station_0 = Graph::Station(0, 0, 0);
  Graph::Station test_station_1 = Graph::Station(1, 0, 0);
  Graph::Station test_station_2 = Graph::Station(2, 0, 0);
  Graph::Station test_station_3 = Graph::Station(3, 0, 0);
  Graph::Station test_station_4 = Graph::Station(4, 0, 0);

  Graph eulerian_path;
  eulerian_path.insertVertex(test_station_0);
  eulerian_path.insertVertex(test_station_1);
  eulerian_path.insertVertex(test_station_2);
  eulerian_path.insertVertex(test_station_3);
  eulerian_path.insertVertex(test_station_4);

  Graph::VertexData *vertex_0 = eulerian_path.getVertex(0);
  Graph::VertexData *vertex_1 = eulerian_path.getVertex(1);
  Graph::VertexData *vertex_2 = eulerian_path.getVertex(2);
  Graph::VertexData *vertex_3 = eulerian_path.getVertex(3);
  Graph::VertexData *vertex_4 = eulerian_path.getVertex(4);

  eulerian_path.insertEdge(vertex_1, vertex_0);
  eulerian_path.insertEdge(vertex_0, vertex_2);
  eulerian_path.insertEdge(vertex_3, vertex_1);
  eulerian_path.insertEdge(vertex_0, vertex_3);
  eulerian_path.insertEdge(vertex_0, vertex_4);

  REQUIRE(eulerian_path.isConnected());
  REQUIRE(eulerian_path.isEulerian() == 1);
}

TEST_CASE("One Eulerian Cycle", "[valgrind][checkConnected][checkEulerian]") {
  Graph::Station test_station_0 = Graph::Station(0, 0, 0);
  Graph::Station test_station_1 = Graph::Station(1, 0, 0);
  Graph::Station test_station_2 = Graph::Station(2, 0, 0);

  Graph eulerian_cycle;
  eulerian_cycle.insertVertex(test_station_0);
  eulerian_cycle.insertVertex(test_station_1);
  eulerian_cycle.insertVertex(test_station_2);

  Graph::VertexData *vertex_0 = eulerian_cycle.getVertex(0);
  Graph::VertexData *vertex_1 = eulerian_cycle.getVertex(1);
  Graph::VertexData *vertex_2 = eulerian_cycle.getVertex(2);

  eulerian_cycle.insertEdge(vertex_1, vertex_0);
  eulerian_cycle.insertEdge(vertex_1, vertex_2);
  eulerian_cycle.insertEdge(vertex_2, vertex_0);

  REQUIRE(eulerian_cycle.isConnected());
  REQUIRE(eulerian_cycle.isEulerian() == 2);

  // add station vertex without edge makes cycle become unconnected and non-eulerian
  Graph::Station test_station_3 = Graph::Station(3, 0, 0);
  eulerian_cycle.insertVertex(test_station_3);

  REQUIRE_FALSE(eulerian_cycle.isConnected());
  REQUIRE(eulerian_cycle.isEulerian() == 0);
}

TEST_CASE("Multiple Eulerian Cycles", "[valgrind][checkConnected][checkEulerian]") {
  Graph::Station test_station_0 = Graph::Station(0, 0, 0);
  Graph::Station test_station_1 = Graph::Station(1, 0, 0);
  Graph::Station test_station_2 = Graph::Station(2, 0, 0);
  Graph::Station test_station_3 = Graph::Station(3, 0, 0);
  Graph::Station test_station_4 = Graph::Station(4, 0, 0);

  Graph eulerian_cycle;
  eulerian_cycle.insertVertex(test_station_0);
  eulerian_cycle.insertVertex(test_station_1);
  eulerian_cycle.insertVertex(test_station_2);
  eulerian_cycle.insertVertex(test_station_3);
  eulerian_cycle.insertVertex(test_station_4);

  Graph::VertexData *vertex_0 = eulerian_cycle.getVertex(0);
  Graph::VertexData *vertex_1 = eulerian_cycle.getVertex(1);
  Graph::VertexData *vertex_2 = eulerian_cycle.getVertex(2);
  Graph::VertexData *vertex_3 = eulerian_cycle.getVertex(3);
  Graph::VertexData *vertex_4 = eulerian_cycle.getVertex(4);

  eulerian_cycle.insertEdge(vertex_1, vertex_0);
  eulerian_cycle.insertEdge(vertex_0, vertex_2);
  eulerian_cycle.insertEdge(vertex_2, vertex_1);
  eulerian_cycle.insertEdge(vertex_2, vertex_3);
  eulerian_cycle.insertEdge(vertex_2, vertex_4);
  eulerian_cycle.insertEdge(vertex_3, vertex_4);

  REQUIRE(eulerian_cycle.isConnected());
  REQUIRE(eulerian_cycle.isEulerian() == 2);
}

/**
 * Test Hamiltonian Cycle
 */
TEST_CASE("Simple Hamiltonian Cycle", "[valgrind][HamiltonianCycle]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/hamiltonian1_dat.csv");
  Graph* largest_hamiltonian_ = test_graph->getLargestHamiltonianCycle();
  REQUIRE(largest_hamiltonian_->size() == 5);
  REQUIRE(largest_hamiltonian_->getTotalDistance() == Approx(3*std::sqrt(2) + std::sqrt(8) + std::sqrt(18)));
  REQUIRE(largest_hamiltonian_->getEdgeList().size() == 5);
  delete test_graph;
}

TEST_CASE("Test Graph With Hamiltonian Cycles Of Multiple Weights", "[valgrind][HamiltonianCycle]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/hamiltonian2_dat.csv");
  Graph* largest_hamiltonian_ = test_graph->getLargestHamiltonianCycle();
  REQUIRE(largest_hamiltonian_->size() == 7);
  REQUIRE(largest_hamiltonian_->getTotalDistance() == Approx(672.1485));
  REQUIRE(largest_hamiltonian_->getEdgeList().size() == 7);
  delete test_graph;
}

TEST_CASE("Graph Without Hamiltonian Cycle", "[valgrind][HamiltonianCycle]") {
   Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/non_hamiltonian_dat.csv");
  Graph* largest_hamiltonian_ = test_graph->getLargestHamiltonianCycle();
  REQUIRE(largest_hamiltonian_ == nullptr);
  delete test_graph; 
}

/**
 * Test Remove Edge
 */
TEST_CASE("Test Remove Vertex Without Edges", "[valgrind][RemoveVertex][size]") {
  Graph* test_graph = new Graph();
  Graph::Station test_station = Graph::Station(0, 0, 0);
  test_graph->insertVertex(test_station);
  Graph::VertexData* to_remove = test_graph->getVertex(0);
  test_graph->removeVertex(to_remove);
  REQUIRE(test_graph->size() == 0);
  delete test_graph;
}

TEST_CASE("Test Remove Vertex With One Edge", "[valgrind][RemoveVertex][size]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/test_dat1.csv");
  Graph::VertexData* to_remove = test_graph->getVertex(0);
  test_graph->removeVertex(to_remove);
  REQUIRE(test_graph->size() == 2);
  REQUIRE(test_graph->getEdgeList().size() == 1);
  delete test_graph;
}

TEST_CASE("Test Remove Vertex With Edges", "[valgrind][RemoveVertex][size]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/traversal2_dat.csv");
  Graph::VertexData* to_remove = test_graph->getVertex(4);
  test_graph->removeVertex(to_remove);
  REQUIRE(test_graph->size() == 4);
  REQUIRE(test_graph->getEdgeList().size() == 4);
  delete test_graph;
}

TEST_CASE("Test Remove Multiple Verticies", "[valgrind][RemoveVertex][size]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/traversal2_dat.csv");
  Graph::VertexData* to_remove = test_graph->getVertex(4);
  test_graph->removeVertex(to_remove);
  REQUIRE(test_graph->size() == 4);
  REQUIRE(test_graph->getEdgeList().size() == 4);

  Graph::VertexData* second_to_remove = test_graph->getVertex(2);
  test_graph->removeVertex(second_to_remove);
  
  REQUIRE(test_graph->size() == 3);
  REQUIRE(test_graph->getEdgeList().size() == 2);

  Graph::VertexData* third_to_remove = test_graph->getVertex(3);
  test_graph->removeVertex(third_to_remove);

  REQUIRE(test_graph->size() == 2);
  REQUIRE(test_graph->getEdgeList().size() == 1);

  delete test_graph;
}

/**
 * Test find Northwest Most & Southeast Most Stations
 */
TEST_CASE("Check NorthwestMost", "[NorthwestMost]") {
  Graph with_data;
  with_data.addDataFromFile("tests/test_data/test_dat_ex.csv");

  Graph::VertexData * north_w_most = with_data.getNorthwestMost();
  REQUIRE(north_w_most->station_.id_ == 0);
}

TEST_CASE("Check SoutheastMost", "[SoutheastMost]") {
  Graph with_data;
  with_data.addDataFromFile("tests/test_data/test_dat_ex.csv");

  Graph::VertexData * south_e_most = with_data.getSoutheastMost();
  REQUIRE(south_e_most->station_.id_ == 3);
}

/**
 * Test Edge Distance
 */
TEST_CASE("Edge & Total Distance", "[EdgeDistance]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/test_dat1.csv");
  for (Graph::Edge* edge : test_graph->getEdgeList()) {
    REQUIRE(edge->getEdgeDistance() == std::sqrt(2));
  }
  REQUIRE(test_graph->getTotalDistance() == 2 * std::sqrt(2));
  delete test_graph;
}

TEST_CASE("Total Distance Updates After Edge Removal", "[EdgeDistance]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/test_dat1.csv");
  test_graph->removeVertex(test_graph->getVertex(0));
  REQUIRE(test_graph->getTotalDistance() == std::sqrt(2));
  delete test_graph;
}

TEST_CASE("Total Distance 0 After All Edges Removed", "[valgrind][EdgeDistance]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/test_dat1.csv");
  test_graph->removeVertex(test_graph->getVertex(1));
  REQUIRE(test_graph->getTotalDistance() == 0);
  delete test_graph;
}

/**
 * Test Dijkstras
 */
TEST_CASE("Test Basic Dijkstra's", "[valgrind][Dijkstras]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/dijkstra1_dat.csv");

  Graph::VertexData* first = test_graph->getVertex(0);

  std::pair<Graph, std::map<int, Graph::VertexData*>> dijkstras_result = test_graph->Dijkstras(first);

  Graph result = dijkstras_result.first;
  REQUIRE(result.size() == 4);
  REQUIRE(result.getEdgeList().size() == 3);

  Graph::VertexData* second = test_graph->getVertex(1);
  Graph::VertexData* third = test_graph->getVertex(2);
  Graph::VertexData* fourth = test_graph->getVertex(3);

  Graph::Edge first_edge = Graph::Edge(first, third);
  Graph::Edge second_edge = Graph::Edge(first, fourth);
  Graph::Edge third_edge = Graph::Edge(first, second);
  
  std::vector<Graph::Edge> expected_edges = {third_edge, second_edge, first_edge};
  for (Graph::Edge* edge : result.getEdgeList()) {
    REQUIRE((*edge == expected_edges.back()));
    expected_edges.pop_back();
  }

  for (std::pair<int, Graph::VertexData*> vertex : dijkstras_result.second) {
    if (vertex.first == 0) {
      REQUIRE(vertex.second == nullptr);
    } else  {
      REQUIRE(vertex.second->station_.id_ == 0);
    }
  }
  delete test_graph;
}

TEST_CASE("Test Dijkstra's", "[valgrind][Dijkstras][Tiebreaks]") {
  Graph* test_graph = new Graph();
  test_graph->addDataFromFile("tests/test_data/dijkstra2_dat.csv");

  Graph::VertexData* first = test_graph->getVertex(4);

  std::pair<Graph, std::map<int, Graph::VertexData*>> dijkstras_result = test_graph->Dijkstras(first);
  Graph result = dijkstras_result.first;

  REQUIRE(result.size() == 5);
  REQUIRE(result.getEdgeList().size() == 4);

  Graph::VertexData* second = test_graph->getVertex(0);
  Graph::VertexData* third = test_graph->getVertex(1);
  Graph::VertexData* fourth = test_graph->getVertex(2);
  Graph::VertexData* fifth = test_graph->getVertex(3);

  Graph::Edge first_edge = Graph::Edge(first, third);
  Graph::Edge second_edge = Graph::Edge(first, second);
  Graph::Edge third_edge = Graph::Edge(third, fourth);
  Graph::Edge fourth_edge = Graph::Edge(third, fifth);
  
  std::vector<Graph::Edge> expected_edges = {fourth_edge, third_edge, second_edge, first_edge};
  for (Graph::Edge* edge : result.getEdgeList()) {
    REQUIRE((*edge == expected_edges.back()));
    expected_edges.pop_back();
  }

  std::vector<int> expected_previous_stations = {1, 1, 4, 4};
  for (std::pair<int, Graph::VertexData*> vertex : dijkstras_result.second) {
    if (vertex.first == 4) {
      REQUIRE(vertex.second == nullptr);
    } else  {
      REQUIRE(vertex.second->station_.id_ == expected_previous_stations.back());
      expected_previous_stations.pop_back();
    }
  }
  delete test_graph;
}
