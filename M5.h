#ifndef M5_H
#define M5_H
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>
#include <functional>
#include <utility>
#include "M4.h"
using namespace std;
class RouteOptimizer{
public:
    // Dijkstra with a min-heap. Returns false when no road connects start and end.
    bool computeRoute(const NavigationSystem &navigation, int start, int end,
                      vector<int> &path, double &total) const{
        int n = navigation.getLocationCount();
        path.clear();
        if (start < 0 || end < 0 || start >= n || end >= n)
            return false;
        vector<double> distance(n, numeric_limits<double>::infinity());
        vector<int> parent(n, -1);
        priority_queue<pair<double, int>, vector<pair<double, int>>,
                       greater<pair<double, int>>> pq;
        distance[start] = 0;
        pq.push(make_pair(0.0, start));
        while (!pq.empty()){
            double currentDistance = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            if (currentDistance > distance[u])
                continue;
            if (u == end)
                break;
            for (const auto &edge : navigation.getNeighbors(u)){
                int v = edge.first;
                if (distance[u] + edge.second < distance[v]){
                    distance[v] = distance[u] + edge.second;
                    parent[v] = u;
                    pq.push(make_pair(distance[v], v));
                }
            }
        }
        if (distance[end] == numeric_limits<double>::infinity())
            return false;
        for (int cur = end; cur != -1; cur = parent[cur])
            path.push_back(cur);
        reverse(path.begin(), path.end());
        total = distance[end];
        return true;
    }
    void findShortestRoute(const NavigationSystem &navigation){
        if (navigation.getLocationCount() < 2){
            cout << "Add at least two locations first.\n";
            return;
        }
        string source, destination;
        cout << "Enter starting location: ";
        getline(cin >> ws, source);
        cout << "Enter destination location: ";
        getline(cin >> ws, destination);
        int start = navigation.getLocationIndex(source);
        int end = navigation.getLocationIndex(destination);
        if (start == -1 || end == -1){
            cout << "Invalid location name.\n";
            return;
        }
        if (start == end){
            cout << "You are already at " << source << ".\n";
            cout << "Total distance: 0 km\n";
            return;
        }
        vector<int> path;
        double total = 0;
        if (!computeRoute(navigation, start, end, path, total)){
            cout << "No route exists between these locations.\n";
            return;
        }
        cout << "\n--- Shortest Route ---\n";
        for (int i = 0; i < (int)path.size(); i++){
            cout << navigation.getLocationName(path[i]);
            if (i != (int)path.size() - 1)
                cout << " -> ";
        }
        cout << "\nTotal shortest distance: " << total << " km\n";
    }
};
#endif
