#pragma once
#ifndef HELPER_H
#define HELPER_H

#include <string>
#include "crud.h"

bool IsArgument(const std::string& str);
int ConverToNumber(const char* str);
int IDSearch(const int id, const std::vector<cats> vec);
void Help();

#endif
