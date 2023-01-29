#include<iostream>
#include "vector.h"
#include<string>
#include<vector>
using namespace tstl;


int main(){
  vector<std::string> one;
  for(int i = 0; i < 10; i++)
  {
      one.emplace_back("I am the storm that is approaching");
  }
  
  for(auto it:one)
    std::cout<< it << std::endl;

  return 0;
}

