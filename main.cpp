#include <algorithm>
#include <memory>
#include <vector>
#include "base.hpp"
#include "modular.hpp"

int main(int argc, char* argv[])
{

	std::vector<std::unique_ptr<base>> extensions;
	extensions.reserve(std::size_t(argc-1));

	for (int i = 1; i < argc; ++i)
	{
		extensions.push_back(modular::create<base>(argv[i], params(), extra_params()));
	}

	std::for_each(extensions.begin(), extensions.end(), [](const std::unique_ptr<base>& ext){ ext->test(); });

	return 0;
}