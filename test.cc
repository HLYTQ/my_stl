#include <iostream>

size_t len(const char* value)
{
  size_t len = 0;
  for(;*value != char(0); value++)
    len++;
  return len;
}

int main(int argc , char* argv[]){
  std::cout<< len("Iam")<<std::endl;
  std::cout<<(s1 < s3)<<std::endl;
  return 0;
}