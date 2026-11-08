#ifndef M6_H
#define M6_H
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include "Json.h"
using namespace std;
struct Payment{
    int paymentID;
    int rentalID;
    double amount;
    string method;
    string status;
};
class PaymentManager{
private:
    vector<Payment> payments;
    int nextPaymentID = 1;
public:
    void makePayment(){
        Payment p;
        cout << "\nEnter Rental ID: ";
        if (!(cin >> p.rentalID) || p.rentalID <= 0){
            cout << "Invalid Rental ID.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Enter Payment Amount: ";
        if (!(cin >> p.amount) ||
            p.amount <= 0 ||
            p.amount > 1000000000.0){
            cout << "Invalid payment amount.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Enter Payment Method (Cash/UPI/Card): ";
        cin >> ws;
        getline(cin, p.method);
        if (p.method.empty()){
            cout << "Payment method cannot be empty.\n";
            return;
        }
        p.paymentID = nextPaymentID++;
        p.status = "Paid";
        payments.push_back(p);
        cout << "\nPayment recorded successfully!\n";
        cout << "Payment ID: " << p.paymentID << '\n';
        cout << "Status: " << p.status << '\n';
    }
    void displayPayments() const{
        if (payments.empty()){
            cout << "\nNo payment records found.\n";
            return;
        }
        cout << "\n========== PAYMENT RECORDS ==========\n";
        for (const Payment &p : payments){
            cout << "Payment ID: " << p.paymentID << '\n';
            cout << "Rental ID: " << p.rentalID << '\n';
            cout << fixed << setprecision(2);
            cout << "Amount: Rs. " << p.amount << '\n';
            cout << "Method: " << p.method << '\n';
            cout << "Status: " << p.status << '\n';
            cout << "------------------------------------\n";
        }
    }
    double paidFor(int rentalID) const{
        double t = 0;
        for (const Payment &p : payments)
            if (p.rentalID == rentalID) t += p.amount;
        return t;
    }
    string toJson() const{
        string o = "[";
        for (size_t i = 0; i < payments.size(); i++){
            const Payment &p = payments[i];
            o += (i ? "," : "") + string("{\"id\":") + ts(p.paymentID) +
                 ",\"rental\":" + ts(p.rentalID) + ",\"amount\":" + jn(p.amount) +
                 ",\"method\":" + jq(p.method) + ",\"status\":" + jq(p.status) + "}";
        }
        return o + "]";
    }
    double getTotalRevenue() const{
        double total = 0;
        for (const Payment &p : payments){
            if (p.status == "Paid"){
                total += p.amount;
            }
        }
        return total;
    }
    int getPaymentCount() const{
        return (int)payments.size();
    }
};
#endif