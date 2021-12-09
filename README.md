# Analyzing Biking Paths Across New York City
CS 225 Final Project -  drshika2-meghanr6-jyt3-joannah2
 
## Overview ##
<i> After loading in the data from the dataset (described below), we use Dijkstra's Algorithm to find the shortest bike path across New York City. Using a DFS traversal, we determined there is not a Euler Circuit or Euler Path in the graph representing the New York City bike paths since it is unconnected. Discovering this, we then found the largest hamiltonian cycle created by the New York City bike paths. </i>
 
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
We generate a graph with nodes as the dataset stations, and 1 edge is drawn between 2 nodes if there has been at least 1 trip between the stations that they represent. The graph edges are weighted based on the distance between the stations calculated using the latitude and longitude values.

The dataset files are concatenated into 1 large file. We account for human error in the data provided (clean the data) by checking that all lines of data used in the creation of the graph have viable start station id, start station name, start station latitude, start station longitude, end station id, end station name, end station latitude, end station longitude data fields. Otherwise, that line of data is not used.
 
## Depth First Search Graph Traversal #
#### Files: DFS.h, DFS.cpp
  <b> Inputs: </b> The graph generated by the dataset (as described in the Dataset/ Graph Representation section)
 
 
  <b> Output: </b> A list of the stations in the graph
 
 
  <b> Target Runtime: </b> O(max(|E|, |V|))
 
## Using Dijkstra's Algorithm to Find the Shortest Bike Path Across New York City ##
#### Files: Graph.h, Graph.cpp
  <b> Inputs: </b> Latitude and longitudes of the bike stations to find the Northwest-most station and the Southeast-most station within the bounds of New York City.
 
 
  <b> Output: </b> A list of stations to travel between in an order that reflects the shortest bike path across New York City
 
 
  <b> Target Runtime: </b> O(|E| + |V|log(|V|))
 
## Determining if there is an Euler Circuit or Euler Path ##
#### Files: Graph.h, Graph.cpp
 
  <b> Inputs: </b> The graph generated by the dataset (as described in the Dataset/ Graph Representation section)
 
 
  <b> Outputs: </b>
 
  <i>Whether an Euler Circuit can exist: </i> 0 if not Eulerian (not connected), 1 if there is a Euler Path, or 2 if there is a Euler Circuit
 
  <b> Target Runtime: </b> O(|V|)
 
 
## Finding the Largest Hamiltonian Cycle ##
#### Files: Graph.h, Graph.cpp
  <b> Inputs: </b> The graph generated by the dataset (as described in the Dataset/ Graph Representation section)
 
 
  <b> Output: </b> A list of stations to travel between in an order that reflects the largest Hamiltonian Cycle in the Graph
 
 
  <b> Target Runtime: </b> O(|V|^2(2^|V|))
 
## Setup ##
Required installations:
* [VS Code] (or IDE with C++) (https://code.visualstudio.com/download)
* [Catch C++] (https://github.com/catchorg/Catch2) (for testing)
* [Boost C++ Library] (https://www.boost.org/) (for Djikstra's)
 
Build and run in IDE:
```
make main
./main
```
 
## Testing ##
#### Files: tests.cpp, test_data folder
To compile and run tests:
```
make test
./test
```

