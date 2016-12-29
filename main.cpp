#include <string>
#include "base.hpp"
#include "modular.hpp"

int main(int argc, char* argv[])
{
	// Shorten the call
	const auto& modules = modular<base>::get_instance;

	std::string e1(argv[1]);
	std::string e2(argv[2]);

	modules().load(e1);
	modules().load(e2);

	std::unique_ptr<base> example1 = modules().create(e1);
	std::unique_ptr<base> example2 = modules().create(e2);

	example1->init();
	example2->init();

	return 0;
}