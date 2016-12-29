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
		void*				handler;
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
				std::cerr << "(modular) Could not open path: " << path << std::endl;
				std::abort();
			}

			void* ctor = dlsym(handler, base::constructor);
			if (dlerror() != nullptr)
			{
				std::cerr << "(modular) Could not read symbol: " << path << ":" << base::constructor << std::endl;
				dlclose(handler);
				std::abort();
			}

			std::function<base*()> ptr = reinterpret_cast<base*(*)()>(ctor);
			modules.insert(std::pair<std::string, M>(path, M(ptr, handler)));
		}

		// Create a new object from a mapped library
		std::unique_ptr<base> create (const std::string& path)
		{
			auto it = modules.find(path);
			return std::unique_ptr<base>(it->second.function());
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
					std::cout << "(modular) Could not close library: " << m.first << std::endl;
				}
			}
		};

	// Disabled functions
	public:
		modular (const modular&)		= delete;
		void operator= (const modular&)	= delete;

	private:
		static std::map<std::string, M>	modules;
};

template<typename base>
std::map<std::string, module<base>>	modular<base>::modules;
