#pragma once

#ifndef CRUD_H
#define CRUD_H

#include <string>
#include <vector>

struct cats {
    int id;
    std::string name;
    std::string bread;
    int age;
};

struct Record {
    int id;
    std::vector<cats> data;

    Record() : id(0), data(std::vector<cats>()) {}
};

struct ArgumentFlag {
    bool name;
    bool bread;
    bool age;

    ArgumentFlag() : name(false), bread(false), age(false) {}

};

void Create(int argc, char** argv, Record& R);
void Read(const Record& R);
void Update(int argc, char** argv, Record& R);
void Delete(int argc, char** argv, Record& R);

#endif






