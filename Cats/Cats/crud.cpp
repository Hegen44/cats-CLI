#include <iostream>

#include <string>
#include <vector>

#include "helper.h"
#include "crud.h"
using namespace std;


cats SetCatsInformation(cats myCat, const ArgumentFlag flag) {

    if (!flag.name) {
        cout << "\nName: ";
        cin >> myCat.name;
    }
    if (!flag.bread) {
        cout << "\nBread: ";
        cin >> myCat.bread;
    }
    if (!flag.age) {
        cout << "\nAge: ";
        while (!(cin >> myCat.age) || myCat.age < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.  Try again.\nAge:  ";
        }
    }

    return myCat;

}

void HandleCreateOperation(Record& R, cats myCat, const ArgumentFlag flag) {
    cout << "\n log: creating cat..." << endl << endl;

    myCat = SetCatsInformation(myCat, flag);
    myCat.id = ++R.id;
    R.data.push_back(myCat);

    cout << "\n log: cat created" << endl << endl;
    cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tBread: " << myCat.bread << "\tAge: " << myCat.age << endl;
}

void Create(int argc, char** argv, Record& R) {

    cats myCat;
    ArgumentFlag flag;

    if (argc <= 2) { // if no arguments given, direct to normal create
        HandleCreateOperation(R, myCat, flag);
    }
    else {
        for (int i = 2; i < argc; ++i) {
            string str = argv[i];
            if (str == "--name") {
                if (flag.name)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    //++i;
                    myCat.name = argv[i];
                    flag.name = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a name.");
                }
            }
            else if (str == "--bread") {
                if (flag.bread)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    //++i;
                    myCat.bread = argv[i];
                    flag.bread = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a bread type.");
                }

            }
            else if (str == "--age") {
                if (flag.age)  throw invalid_argument("duplicated argument.");
                if (++i < argc) {
                    int a = converToNumber(argv[i]);
                    if (a == -1) throw invalid_argument("invalid input argument. Please provide a positive number.");
                    myCat.age = a;
                    flag.age = true;
                }
                else {
                    throw invalid_argument("missing valid input argument. Please provide a name.");
                }

            }
            else {
                throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
            }
        }

        HandleCreateOperation(R, myCat, flag);

    }
}

void Read(const Record& R) {
    for (cats myCat : R.data) {
        cout << "\tUnique ID: " << myCat.id << "\tName: " << myCat.name << "\tBread: " << myCat.bread << "\tAge: " << myCat.age << endl;
    }
}

void Update(int argc, char** argv, Record& R) {


    int counter = 0;

    vector<int> keyVec;

    for (int i = 2; i < argc; ++i) {
        string str = argv[i];
        int num = converToNumber(argv[i]);
        if (num < 0 || num > R.id) throw invalid_argument("invalid ID '" + str + "'. Please provide an valid ID.");

        int key = idSearch(num, R.data);
        // if no key is found, throw exception
        if (key == -1) throw invalid_argument("invaild ID '" + str + "'. Plase provide a valid ID.");

        bool b = false;

        while (i + 1 < argc && isArgument(argv[i + 1])) {
            b = true;
            str = argv[++i];

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
                int a = converToNumber(argv[i]);
                if (a == -1) throw invalid_argument("invalid input argument '" + (string)argv[i] + "'. Please provide a positive number.");
                R.data[key].age = a;
            }
            else {
                throw invalid_argument("'" + str + "' is a invalid argument. See 'cats help'.");
            }
        }

        if (!b)keyVec.push_back(key);

        ++counter;
    }

    ArgumentFlag flag;
    for (int k : keyVec) {
        cats myCat = R.data[k];
        cout << "\n log: updating cat with ID " << myCat.id << "..." << endl;
        myCat = SetCatsInformation(myCat, flag);
        R.data[k] = myCat;
    }

    std::cout << "log: update operation(s) successful. " << counter << " cat(s) updated." << endl;
}

void Delete(int argc, char** argv, Record& R) {

    string str = argv[2];
    // if it is an argument and it is --all, process to delete operation
    //isArgument(str)
    if (str == "--all") {
        std::cout << "delete all cat enties? [y/n]" << endl;
        char answer;
        while (std::cin >> answer) {
            if (answer == 'y' || answer == 'Y') {
                std::cout << "deleted all cats." << endl;
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
    }
    else {

        int counter = 0;

        //if it is an ID, process to delete ID operation
        for (int i = 2; i < argc; ++i) {

            int num = converToNumber(argv[i]);

            if (num > R.id || num < 0) throw invalid_argument("invaild ID '" + (string)argv[i] + "'. Plase provide a valid ID.");

            // search id, if non is found, throw exception
            int key = idSearch(num, R.data);
            if (key == -1) throw invalid_argument("ID '" + (string)argv[i] + "' not found'. Plase provide a valid ID.");

            // id found, delete it
            R.data.erase(R.data.begin() + key);
            std::cout << "log: deleting cat with ID " << num << "..." << endl;
            ++counter;


        }
        std::cout << "log: delete operation(s) successful. " << counter << " cat(s) deleted." << endl;
    }


}
