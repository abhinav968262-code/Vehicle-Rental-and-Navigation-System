#ifndef M1_H
#define M1_H
#include <iostream>
#include <string>
#include <limits>
#include "Json.h"
using namespace std;
class User{
protected:
    int userID;
    string name;
    string phone;
    string username;
    string password;

public:
    User(){
        userID = 0;
        name = "";
        phone = "";
        username = "";
        password = "";
    }
    // Returns true only when every field was entered correctly.
    bool registerUser(){
        cout << "\n===== USER REGISTRATION =====\n";
        cout << "Enter User ID: ";
        if (!readInt(userID) || userID <= 0){
            cout << "Invalid User ID.\n";
            userID = 0;
            return false;
        }
        cout << "Enter Name: ";
        getline(cin >> ws, name);
        cout << "Enter Phone Number: ";
        getline(cin >> ws, phone);
        cout << "Create Username: ";
        getline(cin >> ws, username);
        cout << "Create Password: ";
        getline(cin >> ws, password);
        if (name.empty() || phone.empty() ||
            username.empty() || password.empty()){
            cout << "All fields are required.\n";
            return false;
        }
        cout << "\nRegistration Successful!\n";
        return true;
    }
    bool login(){
        string enteredUsername;
        string enteredPassword;
        cout << "\n===== USER LOGIN =====\n";
        cout << "Enter Username: ";
        cin >> enteredUsername;
        cout << "Enter Password: ";
        cin >> enteredPassword;
        if (matches(enteredUsername, enteredPassword)){
            cout << "\nLogin Successful!\n";
            return true;
        }
        cout << "\nInvalid Username or Password!\n";
        return false;
    }
    bool matches(const string &u, const string &p) const{
        return !username.empty() && u == username && p == password;
    }
    void displayUser() const{
        cout << "\n===== USER DETAILS =====\n";
        cout << "User ID  : " << userID << endl;
        cout << "Name     : " << name << endl;
        cout << "Phone    : " << phone << endl;
        cout << "Username : " << username << endl;
    }
    int getID() const{ return userID; }
    string getName() const{ return name; }
    string getUsername() const{ return username; }
    string toJson() const{
        return "{\"id\":" + ts(userID) + ",\"name\":" + jq(name) +
               ",\"phone\":" + jq(phone) + ",\"username\":" + jq(username) + "}";
    }
    bool operator==(const User &u) const{
        return username == u.username;
    }
};
class Customer : public User{
public:
    void displayRole() const{
        cout << "Role: Customer\n";
    }
};
class Admin : public User{
public:
    void displayRole() const{
        cout << "Role: Admin\n";
    }
};
#endif
