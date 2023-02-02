#include <iostream>
#include "vector.h"
#include <vector>

int main(int argc , char* argv[]){
  tstl::vector<int> nums = {1,2,3};
  nums.erase(nums.begin() + 1);
  for(int x:nums){
    std::cout<<x<<std::endl;
  }
  return 0;
}