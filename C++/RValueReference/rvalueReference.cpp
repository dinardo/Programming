// ##################################################################
// # Simple example to explain how to use std::forward              #
// # http://thbecker.net/articles/rvalue_references/section_01.html #
// ##################################################################

/*
c++ -std=c++14 -o rvalueReference rvalueReference.cpp
*/

#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
auto myFun(vector<T>&& vec)
// ##################################################
// # Inside the function vec is an rvalue reference #
// ##################################################
{
  for(auto& v: vec)
    cout << "Element: " << v << endl;

  vec.push_back(10);

  return vec;
}

int main()
{
  vector<int> vec = {0,1,2,3};

  vec = myFun<int>({3,2,1,0});
  cout << endl;
  vec = myFun<int>(forward< vector<int>&& >(vec));
  cout << endl;
  for(auto& v: vec)
    cout << "Element: " << v << endl;

  return 0;
}
