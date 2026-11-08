#ifndef M8_H
#define M8_H
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include "Json.h"
using namespace std;
struct Maintenance{
    int vehicleID;
    string service;
    double cost;
    string status;
};
class MaintenanceManager{
private:
    vector<Maintenance> records;
public:
    void addMaintenance(){
        Maintenance m;
        cout << "\nEnter Vehicle ID: ";
        if (!(cin >> m.vehicleID) || m.vehicleID <= 0){
            cout << "Invalid Vehicle ID.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Enter Maintenance Description: ";
        cin >> ws;
        getline(cin, m.service);
        if (m.service.empty()){
            cout << "Description cannot be empty.\n";
            return;
        }
        cout << "Enter Maintenance Cost: ";
        if (!(cin >> m.cost) ||
            m.cost < 0 ||
            m.cost > 1000000000.0){
            cout << "Invalid maintenance cost.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        int choice;
        cout << "Select Status:\n";
        cout << "1. Pending\n";
        cout << "2. Completed\n";
        cout << "Enter choice: ";
        if (!(cin >> choice) || (choice != 1 && choice != 2)){
            cout << "Invalid status choice.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        m.status = (choice == 1) ? "Pending" : "Completed";
        records.push_back(m);
        cout << "Maintenance record added successfully!\n";
    }
    void displayMaintenance() const{
        if (records.empty()){
            cout << "\nNo maintenance records available.\n";
            return;
        }
        cout << "\n========== MAINTENANCE RECORDS ==========\n";
        for (int i = 0; i < (int)records.size(); i++){
            cout << "\nRecord " << i + 1 << '\n';
            cout << "Vehicle ID: "
                 << records[i].vehicleID << '\n';
            cout << "Service: "
                 << records[i].service << '\n';
            cout << fixed << setprecision(2);
            cout << "Cost: Rs. "
                 << records[i].cost << '\n';
            cout << "Status: "
                 << records[i].status << '\n';
        }
    }
    string toJson() const{
        string o = "[";
        for (size_t i = 0; i < records.size(); i++){
            const Maintenance &m = records[i];
            o += (i ? "," : "") + string("{\"vehicle\":") + ts(m.vehicleID) +
                 ",\"service\":" + jq(m.service) + ",\"cost\":" + jn(m.cost) +
                 ",\"status\":" + jq(m.status) + "}";
        }
        return o + "]";
    }
    double getTotalMaintenanceCost() const{
        double total = 0;
        for (const Maintenance &m : records){
            total += m.cost;
        }
        return total;
    }
    int getMaintenanceCount() const{
        return (int)records.size();
    }
};
#endif