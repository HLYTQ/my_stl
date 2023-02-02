#include <iostream>

size_t len(const char* value)
{
  size_t len = 0;
  for(;*value != char(0); value++)
    len++;
  return len;
}

int main(int argc , char* argv[]){
  Enity<int> m(123);
  std::cout<<*m.begin()<<std::endl;
  return 0;
}