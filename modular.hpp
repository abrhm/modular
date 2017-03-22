#pragma once
#include <cassert>
#include <string>
#include <dlfcn.h>
#include <memory>
#include <map>
#include <functional>
#include <typeinfo>
#include <iostream>

namespace
{
	struct module_loader
	{
		using module_handler = std::unique_ptr<void, std::function<void(void*)>>;

		static module_handler& load (const std::string& path)
		{
			auto handler = modules.find(path);
			if (handler == modules.end())
			{
				handler = modules.emplace(
					path,
					module_handler(dlopen(path.c_str(), LOAD_MODE), dl_close)
				).first;
				assert(dlerror() == nullptr);
			}
			return handler->second;
		}
		private:
			static constexpr int LOAD_MODE = RTLD_NOW;

			static void dl_close (void* handler)
			{
				dlclose(handler);
			}

			static std::map<std::string, module_handler> modules;
	};
	std::map<std::string, module_loader::module_handler> module_loader::modules;

	template<class B, typename... Args>
	struct function_loader
	{
		using function_type = std::function<B*(Args...)>;

		static std::unique_ptr<B> create (const std::string& path, Args... params)
		{
			std::cout << typeid(function_type).name() << std::endl;

			auto func = functions.find(path);
			if (func == functions.end())
			{
				func = functions.emplace(
					path,
					reinterpret_cast<B*(*)(Args...)>(
						dlsym(module_loader::load(path).get(), typeid(function_type).name())
					)
				).first;
			}

			return std::unique_ptr<B>(func->second(params...));
		}
	private:
		static std::map<std::string, function_type> functions;
	};

	template<class B, typename... Args>
	std::map<std::string, std::function<B*(Args...)>> function_loader<B, Args...>::functions;
}


template<class B, typename... Args>
std::unique_ptr<B> create (const std::string& path, Args... params)
{
	return function_loader<B, Args...>::create(path, params...);
}