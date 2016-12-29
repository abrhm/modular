#include <iostream>
#include "base.hpp"

class example2 : public base {
public:
	example2 ()
	{
		std::cout << "example2 constructor" << std::endl;
	}

	virtual ~example2 ()
	{
		std::cout << "example2 destructor" << std::endl;
	}

	virtual void init () const
	{
		std::cout << "example2 init" << std::endl;
	}
};

extern "C" {
	base* create() {
		std::cout << "Create new example2" << std::endl;
		return new example2;
	}
}
