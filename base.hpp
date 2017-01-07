#pragma once

class base
{
public:
	base () {};
	// Keep this virtual to call the proper destructor
	virtual ~base () {};
	virtual void init() const = 0;
	// Get the inherited class version
	virtual inline unsigned int version() const = 0;

	// Modular will look for this library symbol in the inherited classes
	static constexpr char constructor[] = "create";

	// Versioning
	static constexpr unsigned int version_min = 1;
	static constexpr unsigned int version_max = 1;
};
// Definition
constexpr char base::constructor[];