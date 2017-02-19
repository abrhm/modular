#pragma once
#include "modular.hpp"

using namespace modular::interface;

class params
{};

class extra_params
{};

class base : public modular_base , public params
{
public:
	base () {};

	using P = params;
	using EP = extra_params;

	base (P&& other)
	: params(other)
	{}

	// Keep this virtual to call the proper destructor
	virtual ~base () {};

	virtual void test () const = 0;

	static constexpr char _move[] = "create";
};
constexpr char base::_move[];