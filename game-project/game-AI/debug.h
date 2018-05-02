// functions for debug
// by cysun

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <vector>
#include <string>

using namespace std;

inline void waitKey() {
  cout << "\nPress any key to continue ...";
  cin.get();
  cout << endl;
}

template <class vType>
inline void printVector(std::vector<vType> v, string name) {
  cout << "\n -- " << name << " -- \n" << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << name << "[" << i << "] = " << v[i] << endl;
  }
  cout << endl;
}

template <class vType>
inline void printPointerVector(std::vector<vType> v, string name) {
  cout << "\n -- " << name << " -- \n" << endl;
  for (size_t i = 0; i < v.size(); i++) {
    cout << name << "[" << i << "] = " << *v[i] << endl;
  }
  cout << endl;
}

#endif
