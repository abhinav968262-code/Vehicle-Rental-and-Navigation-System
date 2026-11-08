#ifndef M2_H
#define M2_H
#include <iostream>
#include <string>
#include "Json.h"
#include <vector>
#include <algorithm>
using namespace std;
class Vehicle{
private:
    int vehicleID;
    string model;
    string type;
    double pricePerDay;
    bool available;
public:
    Vehicle(){
        vehicleID = 0;
        model = "";
        type = "";
        pricePerDay = 0;
        available = true;
    }
    // Returns false (and leaves a message) when any value is invalid.
    bool inputVehicle(){
        cout << "Enter Vehicle ID: ";
        if (!readInt(vehicleID) || vehicleID <= 0){
            cout << "Invalid Vehicle ID.\n";
            return false;
        }
        cout << "Enter Vehicle Model: ";
        getline(cin >> ws, model);
        cout << "Enter Vehicle Type: ";
        getline(cin >> ws, type);
        cout << "Enter Rental Price Per Day: ";
        if (!readDouble(pricePerDay) || pricePerDay <= 0){
            cout << "Price must be a number greater than zero.\n";
            return false;
        }
        available = true;
        return true;
    }
    void displayVehicle() const{
        cout << "\nVehicle ID: " << vehicleID;
        cout << "\nModel: " << model;
        cout << "\nType: " << type;
        cout << "\nPrice Per Day: " << pricePerDay;
        cout << "\nAvailability: "
             << (available ? "Available" : "Rented");
        cout << "\n-----------------------------\n";
    }
    int getID() const{
        return vehicleID;
    }
    double getPrice() const{
        return pricePerDay;
    }
    bool isAvailable() const{
        return available;
    }
    void setAvailability(bool status){
        available = status;
    }
    bool updateVehicle(){
        string newModel, newType;
        double newPrice;
        cout << "Enter New Model: ";
        getline(cin >> ws, newModel);
        cout << "Enter New Type: ";
        getline(cin >> ws, newType);
        cout << "Enter New Price Per Day: ";
        if (!readDouble(newPrice) || newPrice <= 0 ||
            newModel.empty() || newType.empty()){
            cout << "Invalid details. Vehicle not changed.\n";
            return false;
        }
        model = newModel;
        type = newType;
        pricePerDay = newPrice;
        return true;
    }
    string getModel() const{ return model; }
    string getType() const{ return type; }
    string toJson() const{
        return "{\"id\":" + ts(vehicleID) + ",\"model\":" + jq(model) +
               ",\"type\":" + jq(type) + ",\"price\":" + jn(pricePerDay) +
               ",\"available\":" + (available ? "true" : "false") + "}";
    }
    bool operator<(const Vehicle &v) const{
        return pricePerDay < v.pricePerDay;
    }
};
class VehicleManager{
private:
    vector<Vehicle> vehicles;
public:
    bool addVehicle(){
        Vehicle v;
        if (!v.inputVehicle())
            return false;
        if (searchVehicle(v.getID()) != -1){
            cout << "Vehicle ID already exists!\n";
            return false;
        }
        vehicles.push_back(v);
        cout << "Vehicle Added Successfully!\n";
        return true;
    }
    int searchVehicle(int id) const{
        for (int i = 0; i < (int)vehicles.size(); i++){
            if (vehicles[i].getID() == id)
                return i;
        }
        return -1;
    }
    void displayAll() const{
        if (vehicles.empty()){
            cout << "No vehicles registered yet.\n";
            return;
        }
        for (const Vehicle &v : vehicles)
            v.displayVehicle();
    }
    void displayAvailable() const{
        bool found = false;
        for (const Vehicle &v : vehicles){
            if (v.isAvailable()){
                v.displayVehicle();
                found = true;
            }
        }
        if (!found)
            cout << "No available vehicles.\n";
    }
    void searchAndDisplay(int id) const{
        int index = searchVehicle(id);
        if (index == -1)
            cout << "Vehicle Not Found!\n";
        else
            vehicles[index].displayVehicle();
    }
    void sortByPrice(){
        sort(vehicles.begin(), vehicles.end());
        cout << "Vehicles Sorted By Rental Price.\n";
    }
    void updateVehicleDetails(int id){
        int index = searchVehicle(id);
        if (index == -1){
            cout << "Vehicle Not Found!\n";
            return;
        }
        if (vehicles[index].updateVehicle())
            cout << "Vehicle Updated Successfully!\n";
    }
    bool rentVehicle(int id){
        int index = searchVehicle(id);
        if (index == -1){
            cout << "Vehicle Not Found!\n";
            return false;
        }
        if (!vehicles[index].isAvailable()){
            cout << "Vehicle Is Already Rented!\n";
            return false;
        }
        vehicles[index].setAvailability(false);
        cout << "Vehicle Rented Successfully!\n";
        return true;
    }
    bool returnVehicle(int id){
        int index = searchVehicle(id);
        if (index == -1){
            cout << "Vehicle Not Found!\n";
            return false;
        }
        if (vehicles[index].isAvailable()){
            cout << "Vehicle Is Not Currently Rented!\n";
            return false;
        }
        vehicles[index].setAvailability(true);
        cout << "Vehicle Returned Successfully!\n";
        return true;
    }
    double getVehiclePrice(int id) const{
        int index = searchVehicle(id);
        if (index == -1)
            return -1;
        return vehicles[index].getPrice();
    }
    string toJson() const{
        string o = "[";
        for (size_t i = 0; i < vehicles.size(); i++)
            o += (i ? "," : "") + vehicles[i].toJson();
        return o + "]";
    }
    bool vehicleExists(int id) const{
        return searchVehicle(id) != -1;
    }
    bool vehicleIsAvailable(int id) const{
        int index = searchVehicle(id);
        return index != -1 && vehicles[index].isAvailable();
    }
};
#endif