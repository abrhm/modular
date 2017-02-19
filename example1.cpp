#include <iostream>
#include <utility>
#include "base.hpp"

using EP = base::EP;

class example1 : public base , public EP {
public:
	example1 (EP&& ep)
	: EP(ep)
	{
		std::cout << "example1 constructor" << std::endl;
	}

	virtual ~example1 () final
	{
		std::cout << "example1 destructor" << std::endl;
	}

	virtual void test () const final
	{
		std::cout << "example1 test" << std::endl;
	}
};

extern "C" {
	base* create(EP&& ep) {
		return new example1(std::move(ep));
	}
}
