#ifndef JSON_H
#define JSON_H
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>
// Helpers shared by the modules: JSON output for the website, safe integer input.
inline std::string jq(const std::string &s){
    std::string o = "\"";
    for (unsigned char c : s){
        switch (c){
            case '"':  o += "\\\""; break;
            case '\\': o += "\\\\"; break;
            case '\n': o += "\\n";  break;
            case '\r': o += "\\r";  break;
            case '\t': o += "\\t";  break;
            default:
                if (c < 0x20){ char b[8]; sprintf(b, "\\u%04x", c); o += b; }
                else o += (char)c;
        }
    }
    return o + "\"";
}
template <class T> inline std::string ts(T v){ std::ostringstream o; o << v; return o.str(); }
// std::to_string is missing on old MinGW (GCC 6), so we use our own.
inline std::string numstr(long long v){ std::ostringstream o; o << v; return o.str(); }
inline std::string jn(double d){ std::ostringstream o; o << d; return o.str(); }
inline bool readInt(int &x){
    if (std::cin >> x) return true;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}
inline bool readDouble(double &x){
    if (std::cin >> x) return true;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}
#endif
