#ifndef M9_H
#define M9_H
#include <iostream>
#include <iomanip>
#include "M6.h"
#include "M7.h"
#include "M8.h"
using namespace std;
class ReportManager
{
public:
    void generateReport(
        const PaymentManager &payments,
        const FeedbackManager &feedback,
        const MaintenanceManager &maintenance){
        double revenue = payments.getTotalRevenue();
        double expenses =
            maintenance.getTotalMaintenanceCost();
        double netAmount = revenue - expenses;
        cout << "-----------VEHICLE RENTAL SYSTEM REPORT-----------\n";
        cout << fixed << setprecision(2);
        cout << "\nPAYMENT REPORT\n";
        cout << "Total Payment Records: "
             << payments.getPaymentCount() << '\n';
        cout << "Total Recorded Revenue: Rs. "
             << revenue << '\n';
        cout << "\nFEEDBACK REPORT\n";
        cout << "Total Feedback Records: "
             << feedback.getFeedbackCount() << '\n';
        cout << "Average Customer Rating: "
             << feedback.getAverageRating()
             << "/5\n";
        cout << "\nMAINTENANCE REPORT\n";
        cout << "Total Maintenance Records: "
             << maintenance.getMaintenanceCount() << '\n';
        cout << "Total Maintenance Expenses: Rs. "
             << expenses << '\n';
        cout << "\nFINANCIAL SUMMARY\n";
        cout << "Recorded Revenue: Rs. "
             << revenue << '\n';
        cout << "Maintenance Expenses: Rs. "
             << expenses << '\n';
        cout << "Revenue After Maintenance Expenses: Rs. "
             << netAmount << '\n';
        cout << "\n========================================\n";
    }
};
#endif