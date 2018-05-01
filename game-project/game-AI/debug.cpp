// functions for debug
// by cysun


#include <iostream>
#include <vector>
#include <string>
#include "debug.h"

using namespace std;

void waitKey() {
  cout << "\nPress any key to continue ...";
  cin.get();
  cout << endl;
}

template <class vType>
void printVector(std::vector<vType> v, string name) {
  cout << "\n -- " << name << " -- \n" << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << name << "[" << i << "] = " << v[i] << endl;
  }
  cout << endl;
}

template <class vType>
void printPointerVector(std::vector<vType> v, string name) {
  cout << "\n -- " << name << " -- \n" << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << name << "[" << i << "] = " << *v[i] << endl;
  }
  cout << endl;
}
