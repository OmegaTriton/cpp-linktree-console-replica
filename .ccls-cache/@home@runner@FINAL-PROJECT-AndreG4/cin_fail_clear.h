#pragma once
#include <iostream>
#include <limits>
using namespace std;

extern "C" void cin_fail_clear(){
  cin.clear();
  cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}