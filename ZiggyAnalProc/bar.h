#pragma once

class bar
{
public:
	bar(float _o, float _h, float _l, float _c) { open = _o; high = _h; low = _l; close = _c; }
	~bar() {};

	float open;
	float high;
	float low;
	float close;
};
