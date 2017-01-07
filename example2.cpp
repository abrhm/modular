#include <iostream>
#include <type_traits>
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

	virtual void init () const override
	{
		std::cout << "example2 init" << std::endl;
	}

	virtual inline unsigned int version() const override
	{
		return module_version;
	}

private:
	// Version check with static_assert
	static constexpr unsigned int module_version = 1;
	static_assert(module_version >= base::version_min && module_version <= base::version_max, "Version mismatch");
};

extern "C" {
	base* create() {
		std::cout << "Create new example2" << std::endl;
		return new example2;
	}
}
