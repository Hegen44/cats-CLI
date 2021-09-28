// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

int main(int argc, char** argv)
{
    

    // no command
    if (argc <= 1) {
        Help();
        cout << "\n\npress any key to continue";
        cin.get();
        return 0;
    }

    //===============================================================================================

    string filename = "cats.txt";
    fstream fsfile;

    Record R;

    fsfile.open(filename, fstream::in | fstream::out);


    // If file does not exist, Create new file
    if (!fsfile)
    {
        cout << "Cannot open '" + filename + "', file does not exist. Creating new file.." << endl;

        fsfile.open(filename, fstream::out | fstream::trunc);
        if (fsfile.is_open())
        {
            cout << "successfully created '" + filename + "' in the current directory" << endl;
            fsfile << R.id << endl;
        }
        else {
            cout << "failed to create '" + filename + "' in the current directory, please try again in another directory." << endl;
            return -1;
        }

    }
    else
    {    // use existing file
        fsfile >> R.id;

        int id;
        string name;
        string bread;
        int age;

        while (fsfile >> id >> name >> bread >> age) {
            cats cat;
            cat.id = id;
            cat.name = name;
            cat.bread = bread;
            cat.age = age;
            R.data.push_back(cat);
        }

        if (R.data.size() == 0) R.id = 0; // reset id if it is empty           
    }
    fsfile.close();

    //===============================================================================================

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
                    cout << "file is empty, there is nothing to read" << endl;
                    return 0;
                }
                Read(R);
                break;
            }
            case 3: {
                // if no id or arguments given 
                if (argc <= 2)  
                    throw invalid_argument("missing ID or argument. See 'cats help'.");
                if (R.data.size() < 1) {
                    cout << "file is empty, there is nothing to update" << endl;
                    return 0;
                }
                Update(argc, argv, R);
                break;
            }
            case 4: {
                if (argc <= 2)
                    throw invalid_argument("missing ID or argument. See 'cats help'.");
                if (R.data.size() < 1) {
                    cout << "file is empty, there is nothing to delete" << endl;
                    return 0;
                }
                Delete(argc, argv, R);
                break;
            }
            case 5: {
                Help();
                break;
            }
            default: {
                throw invalid_argument("'" + key + "' is not a valid command. See 'cats help'.");
                break;
            }
        }

        //===============================================================================================

        fsfile.open(filename, ios::out | ios::trunc);
        fsfile << R.id << endl;
        if (fsfile.is_open()) {
            for (cats c : R.data) {
                fsfile << c.id << " " << c.name << " " << c.bread << " " << c.age << endl;
            }
        }

        fsfile.close();

    }
    
    catch (const invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
        // do stuff with exception... 
    }


    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
