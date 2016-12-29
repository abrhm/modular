#include <iostream>
#include "base.hpp"

class example1 : public base {
public:
	example1 ()
	{
		std::cout << "example1 constructor" << std::endl;
	}

	virtual ~example1 ()
	{
		std::cout << "example1 destructor" << std::endl;
	}

	virtual void init () const
	{
		std::cout << "example1 init" << std::endl;
	}
};

extern "C" {
	base* create() {
		std::cout << "Create new example1" << std::endl;
		return new example1;
	}
}
