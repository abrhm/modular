#include <iostream>
#include "base.hpp"
#include "modular.hpp"

int main(int argc, char* argv[])
{
	std::cout << argv[1] << std::endl;
	std::unique_ptr<base> b1 = create<base, int, float>(argv[1], 5, 6.0);
	std::cout << b1->_i << std::endl;
	std::cout << b1->_b << std::endl;
	return 0;
}