#include <iostream>
#include <limits>

#include "M1.h"
#include "M2.h"
#include "M3.h"
#include "M4.h"
#include "M5.h"
#include "M6.h"
#include "M7.h"
#include "M8.h"
#include "M9.h"
#include "M10.h"

using namespace std;

int main()
{
    // M1: User Management
    Customer customer;
    Admin admin;

    // M2: Vehicle Management
    VehicleManager vehicleManager;

    // M3: Rental Management
    RentalManager rentalManager;

    // M4: Navigation System
    NavigationSystem navigation;

    // M5: Route Optimization
    RouteOptimizer routeOptimizer;

    // M6: Payment Management
    PaymentManager paymentManager;

    // M7: Feedback Management
    FeedbackManager feedbackManager;

    // M8: Vehicle Maintenance
    MaintenanceManager maintenanceManager;

    // M9: Reports
    ReportManager reportManager;

    // M10: File Management
    FileManager fileManager;

    bool customerRegistered = false;
    bool adminRegistered = false;
    bool loggedIn = false;
    bool isAdmin = false;

    int choice;
    int vehicleID;

    do
    {
        cout << "\n";
        cout << "====================================================\n";
        cout << "       VEHICLE RENTAL & NAVIGATION SYSTEM\n";
        cout << "====================================================\n";

        cout << "\n--- M1: USER MANAGEMENT ---\n";
        cout << "1. Register Customer\n";
        cout << "2. Register Admin\n";
        cout << "3. Login\n";
        cout << "4. Logout\n";

        cout << "\n--- M2: VEHICLE MANAGEMENT ---\n";
        cout << "5. Add Vehicle\n";
        cout << "6. Display All Vehicles\n";
        cout << "7. Display Available Vehicles\n";
        cout << "8. Search Vehicle\n";
        cout << "9. Update Vehicle\n";
        cout << "10. Sort Vehicles by Price\n";

        cout << "\n--- M3: RENTAL MANAGEMENT ---\n";
        cout << "11. Book Vehicle\n";
        cout << "12. Display All Rentals\n";
        cout << "13. Display Active Rentals\n";
        cout << "14. Return Rental\n";

        cout << "\n--- M4: NAVIGATION SYSTEM ---\n";
        cout << "15. Add Location\n";
        cout << "16. Add Road\n";
        cout << "17. Display Locations\n";
        cout << "18. Display Roads\n";

        cout << "\n--- M5: ROUTE OPTIMIZATION ---\n";
        cout << "19. Find Shortest Route\n";

        cout << "\n--- M6: PAYMENT MANAGEMENT ---\n";
        cout << "20. Make Payment\n";
        cout << "21. Display Payments\n";

        cout << "\n--- M7: FEEDBACK MANAGEMENT ---\n";
        cout << "22. Submit Feedback\n";
        cout << "23. Display Feedback\n";

        cout << "\n--- M8: VEHICLE MAINTENANCE ---\n";
        cout << "24. Add Maintenance Record\n";
        cout << "25. Display Maintenance Records\n";

        cout << "\n--- M9: REPORTS & ANALYTICS ---\n";
        cout << "26. Generate Report\n";

        cout << "\n--- M10: FILE MANAGEMENT ---\n";
        cout << "27. Save All Summaries\n";
        cout << "28. Read Saved File\n";

        cout << "\n0. Exit\n";

        cout << "\nCurrent User: ";

        if (!loggedIn)
        {
            cout << "Not Logged In\n";
        }
        else if (isAdmin)
        {
            cout << "Admin\n";
        }
        else
        {
            cout << "Customer\n";
        }

        cout << "\nEnter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please enter a number.\n";

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        switch (choice)
        {
            // =====================================
            // M1: USER MANAGEMENT
            // =====================================

            case 1:
                if (customerRegistered)
                {
                    cout << "Customer already registered.\n";
                }
                else
                {
                    if (customer.registerUser())
                    {
                        customerRegistered = true;
                        cout << "Customer registered successfully.\n";
                    }
                }
                break;

            case 2:
                if (adminRegistered)
                {
                    cout << "Admin already registered.\n";
                }
                else
                {
                    if (admin.registerUser())
                    {
                        adminRegistered = true;
                        cout << "Admin registered successfully.\n";
                    }
                }
                break;

            case 3:
            {
                int role;

                cout << "\n1. Customer Login\n";
                cout << "2. Admin Login\n";
                cout << "Enter role: ";

                if (!(cin >> role))
                {
                    cout << "Invalid role input.\n";

                    cin.clear();
                    cin.ignore(
                        numeric_limits<streamsize>::max(), '\n');

                    break;
                }

                if (role == 1)
                {
                    if (!customerRegistered)
                    {
                        cout << "Register a customer first.\n";
                        break;
                    }

                    if (customer.login())
                    {
                        loggedIn = true;
                        isAdmin = false;

                        customer.displayRole();

                        cout << "Customer login successful.\n";
                    }
                    else
                    {
                        cout << "Login failed.\n";
                    }
                }
                else if (role == 2)
                {
                    if (!adminRegistered)
                    {
                        cout << "Register an admin first.\n";
                        break;
                    }

                    if (admin.login())
                    {
                        loggedIn = true;
                        isAdmin = true;

                        admin.displayRole();

                        cout << "Admin login successful.\n";
                    }
                    else
                    {
                        cout << "Login failed.\n";
                    }
                }
                else
                {
                    cout << "Invalid role.\n";
                }

                break;
            }

            case 4:
                if (!loggedIn)
                {
                    cout << "No user is logged in.\n";
                }
                else
                {
                    loggedIn = false;
                    isAdmin = false;

                    cout << "Logged out successfully.\n";
                }
                break;

            // =====================================
            // M2: VEHICLE MANAGEMENT
            // =====================================

            case 5:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    vehicleManager.addVehicle();
                }
                break;

            case 6:
                vehicleManager.displayAll();
                break;

            case 7:
                vehicleManager.displayAvailable();
                break;

            case 8:
                cout << "Enter Vehicle ID: ";

                if (cin >> vehicleID)
                {
                    vehicleManager.searchAndDisplay(vehicleID);
                }
                else
                {
                    cout << "Invalid Vehicle ID.\n";

                    cin.clear();
                    cin.ignore(
                        numeric_limits<streamsize>::max(), '\n');
                }
                break;

            case 9:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    cout << "Enter Vehicle ID to update: ";

                    if (cin >> vehicleID)
                    {
                        vehicleManager.updateVehicleDetails(
                            vehicleID);
                    }
                    else
                    {
                        cout << "Invalid Vehicle ID.\n";

                        cin.clear();
                        cin.ignore(
                            numeric_limits<streamsize>::max(), '\n');
                    }
                }
                break;

            case 10:
                vehicleManager.sortByPrice();

                vehicleManager.displayAll();
                break;

            // =====================================
            // M3: RENTAL MANAGEMENT
            // =====================================

            case 11:
                if (!loggedIn || isAdmin)
                {
                    cout << "Customer login required.\n";
                }
                else
                {
                    rentalManager.createBooking(vehicleManager);
                }
                break;

            case 12:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    rentalManager.displayAllRentals();
                }
                break;

            case 13:
                if (!loggedIn)
                {
                    cout << "Please log in first.\n";
                }
                else
                {
                    rentalManager.displayActiveRentals();
                }
                break;

            case 14:
                if (!loggedIn)
                {
                    cout << "Please log in first.\n";
                }
                else
                {
                    rentalManager.returnRental(vehicleManager);
                }
                break;

            // =====================================
            // M4: NAVIGATION SYSTEM
            // =====================================

            case 15:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    navigation.addLocation();
                }
                break;

            case 16:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    navigation.addRoad();
                }
                break;

            case 17:
                navigation.displayLocations();
                break;

            case 18:
                navigation.displayRoads();
                break;

            // =====================================
            // M5: ROUTE OPTIMIZATION
            // =====================================

            case 19:
                routeOptimizer.findShortestRoute(navigation);
                break;

            // =====================================
            // M6: PAYMENT MANAGEMENT
            // =====================================

            case 20:
                if (!loggedIn || isAdmin)
                {
                    cout << "Customer login required.\n";
                }
                else
                {
                    paymentManager.makePayment();
                }
                break;

            case 21:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    paymentManager.displayPayments();
                }
                break;

            // =====================================
            // M7: FEEDBACK MANAGEMENT
            // =====================================

            case 22:
                if (!loggedIn || isAdmin)
                {
                    cout << "Customer login required.\n";
                }
                else
                {
                    feedbackManager.addFeedback();
                }
                break;

            case 23:
                feedbackManager.displayFeedback();
                break;

            // =====================================
            // M8: VEHICLE MAINTENANCE
            // =====================================

            case 24:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    maintenanceManager.addMaintenance();
                }
                break;

            case 25:
                maintenanceManager.displayMaintenance();
                break;

            // =====================================
            // M9: REPORTS & ANALYTICS
            // =====================================

            case 26:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    reportManager.generateReport(
                        paymentManager,
                        feedbackManager,
                        maintenanceManager);
                }
                break;

            // =====================================
            // M10: FILE MANAGEMENT
            // =====================================

            case 27:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    fileManager.saveAll(
                        paymentManager,
                        feedbackManager,
                        maintenanceManager);
                }
                break;

            case 28:
                if (!loggedIn || !isAdmin)
                {
                    cout << "Admin login required.\n";
                }
                else
                {
                    fileManager.readFile();
                }
                break;

            case 0:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}