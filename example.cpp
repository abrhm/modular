#include <iostream>
#include "base.hpp"

class example : public base {
public:
	example (int i, float b)
	: base(i, b)
	{}

	virtual ~example () final
	{
		std::cout << "dtor" << std::endl;
	}
};

extern "C" {
	base* St8functionIFP4baseifEE(int i, float b)
	{
		return new example(i, b);
	}
}
