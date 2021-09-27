// helper.cpp : This file contains the helper function. 
//

#include <string>
#include <sstream>
using namespace std;

bool isNumber(const string& str)
{
    for (char const& c : str) {
        if (isdigit(c) == 0) return false;
    }
    return true;
}

bool isArgument(const string& str) {
    if (str.size() > 2) { // check the first 2 char
        return str.substr(0, 2) == "--";
    }
    return false;
}

int stringToInt(const string& str) {

    // object from the class stringstream
    stringstream ss(str);

    // The object has the value 12345 and stream
    // it to the integer x
    int x = 0;
    ss >> x;
    return x;
}
