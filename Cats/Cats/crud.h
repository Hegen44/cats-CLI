#pragma once

#ifndef CRUD_H
#define CRUD_H

#include <string>
#include <vector>

/**
*Miximum number of argument count to be recognize as an effective command
* @showinitializer 
 */
const int min_argc = 2;

/**
 * Data resources cats
 */
struct cats {
    int id;
    std::string name;
    std::string breed;
    int age;

    cats():id(-1), name(""), breed(""), age(-1) {}
};

/**
 * Contiaineer for all cats data and the current highest cats ID
 */
struct Record {
    int id;
    std::vector<cats> data;

    Record() : id(0), data(std::vector<cats>()) {}
};

void Create(int argc, char** argv, Record& R);
void Read(const Record& R);
void Update(int argc, char** argv, Record& R);
void Delete(int argc, char** argv, Record& R);

#endif






