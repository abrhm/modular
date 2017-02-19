#pragma once
#include <cassert>
#include <dlfcn.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <map>
#include <utility>


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
				assert(dlerror() == nullptr);
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
				func = functions.emplace(path, reinterpret_cast<I*(*)()>(dlsym(module_loader::get(path).get(), I::_default))).first;
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
				func = functions.emplace(path, reinterpret_cast<I*(*)(const P&, const EP&)>(dlsym(module_loader::get(path).get(), I::_copy))).first;
			}
			return std::unique_ptr<I>(func->second(p, ep));
		}
	private:
		static std::map<std::string, std::function<I*(const P&, const EP&)>> functions;
	};
	template<class I, typename P, typename EP>
	std::map<std::string, std::function<I*(const P&, const EP&)>> _copy<I, P, EP>::functions;


	template<class I, typename P, typename EP>
	struct _copy_move
	{
		static std::unique_ptr<I> create (const std::string& path, const P& p, EP&& ep)
		{
			auto func = functions.find(path);
			if (func == functions.end())
			{
				func = functions.emplace(path, reinterpret_cast<I*(*)(const P&, EP&&)>(dlsym(module_loader::get(path).get(), I::_copy_move))).first;
			}
			return std::unique_ptr<I>(func->second(p, std::move(ep)));
		}
	private:
		static std::map<std::string, std::function<I*(const P&, EP&&)>> functions;
	};
	template<class I, typename P, typename EP>
	std::map<std::string, std::function<I*(const P&, EP&&)>> _copy_move<I, P, EP>::functions;


	template<class I, typename P, typename EP>
	struct _move
	{
		static std::unique_ptr<I> create (const std::string& path, P&& p, EP&& ep)
		{
			auto func = functions.find(path);
			if (func == functions.end())
			{
				func = functions.emplace(path, reinterpret_cast<I*(*)(P&&, EP&&)>(dlsym(module_loader::get(path).get(), I::_move))).first;
			}
			return std::unique_ptr<I>(func->second(std::move(p), std::move(ep)));
		}
	private:
		static std::map<std::string, std::function<I*(P&&, EP&&)>> functions;
	};
	template<class I, typename P, typename EP>
	std::map<std::string, std::function<I*(P&&, EP&&)>> _move<I, P, EP>::functions;
}


namespace modular
{
	namespace interface
	{
		constexpr char disable[] = "";

		struct modular_base
		{
			static constexpr char _default[] = "";
			static constexpr char _copy[] = "";
			static constexpr char _copy_move[] = "";
			static constexpr char _move[] = "";
		};
		constexpr char modular_base::_default[];
		constexpr char modular_base::_copy[];
		constexpr char modular_base::_copy_move[];
		constexpr char modular_base::_move[];
	}


	template<class I, typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, typename std::enable_if<!std::is_same<decltype(I::_default), decltype(interface::disable)>::value, dummy>::type * = 0)
	{
		return _default<I>::create(path);
	}

	template<class I,
	typename P = void,
	typename EP = void,
	typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, const P& p, const EP& ep, typename std::enable_if<!std::is_same<decltype(I::_copy), decltype(interface::disable)>::value, dummy>::type * = 0)
	{
		return _copy<I, P, EP>::create(path, p, ep);
	}

	template<class I,
	typename P = void,
	typename EP = void,
	typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, const P& p, EP&& ep, typename std::enable_if<!std::is_same<decltype(I::_copy_move), decltype(interface::disable)>::value, dummy>::type * = 0)
	{
		return _copy_move<I, P, EP>::create(path, p, std::move(ep));
	}

	template<class I,
	typename P = void,
	typename EP = void,
	typename dummy = char>
	std::unique_ptr<I> create (const std::string& path, P&& p, EP&& ep, typename std::enable_if<!std::is_same<decltype(I::_move), decltype(interface::disable)>::value, dummy>::type * = 0)
	{
		return _move<I, P, EP>::create(path, std::move(p), std::move(ep));
	}
}
