#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <tuple>
#include <vector>
#include <string>
#include <map>

#include "BST.h"

typedef std::tuple<int, int, int> Edge;

class FlightGraph {
    public:
        FlightGraph();
        void addEdge(int, Edge);
        int getAirportIndex(std::string);
        int addAirport(std::string);
        std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> dijkstra(int);
        std::tuple<std::vector<std::string>, int, int> extractPath(std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>, int);
        std::string getAirportName(int);
        std::vector<int> calculateConnections();

        FlightGraph createUndirectedGraph();
    private:
        std::vector<std::vector<Edge>> edges;
        BST airportMap;
};

#endif