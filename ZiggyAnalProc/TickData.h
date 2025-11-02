#pragma once

class TickData
{
public:
	TickData(double dt, float b, float a);
	~TickData();

public:
	double dtime;
	float bid;
	float ask;
};
