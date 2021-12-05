#pragma once

#include "Graph.h"

#include <iterator>
#include <stack>
#include <vector>


class DFS {
  public:
    /**
     * DFS Constructor
     *
     * @param Graph* a pointer to the graph to be traversed
     * @param VertexData* a pointer to the starting vertex of the traversal
     */
    DFS(Graph* graph, Graph::VertexData* starting_vertex);

    /*
     * A foward iterator through a Graph (DFS Traversal)
     */
     class Iterator : std::iterator<std::forward_iterator_tag, Graph::VertexData*> {
        public:
          /**
           * Default constructor for a DFS iterator
           */
          Iterator() {dfs_ = NULL;};

          /**
           * Constructor for DFS graph traversal
           * 
           * @param graph_dfs a pointer to a DFS to traverse
           */
          Iterator(DFS* graph_dfs);

          /**
           * Overridden pre-increment operator for DFS Iterator
           * Moves one vertex foward in DFS
           */
          Iterator& operator++();

          /**
           * Overriden De-reference operator for DFS Iterator
           *
           * @return VertexData* a pointer to the Vertex currently being traversed
           */
          Graph::VertexData* operator*();

          /**
           * Overriden != operator for the DFS Iterator
           *
           * @param other DFS Iterator to comopare to
           * @return boolean that is true if the iterators are not equivalent
           */
          bool operator!=(const Iterator &other);

        private:
          // Stores the DFS being traversed
          DFS* dfs_;
     };
    
     /**
      * Function to get the beggining of the DFS's iterator
      */
     Iterator begin();

     /**
      * Function to get the end of the DFS's iterator
      */
     Iterator end();

     /**
      * Function to add a vertex to the DFS's stack
      * @param vertex a pointer to the vertex to add
      */
     void add(Graph::VertexData* vertex);

     /**
      * Function to access at the vertex at the top of the DFS's stack
      * 
      * @return VertexData* a pointer to the vertex at the top of the DFS's stack
      */
     Graph::VertexData* peek() const;

     /**
      * A function to remove the vertex at the top of the DFS's stack
      */
     void pop();
    
     /**
      * A function to check if the traversal is empty
      * 
      * @return a bool that is true is the traversal is empty
      */
     bool empty();

     /**
      * A function that checks if a verticies in the graph have been traversed
      * If not all verticies have been traversed and the stack is empty, it adds 
      * the next un-traversed vertex to the stack
      */
     void checkVerticiesAllExplored();

    private: 
      // Stores the graph being traversed
      Graph* graph_;

      /**
       * Map storing the verticies of the graph being traversed
       * Key: int representing the station id represented by the vertex
       * Value: Pointer to the vertex being represented by the given id
       */
      std::map<int, Graph::VertexData*> vertex_map_;

      // Stack storing the verticies to be travered
      std::stack<Graph::VertexData*> stack_;

      /**
       * size_t storing the next index in the map to check if traversed
       * serves to ensure graphs with multiple connected components are 
       * completely traversed 
       */
      size_t index_in_map_;
};
