#ifndef M10_H
#define M10_H
#include <iostream>
#include <fstream>
#include <string>
#include "M6.h"
#include "M7.h"
#include "M8.h"
using namespace std;
class FileManager
{
public:
    void savePayments(const PaymentManager &manager){
        ofstream file("payments.txt");
        if (!file){
            cout << "Unable to save payment records.\n";
            return;
        }
        file << "Payment records are maintained in memory.\n";
        file << "Total payment records: "
             << manager.getPaymentCount() << '\n';

        file << "Total revenue: "
             << manager.getTotalRevenue() << '\n';

        file.close();
        cout << "Payment summary saved to payments.txt\n";
    }
    void saveFeedback(const FeedbackManager &manager)
    {
        ofstream file("feedback.txt");

        if (!file){
            cout << "Unable to save feedback summary.\n";
            return;
        }
        file << "Feedback records are maintained in memory.\n";
        file << "Total feedback records: "
             << manager.getFeedbackCount() << '\n';
        file << "Average rating: "
             << manager.getAverageRating() << '\n';
        file.close();
        cout << "Feedback summary saved to feedback.txt\n";
    }
    void saveMaintenance(const MaintenanceManager &manager)
    {
        ofstream file("maintenance.txt");

        if (!file)
        {
            cout << "Unable to save maintenance summary.\n";
            return;
        }

        file << "Maintenance records are maintained in memory.\n";
        file << "Total maintenance records: "
             << manager.getMaintenanceCount() << '\n';

        file << "Total maintenance cost: "
             << manager.getTotalMaintenanceCost() << '\n';

        file.close();

        cout << "Maintenance summary saved to maintenance.txt\n";
    }
    void saveAll(
        const PaymentManager &payments,
        const FeedbackManager &feedback,
        const MaintenanceManager &maintenance)
    {
        savePayments(payments);
        saveFeedback(feedback);
        saveMaintenance(maintenance);
    }
    void readFile(){
        string filename;
        cout << "\nEnter file name to read "
             << "(payments.txt, feedback.txt, maintenance.txt): ";
        cin >> ws;
        getline(cin, filename);
        if (filename != "payments.txt" &&
            filename != "feedback.txt" &&
            filename != "maintenance.txt"){
            cout << "Invalid file name.\n";
            return;
        }
        ifstream file(filename);
        if (!file){
            cout << "File not found. Save records first.\n";
            return;
        }
        string line;
        cout << "\n========== FILE CONTENTS ==========\n";
        while (getline(file, line)){
            cout << line << '\n';
        }
        file.close();
    }
};
#endif