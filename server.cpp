// DriveEase web server: serves index.html and exposes the M1-M10 modules as a JSON API.
// Build (Linux/Mac): g++ -std=gnu++14 server.cpp -o server
// Build (Windows/MinGW): g++ -std=gnu++14 server.cpp -o server -lws2_32
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <cctype>
#include <cstdlib>
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
#ifdef _WIN32
#include <winsock2.h>
#define CLOSESOCK closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define CLOSESOCK close
#endif
using namespace std;

// ---- the real modules, one instance of each ----
vector<Customer> customers;
vector<Admin> admins;
VehicleManager vm;
RentalManager rm;
NavigationSystem nav;
RouteOptimizer router;
PaymentManager pm;
FeedbackManager fm;
MaintenanceManager mm;
ReportManager report;
FileManager files;
int role = 0;      // 0 = guest, 1 = customer, 2 = admin
int who = -1;      // index into customers/admins

// Runs a module method with its cin/cout redirected, so the console modules work unchanged.
string capture(const string &input, const function<void()> &fn){
    istringstream is(input);
    ostringstream os;
    streambuf *oldIn = cin.rdbuf(is.rdbuf());
    streambuf *oldOut = cout.rdbuf(os.rdbuf());
    cin.clear();
    fn();
    cin.clear();
    cin.rdbuf(oldIn);
    cout.rdbuf(oldOut);
    return os.str();
}
string trim(string s){
    while (!s.empty() && isspace((unsigned char)s.back())) s.pop_back();
    size_t i = 0;
    while (i < s.size() && isspace((unsigned char)s[i])) i++;
    return s.substr(i);
}
int myID(){ return role == 1 ? customers[who].getID() : role == 2 ? admins[who].getID() : 0; }

string stateJson(){
    ostringstream o;
    string name = role == 1 ? customers[who].getName() : role == 2 ? admins[who].getName() : "";
    o << "{\"session\":{\"role\":" << jq(role == 2 ? "admin" : role == 1 ? "customer" : "guest")
      << ",\"name\":" << jq(name) << ",\"id\":" << myID() << "}";
    o << ",\"vehicles\":" << vm.toJson();
    o << ",\"rentals\":" << (role == 0 ? "[]" : rm.toJson(role == 1 ? myID() : 0, pm));
    o << ",\"nav\":" << nav.toJson();
    o << ",\"payments\":" << (role == 2 ? pm.toJson() : "[]");
    o << ",\"feedback\":" << fm.toJson();
    o << ",\"maintenance\":" << (role == 2 ? mm.toJson() : "[]");
    o << ",\"customers\":[";
    if (role == 2) for (size_t i = 0; i < customers.size(); i++) o << (i ? "," : "") << customers[i].toJson();
    o << "],\"stats\":{\"revenue\":" << jn(pm.getTotalRevenue()) << ",\"expenses\":" << jn(mm.getTotalMaintenanceCost())
      << ",\"avgRating\":" << jn(fm.getAverageRating()) << ",\"payments\":" << pm.getPaymentCount()
      << ",\"feedback\":" << fm.getFeedbackCount() << ",\"maintenance\":" << mm.getMaintenanceCount() << "}}";
    return o.str();
}

// Handles one API action. f = the fields sent by the page (one per line).
string api(const string &action, const vector<string> &f){
    auto F = [&](size_t i){ return i < f.size() ? trim(f[i]) : string(); };
    auto join = [&](size_t from){ string s; for (size_t i = from; i < f.size(); i++) s += trim(f[i]) + "\n"; return s; };
    string out, data = "null";
    bool ok = false;
    auto need = [&](int r, const char *msg){ if (r == 2 ? role != 2 : r == 1 ? role != 1 : role == 0){ out = msg; return false; } return true; };
    auto done = [&](const char *key){ ok = out.find(key) != string::npos; };

    if (action == "register_customer"){
        Customer c;
        out = capture(join(0), [&]{ ok = c.registerUser(); });
        for (auto &x : customers) if (x == c || x.getID() == c.getID()) { ok = false; out = "That username or user ID is already taken.\n"; }
        if (ok) customers.push_back(c);
    } else if (action == "register_admin"){
        if (!admins.empty() && role != 2) out = "Only a logged-in admin can register another admin.\n";
        else { Admin a; out = capture(join(0), [&]{ ok = a.registerUser(); });
            for (auto &x : admins) if (x == a || x.getID() == a.getID()) { ok = false; out = "That username or user ID is already taken.\n"; }
            if (ok) admins.push_back(a); }
    } else if (action == "login"){
        bool admin = F(0) == "admin";
        if (admin) { for (size_t i = 0; i < admins.size(); i++) if (admins[i].matches(F(1), F(2))) { role = 2; who = (int)i; ok = true; } }
        else { for (size_t i = 0; i < customers.size(); i++) if (customers[i].matches(F(1), F(2))) { role = 1; who = (int)i; ok = true; } }
        out = ok ? "Login successful.\n" : "Invalid username or password.\n";
    } else if (action == "logout"){
        role = 0; who = -1; ok = true; out = "Logged out.\n";
    } else if (action == "add_vehicle"){
        if (need(2, "Admin login required.\n")){ out = capture(join(0), [&]{ vm.addVehicle(); }); done("Successfully"); }
    } else if (action == "update_vehicle"){
        if (need(2, "Admin login required.\n")){ out = capture(join(1), [&]{ vm.updateVehicleDetails(atoi(F(0).c_str())); }); done("Successfully"); }
    } else if (action == "search_vehicle"){
        out = capture("", [&]{ vm.searchAndDisplay(atoi(F(0).c_str())); }); done("Vehicle ID");
    } else if (action == "sort_vehicles"){
        out = capture("", [&]{ vm.sortByPrice(); }); ok = true;
    } else if (action == "book"){
        if (need(1, "Customer login required to book.\n")){
            string from = F(2), to = F(3);
            bool trip = !from.empty() && !to.empty();
            if (trip && (nav.getLocationIndex(from) < 0 || nav.getLocationIndex(to) < 0 || from == to)) out = "Choose two different, valid locations (or leave both empty).\n";
            else {
                out = capture(ts(myID()) + "\n" + F(0) + "\n" + F(1) + "\n", [&]{ rm.createBooking(vm); });
                done("Booking Successful");
                if (ok && trip){
                    rm.lastRental()->setTrip(from, to);
                    vector<int> p; double km = 0;
                    if (router.computeRoute(nav, nav.getLocationIndex(from), nav.getLocationIndex(to), p, km)){
                        data = "{\"km\":" + jn(km) + ",\"path\":[";
                        for (size_t i = 0; i < p.size(); i++) data += (i ? "," : "") + jq(nav.getLocationName(p[i]));
                        data += "]}";
                    }
                }
            }
        }
    } else if (action == "return_rental"){
        if (need(0, "Please log in first.\n")){
            int rid = atoi(F(0).c_str());
            Rental *r = rm.findRental(rid);
            if (r && role == 1 && r->getCustomerID() != myID()) out = "That rental belongs to another customer.\n";
            else { out = capture(ts(rid) + "\n", [&]{ rm.returnRental(vm); }); done("Completed Successfully"); }
        }
    } else if (action == "add_location"){
        if (need(2, "Admin login required.\n")){ out = capture(join(0), [&]{ nav.addLocation(); }); done("successfully"); }
    } else if (action == "add_road"){
        if (need(2, "Admin login required.\n")){ out = capture(join(0), [&]{ nav.addRoad(); }); done("successfully"); }
    } else if (action == "route"){
        vector<int> p; double km = 0;
        int a = nav.getLocationIndex(F(0)), b = nav.getLocationIndex(F(1));
        if (a < 0 || b < 0) out = "Choose two valid locations.\n";
        else if (a == b) out = "Pickup and destination are the same place.\n";
        else if (!router.computeRoute(nav, a, b, p, km)) out = "No route exists between these locations.\n";
        else {
            ok = true; out = "Shortest route found.\n";
            data = "{\"km\":" + jn(km) + ",\"path\":[";
            for (size_t i = 0; i < p.size(); i++) data += (i ? "," : "") + jq(nav.getLocationName(p[i]));
            data += "]}";
        }
    } else if (action == "pay"){
        if (need(1, "Customer login required to pay.\n")){
            Rental *r = rm.findRental(atoi(F(0).c_str()));
            if (!r || r->getCustomerID() != myID()) out = "Rental not found for your account.\n";
            else { out = capture(join(0), [&]{ pm.makePayment(); }); done("recorded successfully"); }
        }
    } else if (action == "feedback"){
        if (need(1, "Customer login required to send feedback.\n")){ out = capture(ts(myID()) + "\n" + join(0), [&]{ fm.addFeedback(); }); done("successfully"); }
    } else if (action == "add_maintenance"){
        if (need(2, "Admin login required.\n")){
            if (!vm.vehicleExists(atoi(F(0).c_str()))) out = "Vehicle does not exist.\n";
            else { out = capture(join(0), [&]{ mm.addMaintenance(); }); done("successfully"); }
        }
    } else if (action == "report"){
        if (need(2, "Admin login required.\n")){ out = capture("", [&]{ report.generateReport(pm, fm, mm); }); ok = true; }
    } else if (action == "save_all"){
        if (need(2, "Admin login required.\n")){ out = capture("", [&]{ files.saveAll(pm, fm, mm); }); ok = true; }
    } else if (action == "read_file"){
        if (need(2, "Admin login required.\n")){ out = capture(join(0), [&]{ files.readFile(); }); done("FILE CONTENTS"); }
    } else out = "Unknown action.\n";

    return "{\"ok\":" + string(ok ? "true" : "false") + ",\"msg\":" + jq(trim(out)) + ",\"data\":" + data + ",\"state\":" + stateJson() + "}";
}

void seed(){
    Admin a; capture("1\nAdmin\n9000000001\nadmin\nadmin123\n", [&]{ a.registerUser(); }); admins.push_back(a);
    Customer c; capture("101\nDemo Customer\n9876543210\ncustomer\ncust123\n", [&]{ c.registerUser(); }); customers.push_back(c);
    const char *v[] = {"1\nMaruti Swift\nCar\n1200\n","2\nHyundai i20\nCar\n1400\n","3\nToyota Innova\nSUV\n2600\n","4\nMahindra Thar\nSUV\n3000\n","5\nRoyal Enfield 350\nBike\n700\n","6\nHonda Activa\nBike\n400\n"};
    for (auto s : v) capture(s, []{ vm.addVehicle(); });
    const char *l[] = {"Central Station","Airport Road","City Mall","University","Tech Park","Old Town"};
    for (auto s : l) capture(string(s) + "\n", []{ nav.addLocation(); });
    const char *r[] = {"Central Station\nAirport Road\n10\n","Central Station\nCity Mall\n6\n","Airport Road\nUniversity\n8\n","City Mall\nUniversity\n8\n","Airport Road\nCity Mall\n2\n","University\nTech Park\n5\n","City Mall\nTech Park\n9\n","University\nOld Town\n6\n","Tech Park\nOld Town\n7\n"};
    for (auto s : r) capture(s, []{ nav.addRoad(); });
}

// ---- tiny HTTP layer ----
bool waitReadable(SOCKET s){ fd_set fs; FD_ZERO(&fs); FD_SET(s, &fs); timeval tv = {1, 0}; return select((int)s + 1, &fs, 0, 0, &tv) > 0; }
void reply(SOCKET c, int code, const string &type, const string &body){
    string h = "HTTP/1.1 " + ts(code) + (code == 200 ? " OK" : " Not Found") + "\r\nContent-Type: " + type +
               "\r\nContent-Length: " + ts(body.size()) + "\r\nCache-Control: no-store\r\nConnection: close\r\n\r\n";
    string all = h + body; size_t off = 0;
    while (off < all.size()){ int n = send(c, all.data() + off, (int)(all.size() - off), 0); if (n <= 0) break; off += n; }
}
void handle(SOCKET c){
    string req; char buf[4096]; size_t he;
    while ((he = req.find("\r\n\r\n")) == string::npos){
        if (!waitReadable(c)) return;
        int n = recv(c, buf, sizeof buf, 0); if (n <= 0) return;
        req.append(buf, n); if (req.size() > (1 << 20)) return;
    }
    string head = req.substr(0, he), low = head;
    for (auto &ch : low) ch = (char)tolower((unsigned char)ch);
    size_t cl = 0, p = low.find("content-length:");
    if (p != string::npos) cl = (size_t)atoi(low.c_str() + p + 15);
    string body = req.substr(he + 4);
    while (body.size() < cl){ if (!waitReadable(c)) return; int n = recv(c, buf, sizeof buf, 0); if (n <= 0) return; body.append(buf, n); }
    istringstream first(head); string method, path; first >> method >> path;
    if (method == "GET" && (path == "/" || path == "/index.html")){
        ifstream in("index.html", ios::binary); ostringstream o; o << in.rdbuf();
        if (!in) reply(c, 404, "text/plain", "index.html not found. Run the server from the project folder.");
        else reply(c, 200, "text/html; charset=utf-8", o.str());
    } else if (method == "GET" && path == "/api/state"){
        reply(c, 200, "application/json", "{\"state\":" + stateJson() + "}");
    } else if (method == "POST" && path.rfind("/api/", 0) == 0){
        vector<string> f; string line; istringstream bs(body);
        while (getline(bs, line)) f.push_back(line);
        reply(c, 200, "application/json", api(path.substr(5), f));
    } else reply(c, 404, "text/plain", "Not found");
}
int main(int argc, char **argv){
    int port = argc > 1 ? atoi(argv[1]) : 8080;
#ifdef _WIN32
    WSADATA w; WSAStartup(MAKEWORD(2, 2), &w);
#endif
    seed();
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof yes);
    sockaddr_in addr = {}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = htonl(INADDR_ANY); addr.sin_port = htons((unsigned short)port);
    if (bind(s, (sockaddr *)&addr, sizeof addr) < 0 || listen(s, 16) < 0){ cerr << "Cannot listen on port " << port << " (is it already in use?)\n"; return 1; }
    cout << "DriveEase running. Open http://localhost:" << port << " in your browser.\n"
         << "Demo logins - admin: admin / admin123, customer: customer / cust123\n";
    for (;;){ SOCKET c = accept(s, 0, 0); if (c == INVALID_SOCKET) continue; handle(c); CLOSESOCK(c); }
}
