#include <stdexcept>
#include <climits>

#include "FlightGraph.h"
#include "PriorityQueue.h"

FlightGraph::FlightGraph() {
  
}

void FlightGraph::addEdge(int originIndex, std::tuple<int, int, int> edge) {
  edges[originIndex].push_back(edge);
}

int FlightGraph::getAirportIndex(std::string airport) {
  try {
    return airportMap.at(airport);
    } catch (const std::out_of_range& e) {
    return -1;
  }
}

int FlightGraph::addAirport(std::string airport) {
  int index = edges.size();
  airportMap.set(airport, index);
  edges.push_back({});

  return index;
}

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
      int alt_distance = distances[current] + std::get<1>(edge);
      int alt_cost = costs[current] + std::get<2>(edge);

      if (alt_distance < distances[v]) {
        previous[v] = current;
        distances[v] = alt_distance;
        costs[v] = alt_cost;
        pq.add_with_priority(v, alt_distance);
      }
    }
  }

  return std::make_tuple(previous, distances, costs);
}

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

std::string FlightGraph::getAirportName(int index) {
  return airportMap.getKey(index);
}
