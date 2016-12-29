#pragma once

class base
{
public:
	base () {};
	virtual ~base () {};
	virtual void init() const = 0;
	static constexpr char constructor[] = "create";
};
// Definition
constexpr char base::constructor[];