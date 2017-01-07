#include <algorithm>
#include <memory>
#include <vector>
#include "base.hpp"
#include "modular.hpp"

int main(int argc, char* argv[])
{
	// Cache reference to a singleton
	modular<base>& modules = modular<base>::get_instance();

	// Reserve enough space for the extensions
	std::vector<std::unique_ptr<base>> extensions;
	extensions.reserve(argc - 1);

	// Load and instantiate the extensions
	for (int i = 1; i < argc; ++i)
	{
		modules.load(argv[i]);
		extensions.push_back(modules.create(argv[i]));
	}

	// Call the init method for each extension
	std::for_each(extensions.begin(), extensions.end(), [](const std::unique_ptr<base>& ext){ ext->init(); });

	return 0;
}