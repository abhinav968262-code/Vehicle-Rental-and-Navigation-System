#ifndef M7_H
#define M7_H
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include "Json.h"
using namespace std;
struct Feedback{
    int customerID;
    int rating;
    string comment;
};
class FeedbackManager{
private:
    vector<Feedback> feedbackList;
public:
    void addFeedback(){
        Feedback f;
        cout << "\nEnter Customer ID: ";
        if (!(cin >> f.customerID) || f.customerID <= 0){
            cout << "Invalid Customer ID.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Enter Rating (1 to 5): ";
        if (!(cin >> f.rating) ||
            f.rating < 1 ||
            f.rating > 5){
            cout << "Rating must be between 1 and 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cout << "Enter Your Feedback: ";
        cin >> ws;
        getline(cin, f.comment);
        if (f.comment.empty()){
            cout << "Feedback cannot be empty.\n";
            return;
        }
        feedbackList.push_back(f);
        cout << "Feedback submitted successfully!\n";
    }
    void displayFeedback() const{
        if (feedbackList.empty()){
            cout << "\nNo feedback available.\n";
            return;
        }
        cout << "\n========== CUSTOMER FEEDBACK ==========\n";
        for (int i = 0; i < (int)feedbackList.size(); i++){
            cout << "\nFeedback " << i + 1 << '\n';
            cout << "Customer ID: "
                 << feedbackList[i].customerID << '\n';
            cout << "Rating: "
                 << feedbackList[i].rating << "/5\n";
            cout << "Comment: "
                 << feedbackList[i].comment << '\n';
        }
    }
    string toJson() const{
        string o = "[";
        for (size_t i = 0; i < feedbackList.size(); i++){
            const Feedback &f = feedbackList[i];
            o += (i ? "," : "") + string("{\"customer\":") + ts(f.customerID) +
                 ",\"rating\":" + ts(f.rating) + ",\"comment\":" + jq(f.comment) + "}";
        }
        return o + "]";
    }
    double getAverageRating() const{
        if (feedbackList.empty()){
            return 0;
        }
        int total = 0;
        for (const Feedback &f : feedbackList){
            total += f.rating;
        }
        return (double)total / feedbackList.size();
    }
    int getFeedbackCount() const{
        return (int)feedbackList.size();
    }
};
#endif