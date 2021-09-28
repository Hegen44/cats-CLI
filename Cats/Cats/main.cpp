// main.cpp : This file contains the 'main' function. 
//
#include <iostream>
#include <fstream>
#include <filesystem>

#include <string>
#include <vector>
#include <unordered_map>

#include "helper.h"
#include "crud.h"
using namespace std;

unordered_map<string, int> dictionary = {
    { "create", 1},
    { "read",   2},
    { "update", 3},
    { "delete", 4},
    { "help",   5}
};

/**
    Program execution begins and ends there.

    @param argc command argument counts
    @param argv command argument vector
*/
int main(int argc, char** argv)
{
    /*---------------------------------------------------------
     *  Has no argument, display help informaiton to the user
     *  and exit the program
     *---------------------------------------------------------*/
    if (argc <= 1) {
        Help();
        return 0;
    }

    /*---------------------------------------------------------
     *  Read data from cats.txt and store them into 'Record'.
     *  if no cats.txt detected, then the program will create one
     *  at the current directory. If create operation is failed,
     *  display error message and exit program.
     *---------------------------------------------------------*/
    Record R;

    string filename = "cats.txt";
    fstream fsfile;
    fsfile.open(filename, fstream::in);

    if (!fsfile) {
        cout << "Cannot open '" + filename + "', file does not exist. Creating new file.." << endl;
        fsfile.open(filename, fstream::out | fstream::trunc);
        if (!fsfile.is_open())
        {
            cout << "failed to create '" + filename + "' in the current directory, please try again in another directory." << endl;
            return -1;
        }
        cout << "successfully created '" + filename + "' in the current directory" << endl;
        fsfile << R.id << endl;
        fsfile.close();
    }
    else {
       
        fsfile >> R.id;
        cats cat;
        while (fsfile >> cat.id >> cat.name >> cat.breed >> cat.age) {
            R.data.push_back(cat);
        }

        if (R.data.size() == 0) R.id = 0; // reset id if it is empty    
        fsfile.close();
    }
 
    /*---------------------------------------------------------
     *  Take the progam to different base the first argument,
     *  and their corressponding code in the dictionary
     *---------------------------------------------------------*/
    try {
        string key = argv[1];
        int n = dictionary.count(key) > 0 ? dictionary[key] : -1;

        switch (n) {
            case 1: { // create
                Create(argc, argv, R);
                break;
            }
            case 2: { // read
                if (R.data.size() < 1) {
                    cout << "log: File is empty, there is nothing to read" << endl;
                    return 0;
                }
                Read(R);
                break;
            }
            case 3: { // update
                if (argc <= min_argc)  
                    throw invalid_argument("missing ID or argument. See 'cats help'.");
                if (R.data.size() < 1) {
                    cout << "log: File is empty, there is nothing to update" << endl;
                    return 0;
                }
                Update(argc, argv, R);
                break;
            }
            case 4: { //delete
                if (argc <= min_argc)
                    throw invalid_argument("missing ID or argument. See 'cats help'.");
                if (R.data.size() < 1) {
                    cout << "log: File is empty, there is nothing to delete" << endl;
                    return 0;
                }
                Delete(argc, argv, R);
                break;
            }
            case 5: { //help
                Help();
                break;
            }
            default: {
                throw invalid_argument("'" + key + "' is not a valid command. See 'cats help'.");
                break;
            }
        }

    } catch (const invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
        return -1;
    }

    /*---------------------------------------------------------
     *  Save and overwrite all the data in the cats.txt with newest 
     *  data from 'Record'
     *---------------------------------------------------------*/

    fsfile.open(filename, ios::out | ios::trunc);
    fsfile << R.id << endl;
    if (fsfile.is_open()) {
        for (cats c : R.data) {
            fsfile << c.id << " " << c.name << " " << c.breed << " " << c.age << endl;
        }
    }

    fsfile.close();


    return 0;
}
