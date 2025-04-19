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

//Task 7: create a function that creates the Prim's algorithm 
std::vector<std::tuple<int,int,int>> FlightGraph::Prim(){
    std::vector<bool>BV(edges.size(),false);//to check if the given value is already in the MST
  
    std::vector<int>origin(edges.size(),-1);//create a vector that will be the origin point
  
    std::vector<int>Min_Cost(edges.size(),450);//will go to the highest # (450) and will replace it if the next # is smaller and would continue until it gets the smallest cost
   
    //declare a function that would be used to make a MST
     std::vector<std::tuple<int,int,int>>MST;
     //declare the min cost as zero at the first index
       Min_Cost[0] = 0;
     //declare the priority queue that holds the value
      PriorityQueue pq;

      pq.add_with_priority(0, 0);
  
     while(!pq.empty()){
       //extract the min from the priority queue
       int p = pq.extract_min();
       BV[p] = true;
       
       
       //the for loop would check the edges that are connected to the main edge
       for(auto& edges:edges[p]){
         int cost = std::get<2>(edges);
         int dest = std::get<0>(edges);
          
         if(!BV[dest] && cost < Min_Cost[dest]){// to sort out the smallest cost with each destination 
           Min_Cost[dest] = cost;
           pq.add_with_priority(dest, cost);
           origin[p] = dest;
         }
         
       }
     }
      int total_cost = 0;
      for(int i = 0; i < edges.size();i++ ){
        if(origin[i] != -1){
          int v_p = origin[i];
          int c_p = Min_Cost[i];//find the minimum cost and put it into c_p
          total_cost += c_p;
          MST.emplace_back(v_p,i,c_p); //puts the given values in the MST
        }
      }
       std::cout << "The total cost is " << total_cost <<std::endl;
       

  return MST;


}
//Task 8 creating a MST using Kruskals' method
std::vector<std::tuple<int,int,int>>FlightGraph::Kruskal(){
  std::vector<std::tuple<int,int,int>>MST2;
  std::vector<int>parent(edges.size());
  std::vector<int>rank(edges.size());
  
  //this for-loop is used to sort all edges in non-decreasing order of their own weight
    for(int i = 0;i < edges.size(); i++){
      for(auto& edges:edges[i]){
        int cost = std::get<2>(edges);
        int dest = std::get<0>(edges);

        if(i < dest){
          MST2.emplace_back(dest,i,cost);
        }
      }
    }
    //create a loop that would sort the edges from lowest to highest
    for(int i = 0; i < edges.size(); i++){
      int in = i;
      for(int j =i+1; j < edges.size(); j++){
        if(std::get<2>(MST2[j]) < std::get<2>(MST2[i]) )
          in = j;
      }
      
      if(in != i){
        std::tuple<int,int,int>temp;
        temp = MST2[i];
        MST2[i] = MST2[in];
        MST2[in] = temp;
      }
    }
    //create a find function that would find the parent
    auto Find = [&](int pa){
      //std::vector<int>parent(edges.size());
      while(pa != parent[pa]){
        parent[pa] = parent[parent[pa]];
        pa = parent[pa];
      }
      return pa;
    };
    //create a unite function that would check if two roots have the same dest (then it would be a cycle)
     auto unite = [&](int x, int y){
       //std::vector<int>rank(edges.size()); 
       int r1 = Find(x);
       int r2 = Find(y);

       if(r1 == r2){
         return false;
       }
       else if(rank[r1] < rank[r2]){
         parent[r1] =r2;
       }
       else if(rank[r1] > rank[r2]){
         parent[r2] = r1;
       }
       else{
         parent[r2] = r1 ,
           rank[r1]++;
       }
       return true;
     };
     //create a MST2 using a for-loop
      int t_c = 0; //variable for total_cost
     for(auto& edges:MST2){
       int i = std::get<1>(edges);
       int cost = std::get<2>(edges);
       int dest = std::get<0>(edges);

       if(unite(dest,i)){
         t_c += cost;
         MST2.emplace_back(dest,i,cost);
       }
     }
    std::cout << "The total cost is " << t_c << std::endl;
         

  return MST2;
  
}
