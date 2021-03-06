// helper.cpp : This file contains the helper function. 
//

#include <string>
#include <iostream>
#include <vector>
#include "crud.h"
using namespace std;

/**
    Search through a vector of cats for the index of the cat with the matching ID

    @param id the ID we are looking for
    @param vec container containing cats
    @return index of cat with matching ID. Return '-1' if not cat with matching ID.
*/
int IDSearch(const int id, const vector<cats> vec) {
    for (int j = 0; j < vec.size(); ++j) {
        if (vec[j].id == id) {
            return j;
        }
    }
    return -1;
}

/**
    Test the string too see if it is a command argument. It test the first 2 characters
    of the string to see if it is '--'.

    @param str the string to test
    @return result of the test. 'ture' if it is an argrument, else return false
*/
bool IsArgument(const string& str) {
    if (str.size() > 2) { // check the first 2 char
        return str.substr(0, 2) == "--";
    }
    return false;
}

/**
    Convert user input argument into a positive int. The function first turn the string into
    a long before converting into an int. It will return '-1' if the input string is not a number, 
    is a negative number, or if it is too large

    @param str the string to convert to number
    @return number converted from the user input string
*/
int ConverToNumber(const char* str) {
    char* p;
    long arg = strtol(str, &p, 10);
    if (*p != '\0') return -1; // an invalid character was found before the end of the string
    if (arg < 0 || arg > INT_MAX) return -1; // it is a negative number or it is too large and will will overflow
    int arg_int = arg;
    return arg_int;
}


/**
    print help informaation to the console.
*/
void Help() {
    cout << "\n\tusage: cats <command> [<args>]\n\n"
        "\tAvailable commands:\n\n"
        "\tcreate - create and add cats data to the file.\n"
        "\t  options:\n"
        "\t\t--name <name>\n"
        "\t\t   add name to the new cat.\n"
        "\t\t--breed <breed>\n"
        "\t\t   add breed to the new cat.\n"
        "\t\t--age <age>\n"
        "\t\t   add age to the new cat. Only accept positive number.\n\n"
        "\tread - list all cats entry in the file\n\n"
        "\tupdate - edit existing in the file by their corresponding id.\n"
        "\t  options:\n"
        "\t\t<id>\n"
        "\t\t   indexes of cats to update content from.\n"
        "\t\t--name <name>\n"
        "\t\t   update only the name of the cats.\n"
        "\t\t--breed <breed>\n"
        "\t\t   update only the breed of the cats.\n"
        "\t\t--age <age>\n"
        "\t\t   update only the age of the cats. Only accept positive number.\n\n"
        "\tdelete - delete cats from file.\n"
        "\t  options:\n"
        "\t\t<id>\n"
        "\t\t   indexes of cats to remove.\n"
        "\t\t--all\n"
        "\t\t   Delete all the existing records in the file.\n"
        "\thelp - Display help information about Cats." << endl;
}