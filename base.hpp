struct base
{
	base(int i, float b)
	: _i(i)
	, _b(b)
	{}

	base(int i)
	: _i(i)
	{}

	virtual ~base() {};

	int _i;
	float _b;
};