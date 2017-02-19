#include <iostream>
#include <utility>
#include "base.hpp"

using EP = base::EP;

class example2 : public base , public EP {
public:
	example2 (EP&& ep)
	: EP(ep)
	{
		std::cout << "example2 constructor" << std::endl;
	}

	virtual ~example2 () final
	{
		std::cout << "example2 destructor" << std::endl;
	}

	virtual void test () const final
	{
		std::cout << "example2 test" << std::endl;
	}
};

extern "C" {
	base* create(EP&& ep) {
		return new example2(std::move(ep));
	}
}
