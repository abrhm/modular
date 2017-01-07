#pragma once
// TODO only include iostream/write errors in debug
#include <csignal>
#include <dlfcn.h>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>

constexpr int LOAD_MODE = RTLD_NOW;

// This class should be unreachable from outside
namespace
{
	template<class base>
	struct module
	{
		module (std::function<base*()> f, void* h)
		: function(f)
		, handler(h)
		{}

		std::function<base*()> function;
		void* handler;
	};
};

template<class base>
class modular
{
	using M = module<base>;

public:
	// Get the singleton instance
	static modular<base>& get_instance()
	{
		static modular<base> singleton_instance;
		return singleton_instance;
	}

	// Load a library with a given path
	void load (const std::string& path)
	{
		void* handler = dlopen(path.c_str(), LOAD_MODE);
		if (!handler)
		{
			std::cerr << "(modular) Could not open path: " << path << '\n';
			std::abort();
		}

		void* ctor = dlsym(handler, base::constructor);
		if (dlerror() != nullptr)
		{
			std::cerr << "(modular) Could not read symbol: " << path << ":" << base::constructor << '\n';
			dlclose(handler);
			std::abort();
		}

		std::function<base*()> ptr = reinterpret_cast<base*(*)()>(ctor);
		modules.insert(std::pair<std::string, M>(path, M(ptr, handler)));
	}

	// Create a new object from a mapped library
	std::unique_ptr<base> create (const std::string& path) const
	{
		auto it = modules.find(path);
		std::unique_ptr<base> module_instance(it->second.function());
		// Check version and abort in case of version mismatch
		if (module_instance->version() >= base::version_min && module_instance->version() <= base::version_max)
		{
			return module_instance;
		}
		else
		{
			std::cerr	<< "(modular) Module version mismatch:\n"
						<< "Supported version range: " << base::version_min << " - " << base::version_max << '\n'
						<< path << " version: " << module_instance->version() << '\n';
			std::abort();
		}
	}

private:
	modular (){};
	~modular ()
	{
		for (auto& m : modules)
		{
			int error = dlclose(m.second.handler);
			if (error)
			{
				std::cout << "(modular) Could not close library: " << m.first << '\n';
			}
		}
	};

	// Disable copy
	modular (const modular&) = delete;
	modular& operator= (const modular&) = delete;
	// Disable move
	modular (const modular&&) = delete;
	modular& operator= (modular&&) = delete;

	static std::map<std::string, M> modules;
};

template<typename base>
std::map<std::string, module<base>> modular<base>::modules;
