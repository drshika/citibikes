# Analyzing Biking Paths Across New York City
CS 225 Final Project -  drshika2-meghanr6-jyt3-joannah2 

## Overview ##
<i> After loading in the data from the dataset (described below), we used Dijkstra's Algorithm to find the shortest bike path to traverse all bike stations in New York City. We then calculated the northwest-most and southeast-most stations in NYC to use the result of running Dijkstra's from the northwest-most station to find the shortest bike path across NYC. Using a DFS traversal, we determined there is not a Euler Circuit or Euler Path in the graph representing the New York City bike paths. Discovering this, we then attempted to find the largest hamiltonian cycle created by the New York City bike paths; however, due the large amount of data and the fact that this algorithm in NP-Complete, it would take an extremely long time to run the code and we do not know if there is a hamiltonian cycle in the dataset. We also learned that the graph is connected </i>

## Results ##
<b> Is Connected </b>: Yes  
<b> Contains an Euler Circut or Path </b>: No  
<b> Total Stations</b>: 140  
<b> Northwest-Most Station</b>: 4282  
<b> Southeast-Most Station </b>: 3475  
<b> Degrees Latitude of Minimum Spanning Tree </b>: 91  
<b> Stations in Shortest Path Across NYC </b>: 4  
<b> Shortest Path Across NYC </b>: 4282->3202->3186->3475  
<b> Largest Hamiltonian Cycle in NYC </b>: Inconclusive due to NP-Completeness - due to the large size of the data the algorithm would take a very, very large amount of time to run

### Dataset / Graph Representation ###
The [dataset](https://www.kaggle.com/vineethakkinapalli/citibike-bike-sharingnewyork-cityjan-to-apr-2021) provides us data formatted as follows (taken from the dataset documentation):
```
tripduration - Duration in Seconds
starttime - Start Time and Date
stoptime - Stop Time and Date
start station id - ID of Start Station
start station name - Name of Start Station
start station latitude - Latitude of start station
start station longitude - Longitude of start station
end station id - ID of End Station
end station name - Name of End Station
end station latitude - Latitude of End station
end station longitude - Longitude of End station
Bike ID - Bike ID
usertype - Customer = 24-hour pass or 3-day pass user; Subscriber = Annual Member
gender - Zero=unknown; 1=male; 2=female
birthyear - Year of Birth
```
#### Files: data folder
We generate a graph with verticies as the dataset stations, and 1 edge is drawn between 2 nodes if there has been at least 1 trip between the stations that they represent. The graph edges are weighted based on the distance between the stations calculated using the latitude and longitude values.

The dataset files are concatenated into 1 large file. We account for human error in the data provided (clean the data) by checking that all lines of data used in the creation of the graph have viable start station id, start station name, start station latitude, start station longitude, end station id, end station name, end station latitude, end station longitude data fields. Otherwise, that line of data is not used.

## Depth First Search Graph Traversal #
#### Files: DFS.h, DFS.cpp
  <b> Inputs: </b> 
   * The graph generated by the dataset (as described in the Dataset/ Graph Representation section)
   * A pointer to the vertex to begin the traversal from

  <b> Output: </b> An iterator that performs a DFS traversal of the Graph

  <b> Result: </b> 
  ```
  82 3185 3746 327 173 3426 3201 505 3207 3281 514 3540 3196 4004 3178 380 347 3210 4007 3992 500 4069 3191 3206 3649 3314 3791 3245 223 4071 435 3278 3220 3640 358 3677 3270 3678 3194 3664 3679 249 3629 328 525 3198 304 3906 3150 3817 482 3709 3280 2021 3279 3184 315 3919 127 3504 3275 3202 4061 534 3578 3467 4282 3268 519 3277 3638 3461 3545 3205 308 2006 3195 476 83 3481 251 151 3214 4000 3186 3443 540 3475 3794 3193 3187 3744 3792 3715 480 3694 3347 3199 3735 324 3269 3256 3681 2008 3273 417 285 3623 504 3225 3804 3192 363 3213 244 319 489 3282 3267 3748 3203 3547 351 3209 3276 3934 3211 518 143 426 495 406 3798 3272 3639 303 405 3521 3212 3483 

  Number of Stations: 140
  ```

  <b> Runtime: </b> O(max(|E|, |V|))

## Using Dijkstra's Algorithm to Find the Shortest Bike Path That Traverses Every Bike Station in New York City ##
#### Files: Graph.h, Graph.cpp
  <b> Inputs: </b> 
   * A pointer to the vertex to find the minimum spanning tree from

  <b> Output: </b> 
   * A pair containing:
      * The graph that represents the Minimum Spanning Tree of the graph representing the bike paths in New York City from the given vertex (first)
      * A map with:  
        * Key: int representing the station id of the vertex to find the previous vertex of
        * Value: Previous vertex of the given vertex in the shortest path produced by running Dijkstra's on the graph


  <b> Result: </b>
  ```
  Degrees Latitude of the Directed Acyclic Graph Produced by Dijkstra's on the Full Dataset : 91.5746

  Shortest Station Path Across NYC:
  4282 3202 3186 3475 
  ```

  <b> Runtime: </b> O(|E| + |V|log(|V|))

## Determining if there is an Euler Circuit or Euler Path ##
#### Files: Graph.h, Graph.cpp

  <b> Inputs: </b> The graph generated by the dataset (as described in the Dataset/ Graph Representation section)


  <b> Outputs: </b>

  <i>Whether an Euler Circuit can exist: </i> 
   * 0 if not Eulerian
   * 1 if there is a Euler Path
   * 2 if there is a Euler Circuit

  <b> Result </b>: 
  ```
  Graph is Eulerian: 0 (Not Eulerian)
  Graph is Connected: 1 (Graph is Connected)
  ```
  <b> Runtime: </b> O(|V|)


## Finding the Largest Hamiltonian Cycle ##
#### Files: Graph.h, Graph.cpp
  <b> Inputs: </b> The graph generated by the dataset (as described in the Dataset/ Graph Representation section)


  <b> Output: </b> A graph that represents the largest hamiltonian cycle in the graph representing all bike paths
  in New York City

  <b> Result </b>: Due the large amount of data and the fact that this algorithm in NP-Complete, it would take an extremely long time to run the code on the full dataset and thus we do not know if there is a hamiltonian cycle in the across all bike stations in NYC.

  <b> Runtime: </b> O((2|E|)!)

## Setup ##
Required dependencies:
* [VS Code] (or IDE with C++) (https://code.visualstudio.com/download)
* [Catch C++] (https://github.com/catchorg/Catch2) (for testing)
* [Boost C++ Library] (https://www.boost.org/) (for Djikstra's)

Build and run in IDE:
```
make
./project_exe
```

## Testing ##
#### Files: tests.cpp, test_data folder
To compile and run tests:
```
make test
./test
```

Testing Breakdown:  
 * Test Edge Equality Operator (starting line 16)
 * Test Destructor (starting line 85)
 * Test Copy Constructor (starting line 108)
 * Test Graph Implementation (starting line 151)
 * Test Reading data in from a file (starting line 333)
 * Test DFS Traversal (starting line 465)
 * Test isEulerian (starting line 526)
 * Test Largest Hamiltonian Cycle (starting line 689)
 * Test Remove Edge (starting line 720)
 * Test Find Northwest Most & Southeast Most Stations (starting line 776)
 * Test Edge Distance Calculation (starting line 795)
 * Test Edge Dijkstras (starting line 824)

## Final Project Presentation
Google Drive Link: https://drive.google.com/file/d/1T3pU9wQZd1W2RCfjNZmXirZ0OSotqXoX/view?usp=sharing (available with your google apps at illinois account)
