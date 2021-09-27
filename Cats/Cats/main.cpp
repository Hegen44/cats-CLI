// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <filesystem>

#include <string>
#include <vector>
#include <unordered_map>

#include "helper.h"
using namespace std;


struct cats {
    int id;
    string name;
    string bread;
    int age;
};

struct Record {
    int id;
    vector<cats> data;
};

unordered_map<string, int> dictionary = {
    { "create", 1},
    { "read",   2},
    { "update", 3},
    { "delete", 4},
    { "help",   5}
};

int idSearch(const int id, const vector<cats> vec) {
    for (int j = 0; j < vec.size(); ++j) {
        if (vec[j].id == id) {
            return j;
        }
    }
    return -1;
}

void Create(Record& R, cats myCat, bool b_name, bool b_bread, bool b_age) {
    cout << "\n log: creating cat..." << endl << endl;

    if (!b_name) {
        cout << "\nName: ";
        cin >> myCat.name;
    }
    if (!b_bread) {
        cout << "\nBread: ";
        cin >> myCat.bread;
    }
    if (!b_age) {
        cout << "\nAge: ";
        while (!(cin >> myCat.age) || myCat.age < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.  Try again.\nAge:  ";
        }
    }

    myCat.id = ++R.id;
    R.data.push_back(myCat);

    cout << "\n log: cat created" << endl << endl;
    cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tBread: " << myCat.bread << "\tAge: " << myCat.age << endl;
}

void Create(int argc, char** argv, Record& R) {

    cats myCat;
    bool b_name = false;
    bool b_bread = false;
    bool b_age = false;

    if (argc <= 2) { // if no arguments given, direct to normal create
        Create(R, myCat, b_name, b_bread, b_age);
    }
    else {

        try {
           
            for (int i = 2; i < argc; ++i) {
                string str = argv[i];
                if (str == "--name") {
                    if(b_name)  throw invalid_argument("duplicated argument.");
                    if (++i < argc) {
                        //++i;
                        myCat.name = argv[i];
                        b_name = true;
                    }
                    else {
                        throw invalid_argument("missing valid input argument. Please provide a name.");
                    }
                }
                else if (str == "--bread") {
                    if (b_bread)  throw invalid_argument("duplicated argument.");
                    if (++i < argc) {
                        //++i;
                        myCat.bread = argv[i];
                        b_bread = true;
                    }
                    else {
                        throw invalid_argument("missing valid input argument. Please provide a bread type.");
                    }
                    
                }
                else if (str == "--age") {
                    if (b_age)  throw invalid_argument("duplicated argument.");
                    if (++i < argc) {
                        //++i;
                        if (isNumber(argv[i])) {
                            int a = stringToInt(argv[i]);
                            if (a > 0) {
                                myCat.age = a;
                            }
                            else {
                                throw invalid_argument("received negative number. Please provide a positive number.");
                            }
                        }
                        else {
                            throw invalid_argument("invalid input argument. Please provide a positive number.");
                        }

                        b_age = true;
                    }
                    else {
                        throw invalid_argument("missing valid input argument. Please provide a name.");
                    }
                    
                }
                else {
                    throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
                }
            }

            Create(R, myCat, b_name, b_bread, b_age);
        }
        catch (const invalid_argument& e) {
            cerr << "error: " << e.what() << endl;
            exit(0); // exit program 
        }



    }
}

void Read(Record& R) {
    if (R.data.size() < 1) {
        cout << "file is empty. Please create cats." << endl;
    }

    for (cats myCat : R.data) {
        cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tBread: " << myCat.bread << "\tAge: " << myCat.age << endl;
    }
}

void Update(cats& myCat) {

     cout << "\n log: updating cat with ID " << myCat.id <<  "..." << endl;

     cout << "\nName: ";
     cin >> myCat.name;


     cout << "\nBread: ";
     cin >> myCat.bread;


     cout << "\nAge: ";
     while (!(cin >> myCat.age) || myCat.age < 0) {
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         cout << "Invalid input.  Try again.\nAge:  ";
     }
    
}

void Update(int argc, char** argv, Record& R) {
    try {
        

        if (argc <= 2) { // if no id or arguments given 
            throw invalid_argument("missing ID or argument. See 'cats help'.");
        }

        if (R.data.size() < 1) {
            throw invalid_argument("file is empty, there is nothing to update.");
        }

        int counter = 0;

        vector<int> keyVec;

        for (int i = 2; i < argc; ++i) {
            string str = argv[i];
            if (isNumber(str)) {
                int num = stringToInt(str);
                if(num > R.id) throw invalid_argument("invalid ID " + str + ". Please provide an valid ID.");


                int key = idSearch(num, R.data);
                // if no key is found, throw exception
                if(key == -1 ) throw invalid_argument("invaild ID " + str + ". Plase provide a valid ID.");
 
                bool b = false;

                while (i + 1 < argc && isArgument(argv[i + 1])) {
                    b = true;
                    ++i;
                    str = argv[i];
                    if (str == "--name") {
                        if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                        R.data[key].name = argv[i];
                    }
                    else if (str == "--bread") {
                        if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                        R.data[key].bread = argv[i];
                    }
                    else if (str == "--age") {
                        if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                        if (isNumber(argv[i])) {
                            int a = stringToInt(argv[i]);
                            if (a > 0) {
                                R.data[key].age = a;
                            }
                            else {
                                throw invalid_argument("received negative number. Please provide a positive number.");
                            }
                        }
                        else
                            throw invalid_argument("Invalid input argument. Please provide a number.");
                    }
                    else {
                        throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
                    }
                }


                if(!b)keyVec.push_back(key);


                ++counter;
            }
            else {
                throw invalid_argument("invalid ID " + str + ". Please provide an valid ID.");
            }
        }

        for (int k : keyVec) {
            Update(R.data[k]);
        }

        std::cout << "log: update operation(s) successful. " << counter << " cat(s) updated." << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
        exit(0); // exit program 
    }
}

void Delete(int argc, char** argv, Record& R) {
    try{
        if (argc <= 2) { // if no id or arguments given 
            throw invalid_argument("missing ID or argument. See 'cats help'.");
        }

        if (R.data.size() < 1) {
            throw invalid_argument("file is empty, there is nothing to delete.");
        }

        string str = argv[2];
        // if it is an argument and it is --all, process to delete operation
        //isArgument(str)
        if (str == "--all") {
            std::cout << "delete all cat enties? [y/n]" << endl;
            char answer;
            while (std::cin >> answer) {
                if (answer == 'y' || answer == 'Y') {
                    std::cout << "deleted all cats." << endl;
                    R.id = 0;
                    R.data.clear();
                    return;
                }
                else if (answer == 'n' || answer == 'N') {
                    std::cout << "aborting delete operation." << endl;
                    break;
                }

                std::cin.clear();
                std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                std::cout << "Invalid input.  Try again.\n ";
            }
        } else {

            int counter = 0;
            
            //if it is an ID, process to delete ID operation
            for (int i = 2; i < argc; ++i) {
                str = argv[i];
                if (isNumber(str)) {  // check if it is a number
                    int num = stringToInt(str);

                    if(num > R.id) throw invalid_argument("invaild ID " + str + ". Plase provide a valid ID.");

                    // search id, if non is found, throw exception
                    int key = idSearch(num, R.data);
                    if (key == -1) throw invalid_argument("invaild ID " + str + ". Plase provide a valid ID.");

                    // id found, delete it
                    R.data.erase(R.data.begin() + key);
                    std::cout << "log: deleting cat with ID " << num << "..." << endl;
                    ++counter;

                }
                else {
                    throw invalid_argument("invaild ID " + str + ". Plase provide a valid ID.");
                }
            }
            if (R.data.size() == 0) R.id = 0; // reset id if it is empty
            std::cout << "log: delete operation(s) successful. " << counter << " cat(s) deleted." << endl;
        } 

    }catch(const invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
        exit(0); // exit program 
    }
}

void Help() {
    cout << "\n\tusage: cats <command> [<args>]\n\n"
            "\tAvailable commands:\n\n"
            "\tcreate - create and add cats data to the file.\n"
            "\t  options:\n"
               "\t\t--name <name>\n"
               "\t\t   add name to the new cat.\n"
               "\t\t--bread <bread>\n"
               "\t\t   add bread to the new cat.\n"
               "\t\t--age <age>\n"
               "\t\t   add age to the new cat. Only accept positive number.\n\n"
            "\tread - list all cats entry in the file\n\n"
            "\tupdate - edit existing in the file by their corresponding id.\n"
            "\t  options:\n"
                "\t\t<id>\n"
                "\t\t   indexes of cats to update content from.\n"
                "\t\t--name <name>\n"
                "\t\t   update only the name of the cats.\n"
                "\t\t--bread <bread>\n"
                "\t\t   update only the bread of the cats.\n"
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

int main(int argc, char** argv)
{
    bool isExist = true;

    if (argc <= 1) {
        //cout << "argc: " << argc << endl;

        Help();
        cout << "\n\npress any key to continue";
        cin.get();
        return 0;
    }

    string filename = "cats.txt";

    Record R;

    ifstream infile;
    infile.open(filename, ios::in);
    if (infile.is_open()) {
        //std::cout << "Opened !\n";
        
        infile >> R.id;

        int id;
        string name;
        string bread;
        int age;

        while (infile >> id >> name >> bread >> age) {
            cats cat;
            cat.id = id;
            cat.name = name;
            cat.bread = bread;
            cat.age = age;
            R.data.push_back(cat);
        }


    } else {
        //std::cout << "Not Opened !\n";
        isExist = false;
        R.id = 0;
        R.data = vector<cats>();

    }
        
    infile.close();

    try {
        string key = argv[1];
        int n = dictionary.count(key) > 0 ? dictionary[key] : -1;

        switch (n) {
            case 1: { // create
                Create(argc, argv, R);
                break;
            }
            case 2: { // read
                Read(R);
                break;
            }
            case 3: {
                Update(argc, argv, R);
                break;
            }
            case 4: {
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
    }catch (const invalid_argument& e) {
        cerr << "error: " << e.what() << endl;
        return 0;
        // do stuff with exception... 
    }

    if (!isExist) {
        cout << "'cats.txt' not detected. Creating a new one at the current directory..." << endl;
    }

    ofstream outfile;
    outfile.open(filename, ios::out);
    outfile << R.id << endl;
    if (outfile.is_open()) {
        for (cats c : R.data) {
            outfile << c.id << " " << c.name << " " << c.bread << " " << c.age << endl;
        }
    }
    else {
        cout << "failed to create cats.txt at the current directory!" << endl;
    }

    outfile.close();


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
