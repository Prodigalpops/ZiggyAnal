#pragma once
#include<vector>

class SourceData
{
	public:
	SourceData();
	~SourceData();

public:
	float open = 0.0f;
	float high = 0.0f;
	float low = 0.0f;
	float close = 0.0f;

	float aopen = 0.0f;
	float ahigh = 0.0f;
	float alow = 0.0f;
	float aclose = 0.0f;

//	std::vector<char> mins;
	//This has a dramatic adverse effect on processing time 2 mins to 20+!!!
	//char mins[61];	//4 chars per minute representing the move size, direction and size of the hlc per minute
	int volume=0;
	double date=0.0;
};

class VolSourceData
{
public:
	VolSourceData();
	~VolSourceData();

	int volume=0;
	double date=0.0;
};

class MinuteData
{
public:
	MinuteData();
	~MinuteData();

	unsigned char mins[31];	
};