#pragma once
#ifndef HELPER_H
#define HELPER_H

#include <string>
#include "crud.h"

bool isArgument(const std::string& str);
int converToNumber(const char* str);
int idSearch(const int id, const std::vector<cats> vec);
void Help();

#endif
