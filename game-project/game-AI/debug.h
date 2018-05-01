// functions for debug
// by cysun

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <vector>
#include <string>

using namespace std;

void waitKey();

template <class vType>
void printVector(std::vector<vType> v, string name);

template <class vType>
void printPointerVector(std::vector<vType> v, string name);

#endif
