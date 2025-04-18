#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "FlightGraph.h"

#define CSV_FILEPATH "airports.csv"

int main() {
  std::string originAirport = "ABE";
  std::string destinationAirport = "BOS";
  std::string destinationState = "MI";
  
  FlightGraph graph;
  std::vector<std::string> stateBasedAirports;
  
  // Load the csv file
  std::ifstream file(CSV_FILEPATH);
  std::string line;

  if (!file.is_open()) {
    std::cerr << "Error opening file" << std::endl;
    return 1;
  }

  // Skip header line
  std::getline(file, line);

  while (std::getline(file, line)) {
    line += ',';

    // Parse the row
    std::vector<std::string> row;
    std::stringstream ss(line);
    std::string param;
    std::string current;
    bool quoted = false;

    for (char ch : line) {
      if (ch == '"') {
        quoted = !quoted;
      } else if (ch == ',' && !quoted) {
        row.push_back(current);
        current = "";
      } else {
        current += ch;
      }
    }

    std::string originAirport = row[0];
    std::string destinationAirport = row[1];
    std::string originCity = row[2];
    std::string destinationCity = row[3];
    int distance = std::stoi(row[4]);
    int cost = std::stoi(row[5]);

    // If the destination state is equal to the state in the destination city, add the destination airport for future ref
    if (destinationCity.substr(destinationCity.length() - 2, 2) == destinationState && std::find(stateBasedAirports.begin(), stateBasedAirports.end(), destinationAirport) == stateBasedAirports.end()) {
      stateBasedAirports.push_back(destinationAirport);
    }
    
    // Ensure the origin and destination airports have edge indices
    int originIndex = graph.getAirportIndex(originAirport);
    int destinationIndex = graph.getAirportIndex(destinationAirport);

    if (originIndex == -1) {
      originIndex = graph.addAirport(originAirport);
    }

    if (destinationIndex == -1) {
      destinationIndex = graph.addAirport(destinationAirport);
    }

    graph.addEdge(originIndex, std::tuple<int, int, int>(destinationIndex, distance, cost));
  }

  file.close();

  // Task 2 Output
  // Run dijkstra's 
  std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> pathData = graph.dijkstra(graph.getAirportIndex(originAirport));
  std::tuple<std::vector<std::string>, int, int> path = graph.extractPath(pathData, graph.getAirportIndex(destinationAirport));

  std::vector<std::string> pathVec = std::get<0>(path);
  int distance = std::get<1>(path);
  int cost = std::get<2>(path);

  // output the path
  std::cout << "Shortest route from " << originAirport << " to " << destinationAirport << ": ";
  if (pathVec.empty()) {
    std::cout << "None" << std::endl;
  } else {
    // Print the first airport and then the rest
    std::cout << pathVec[0];
    for (int i = 1; i < pathVec.size(); i++) {
      std::cout << " -> " << pathVec[i];
    }

    std::cout << ". The length is " << distance << ". The cost is " << cost << "." << std::endl;
  }

  // Task 3 Output
  std::cout << "Shortest paths from " << originAirport << " to " << destinationState << " state airports are:" << std::endl;
  std::cout << "Airport\t\tDistance\t\tCost" << std::endl;

  // extract the path for each airport in the target state
  for (std::string airport : stateBasedAirports) {
    std::tuple<std::vector<std::string>, int, int> path = graph.extractPath(pathData, graph.getAirportIndex(airport));
    std::vector<std::string> pathVec = std::get<0>(path);
    int distance = std::get<1>(path);
    int cost = std::get<2>(path);

    if (!pathVec.empty()) {
      // Print the first and then rest
      std::cout << pathVec[0];
      for (int i = 1; i < pathVec.size(); i++) {
        std::cout << "->" << pathVec[i];
      }
      std::cout << "\t\t" << distance << "\t\t" << cost << std::endl;
    }
  }

  // Task 5 Output
  std::vector<int> connections = graph.calculateConnections();
  std::cout << "Airport\t\tConnections" << std::endl;
  for (int i = 0; i < connections.size(); i++) {
    std::cout << graph.getAirportName(i) << "\t\t" << connections[i] << std::endl;
  }
}