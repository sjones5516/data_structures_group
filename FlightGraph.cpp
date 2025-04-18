#include <stdexcept>
#include <climits>
#include <algorithm>

#include "FlightGraph.h"
#include "PriorityQueue.h"

FlightGraph::FlightGraph() {
  
}

void FlightGraph::addEdge(int originIndex, std::tuple<int, int, int> edge) {
  edges[originIndex].push_back(edge);
}

// gets the index by the airport code or -1 if it doesen't exist. This corresponds to the index in edges
int FlightGraph::getAirportIndex(std::string airport) {
  try {
    return airportMap.at(airport);
    } catch (const std::out_of_range& e) {
    return -1;
  }
}

// adds airport code to the bst and returns the index in edges
int FlightGraph::addAirport(std::string airport) {
  int index = edges.size();
  airportMap.set(airport, index);
  edges.push_back({});

  return index;
}

// dijkstra's algorithm, returns the previous node, distances, and costs. See wikipedia page for how this works
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> FlightGraph::dijkstra(int originIndex) {
  PriorityQueue pq;

  std::vector<int> distances(edges.size(), INT_MAX);
  std::vector<int> costs(edges.size(), INT_MAX); 
  std::vector<int> previous(edges.size(), -1);
  distances[originIndex] = 0;
  costs[originIndex] = 0;
  pq.add_with_priority(originIndex, 0);

  while (!pq.empty()) {
    int current = pq.extract_min();
    for (Edge edge : edges[current]) {
      int v = std::get<0>(edge);
      int altDistance = distances[current] + std::get<1>(edge);
      int altCost = costs[current] + std::get<2>(edge);

      if (altDistance < distances[v]) {
        previous[v] = current;
        distances[v] = altDistance;
        costs[v] = altCost;
        pq.add_with_priority(v, altDistance);
      }
    }
  }

  return std::make_tuple(previous, distances, costs);
}

// from the dijkstra data, turns the data into a vector containing the path, distance, and cost
std::tuple<std::vector<std::string>, int, int> FlightGraph::extractPath(std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> pathData, int destinationIndex) {
  std::vector<int> previous = std::get<0>(pathData);
  std::vector<int> distances = std::get<1>(pathData);
  std::vector<int> costs = std::get<2>(pathData);

  std::vector<std::string> path;
  int cost = 0;
  int distance = 0;
  int u = destinationIndex;
  if (previous[u] != -1 || u == destinationIndex) {
    while (u != -1) {
      path.insert(path.begin(), getAirportName(u));
      distance += distances[u];
      cost += costs[u];
      u = previous[u];
    }
  }

  return std::make_tuple(path, distance, cost);
}

// gets the airport code by the index in edges
std::string FlightGraph::getAirportName(int index) {
  return airportMap.getKey(index);
}

// Calculates the number of connections for each airport
std::vector<int> FlightGraph::calculateConnections() {
  std::vector<int> connections = std::vector<int>(edges.size(), 0);
  for (int i = 0; i < edges.size(); i++) {
    connections[i] += edges[i].size();

    for (Edge edge : edges[i]) {
      connections[std::get<0>(edge)]++;
    }
  }

  return connections;
}

FlightGraph FlightGraph::createUndirectedGraph() {
  FlightGraph undirectedGraph;
  // Creates an undirected graph from the original graph. The distances are completely ignored
  for (int i = 0; i < edges.size(); i++) {
    for (Edge edge : edges[i]) {
      // Check if the reverse edge exists
      bool reverseEdgeExists = false;
      for (Edge reverseEdge : edges[std::get<0>(edge)]) {
        if (std::get<0>(reverseEdge) == i) {
          reverseEdgeExists = true;

          // Add the minimum cost edge
          if (std::get<2>(edge) < std::get<2>(reverseEdge)) {
            undirectedGraph.addEdge(i, edge);
          } else {
            undirectedGraph.addEdge(i, reverseEdge);
          }
          
          break;
        }
      }

      if (!reverseEdgeExists) {
        undirectedGraph.addEdge(i, edge);
      }
    }
  }

  return undirectedGraph;
}