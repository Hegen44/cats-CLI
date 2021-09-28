// crud.cpp : This file contains the CRUD commands function.
//
#include <iostream>

#include <string>
#include <vector>

#include "helper.h"
#include "crud.h"
using namespace std;

/**
 * Flags use to keep track of activated argument
 */
struct ArgumentFlag {
    bool name;
    bool breed;
    bool age;
    ArgumentFlag() : name(false), breed(false), age(false) {}
};

/**
    Ask for user input to set the data into select cat.
    base on the psssed in flag, some operation will be skipped.
    it is a helper function for both crette and update command.

    @relatesalso Create, Update

    @param myCat cats data that is already either empty or partly filled in base on command argument
    @param flag to keep track of activated arguments
    @return cat with all information filled except the id
*/
cats SetCatsInformation(cats myCat, const ArgumentFlag flag) {

    // ask for 
    if (!flag.name) {
        cout << "Name: ";
        cin >> myCat.name;
        cout << endl;
    }
    if (!flag.breed) {
        cout << "breed: ";
        cin >> myCat.breed;
        cout << endl;
    }
    if (!flag.age) {
        cout << "Age: ";
        while (!(cin >> myCat.age) || myCat.age < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.  Try again.\nAge:  ";
        }
        cout << endl;
    }

    return myCat;

}

/**
    output log message to the user the create operation is ongoing. This funcion
    also insert the now data filled cats into the data contineer

    @param R record containing the containeers of cats
    @param myCat cats data that is already either empty or partly filled in base on command argument
    @param flag to keep track of activated arguments
*/
void CreateOperation(Record& R, cats myCat, const ArgumentFlag flag) {
    cout << "\nlog: creating cat..." << endl << endl;

    myCat = SetCatsInformation(myCat, flag);
    myCat.id = ++R.id;
    R.data.push_back(myCat);

    cout << "log: cat created" << endl << endl;
    cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tbreed: " << myCat.breed << "\tAge: " << myCat.age << endl;
}

/**
    Create and insert cats into the data container.

    @param argc command argument counts
    @param argv command argument vector
    @param R record containing the containeers of cats
*/
void Create(int argc, char** argv, Record& R) {

    cats myCat;             // a new empty cat
    ArgumentFlag flag;      // flag being use to keep track of the arguments\

    if (argc <= min_argc) { // if no arguments given, direct to normal create
        CreateOperation(R, myCat, flag);
    }
    else {

        /*---------------------------------------------------------
         *  Loop through the argument vector to decconstruct the command 
         *  into their corresponding arguments, and act accordingly.
         *---------------------------------------------------------*/
        for (int i = min_argc; i < argc; ++i) {
            string str = argv[i];
            if (str == "--name") {
                if (flag.name)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    myCat.name = argv[i];
                    flag.name = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a name.");
                }
            }
            
            else if (str == "--breed") {
                if (flag.breed)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    myCat.breed = argv[i];
                    flag.breed = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a breed type.");
                }

            }
            else if (str == "--age") {
                if (flag.age)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    int a = ConverToNumber(argv[i]); // check if it is a valid number 
                    if (a == -1) throw invalid_argument("invalid input argument. Please provide a positive number.");
                    myCat.age = a;
                    flag.age = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a name.");
                }

            } // the argument does not much any availble argument options
            else {
                throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
            }
        }

        // All arguments have been handled, move on to the rest of create operation 
        CreateOperation(R, myCat, flag);

    }
}

/**
    Read and list all the cats in the data container.

    @param R record containing the containeers of cats
*/
void Read(const Record& R) {
    for (cats myCat : R.data) {
        cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tbreed: " << myCat.breed << "\tAge: " << myCat.age << endl;
    }
}

/**
    Handle update command arguments. Prioritize cats with argument.
    cat that require user input to update would have its ID save to a vector 
    and be proceed at the end

    @param argc command argument counts
    @param argv command argument vector
    @param R record containing the containeers of cats
    @param counter number couting successful update operation
    @return list of cats index that has no arguemnts and need user input
*/
vector<int> UpdateHandleArgument(int argc, char** argv, Record& R, int& counter) {
    vector<int> result;

    for (int i = min_argc; i < argc; ++i) {

        int num = ConverToNumber(argv[i]);
        if (num < 0 || num > R.id) throw invalid_argument("invalid ID '" + (string)argv[i] + "'. Please provide an valid ID.");
        int key = IDSearch(num, R.data);
        if (key == -1) throw invalid_argument("invaild ID '" + (string)argv[i] + "'. Plase provide a valid ID."); //  no key is found, throw exception

        bool b = false;

        while (i + 1 < argc && IsArgument(argv[i + 1])) {
            b = true;
            string str = argv[++i];

            if (str == "--name") {
                if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                R.data[key].name = argv[i];
            }
            else if (str == "--breed") {
                if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                R.data[key].breed = argv[i];
            }
            else if (str == "--age") {
                if (++i >= argc) throw invalid_argument("missing valid input argument. Please provide a value.");
                int a = ConverToNumber(argv[i]);
                if (a == -1) throw invalid_argument("invalid input argument '" + (string)argv[i] + "'. Please provide a positive number.");
                R.data[key].age = a;
            }
            else {
                throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
            }
        }

        if (!b)result.push_back(key);

        ++counter;

    }

    return result;
}

/**
    Update cats with its ID match with the arguments ID. 
    It will first handle IDs with argument before moving to update 
    ID that has no argument

    @param argc command argument counts
    @param argv command argument vector
    @param R record containing the containeers of cats
*/
void Update(int argc, char** argv, Record& R) {


    int counter = 0;

    vector<int> keyVec = UpdateHandleArgument(argc, argv, R, counter);

    /*---------------------------------------------------------
    * handle all the ID arugments for no arugments
    * update individually by ask user input for each cats
    * by calling 'SetCatsInformation()' helper function
    *---------------------------------------------------------*/

    ArgumentFlag flag;
    for (int k : keyVec) {
        cats myCat = R.data[k];
        cout << "\nlog: updating cat with ID " << myCat.id << "..." << endl << endl;
        myCat = SetCatsInformation(myCat, flag);
        R.data[k] = myCat;
    }

    std::cout << "log: update operation(s) successful. " << counter << " cat(s) updated." << endl;
}

/**
    Delete all records from the data container. It will ask the user for confirmation 
    before perform the delete operation.

    @param R record containing the containeers of cats
*/
void Delete_all(Record& R) {
    std::cout << "delete all cat enties? [y/n]" << endl;
    char answer;
    while (std::cin >> answer) {
        if (answer == 'y' || answer == 'Y') {
            R.data.clear();
            std::cout << "deleted all cats." << endl;
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
}

/**
    Delete all records from the data container. It will ask the user for confirmation
    before perform the delete operation.

    @param argc command argument counts
    @param argv command argument vector
    @param R record containing the containeers of cats
*/
void Delete(int argc, char** argv, Record& R) {

    string str = argv[min_argc];

    // if it is an argument and it is --all, process to delete operation
    if (str == "--all") {
        Delete_all(R); // to delete all operation
    }
    else {

        //if it is an ID, process to delete each entry individually 
        for (int i = min_argc; i < argc; ++i) {

            int num = ConverToNumber(argv[i]);

            if (num > R.id || num < 0) throw invalid_argument("invaild ID '" + (string)argv[i] + "'. Plase provide a valid ID.");

            // search id, if non is found, throw exception
            int key = IDSearch(num, R.data);
            if (key == -1) throw invalid_argument("ID '" + (string)argv[i] + "' not found'. Plase provide a valid ID.");

            // id found, delete it
            R.data.erase(R.data.begin() + key);
            std::cout << "log: deleting cat with ID " << num << "..." << endl;

        }
        std::cout << "log: delete operation(s) successful. " << (argc - min_argc) << " cat(s) deleted." << endl;
    }


}
