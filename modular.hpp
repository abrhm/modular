#pragma once
#include <dlfcn.h>
#include <functional>
#include <map>
#include <memory>
#include <string>


namespace
{
	struct module_loader
	{
		using module_handler = std::unique_ptr<void, std::function<void(void*)>>;

		static module_handler& get (const std::string& path)
		{
			auto handler = modules.find(path);
			if (handler == modules.end())
			{
				handler = modules.emplace(path, module_handler(dlopen(path.c_str(), LOAD_MODE), dl_close)).first;
			}
			return handler->second;
		}

	private:
		// dlopen load mode
		static constexpr int LOAD_MODE = RTLD_NOW;

		static void dl_close (void* handler)
		{
			dlclose(handler);
		}

		static std::map<std::string, module_handler>  modules;
	};
	std::map<std::string, module_loader::module_handler> module_loader::modules;


	template<class I>
	struct _default
	{
		static std::unique_ptr<I> create (const std::string& path)
		{
			auto func = functions.find(path);
			if (func == functions.end())
			{
				func = functions.emplace(path, reinterpret_cast<I*(*)()>(dlsym(module_loader::get(path).get(), I::default_param))).first;
			}
			return std::unique_ptr<I>(func->second());
		}
	private:
		static std::map<std::string, std::function<I*()>> functions;
	};
	template<class I>
	std::map<std::string, std::function<I*()>> _default<I>::functions;


	template<class I, typename P, typename EP>
	struct _copy
	{
		static std::unique_ptr<I> create (const std::string& path, const P& p, const EP& ep)
		{
			auto func = functions.find(path);
			if (func == functions.end())
			{
				func = functions.emplace(path, reinterpret_cast<I*(*)(const P&, const EP&)>(dlsym(module_loader::get(path).get(), I::default_param))).first;
			}
			return std::unique_ptr<I>(func->second(p, ep));
		}
	private:
		static std::map<std::string, std::function<I*(const P&, const EP&)>> functions;
	};
	template<class I, typename P, typename EP>
	std::map<std::string, std::function<I*(const P&, const EP&)>> _copy<I, P, EP>::functions;
}


namespace modular
{
	// struct interface
	// {};

	template<class I, typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, typename std::enable_if<I::enable_default, dummy>::type * = 0)
	{
		return _default<I>::create(path);
	}

	template<class I,
	typename P = void,
	typename EP = void,
	typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, const P& p, const EP& ep, typename std::enable_if<I::enable_copy, dummy>::type * = 0)
	{
		return _copy<I, P, EP>::create(path, p, ep);
	}
}
