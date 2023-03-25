#include <iostream>
#include "basic_string.h"
#include <string>

int main(){
	std::basic_string<char> one("abcdefg");
	std::basic_string<char> two(one,1,3);
	tstl::basic_string<char> three("abcs");
	tstl::basic_string<char> four(three.begin(),three.end());
    std::cout<<four<<std::endl;
}
