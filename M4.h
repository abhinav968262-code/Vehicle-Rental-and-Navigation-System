#ifndef M4_H
#define M4_H
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <utility>
#include "Json.h"
using namespace std;
class NavigationSystem{
private:
    vector<string> locations;
    vector<vector<pair<int, double>>> graph;
    int findLocation(const string &name) const{
        for (int i = 0; i < (int)locations.size(); i++){
            if (locations[i] == name){
                return i;
            }
        }
        return -1;
    }
public:
    void addLocation()
    {
        string name;
        cout << "Enter location name: ";
        getline(cin >> ws, name);
        if (name.empty()){
            cout << "Location name cannot be empty.\n";
            return;
        }
        if (findLocation(name) != -1){
            cout << "Location already exists.\n";
            return;
        }
        locations.push_back(name);
        graph.push_back(vector<pair<int, double>>());
        cout << "Location added successfully.\n";
    }
    void addRoad(){
        if (locations.size() < 2){
            cout << "Add at least two locations first.\n";
            return;
        }
        string source, destination;
        double distance;
        cout << "Enter starting location: ";
        getline(cin >> ws, source);
        cout << "Enter destination location: ";
        getline(cin >> ws, destination);
        int u = findLocation(source);
        int v = findLocation(destination);
        if (u == -1 || v == -1){
            cout << "One or both locations do not exist.\n";
            return;
        }
        if (u == v){
            cout << "A road cannot connect a location to itself.\n";
            return;
        }
        for (const auto &edge : graph[u]){
            if (edge.first == v){
                cout << "Road already exists.\n";
                return;
            }
        }
        cout << "Enter distance between locations: ";
        if (!(cin >> distance)){
            cout << "Invalid distance.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if (distance <= 0){
            cout << "Distance must be greater than zero.\n";
            return;
        }
        graph[u].push_back(make_pair(v, distance));
        graph[v].push_back(make_pair(u, distance));
        cout << "Road added successfully.\n";
    }
    void displayLocations() const{
        if (locations.empty()){
            cout << "No locations available.\n";
            return;
        }
        cout << "\n--- Available Locations ---\n";
        for (int i = 0; i < (int)locations.size(); i++){
            cout << i + 1 << ". " << locations[i] << '\n';
        }
    }
    void displayRoads() const{
        bool found = false;
        cout << "\n--- Road Details ---\n";
        for (int i = 0; i < (int)graph.size(); i++){
            for (const auto &edge : graph[i]){
                // Display each undirected road only once
                if (i < edge.first){
                    cout << locations[i] << " <-> "
                         << locations[edge.first]
                         << " : " << edge.second << " km\n";
                    found = true;
                }
            }
        }
        if (!found){
            cout << "No roads available.\n";
        }
    }
    string toJson() const{
        string o = "{\"locations\":[";
        for (size_t i = 0; i < locations.size(); i++)
            o += (i ? "," : "") + jq(locations[i]);
        o += "],\"roads\":[";
        bool first = true;
        for (size_t i = 0; i < graph.size(); i++)
            for (const auto &e : graph[i])
                if ((int)i < e.first){
                    o += (first ? "" : ",") + string("{\"a\":") + jq(locations[i]) +
                         ",\"b\":" + jq(locations[e.first]) + ",\"km\":" + jn(e.second) + "}";
                    first = false;
                }
        return o + "]}";
    }
    int getLocationIndex(string name) const{
        return findLocation(name);
    }
    int getLocationCount() const{
        return (int)locations.size();
    }
    string getLocationName(int index) const{
        if (index < 0 || index >= (int)locations.size())
        {
            return "";
        }

        return locations[index];
    }
    const vector<pair<int, double>>& getNeighbors(int index) const{
        static const vector<pair<int, double>> emptyList;
        if (index < 0 || index >= (int)graph.size()){
            return emptyList;
        }
        return graph[index];
    }
};
#endif