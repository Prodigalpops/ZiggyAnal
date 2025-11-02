#include "TickData.h"


TickData::TickData(double dt, float b, float a)
{
	this->dtime = dt;
	this->bid = b;
	this->ask = a;
}

TickData::~TickData()
{
}