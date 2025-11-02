#pragma once
using namespace std;

class History
{
public:
	History();
	~History();
	History(long i) : Key(i) {}

	long Key=0;
	int Period=0;
	int Type=0;
	
	float rate = 0.0f;
	float entry = 0.0f;
	float tgt = 0.0f;
	int ftgt = 0;

//	int Invert=0;
//	int InvVal = 0;
	long Count=0;
	long Bias=0;

	struct Finder 
	{
		Finder(long const& n) : _key(n) { }
		bool operator () (const History& el) const {
			return el.Key == _key;
		}
		long _key;
	};

	struct Matcher
	{
		Matcher(long const& n) : _pair(n) { }
		bool operator () (const History& bm) const {
			return bm.Key == _pair;
		}
		long _pair;
	};

};

class Histdata
{
public:
	Histdata();

	~Histdata();

	float Date=0.0f;
	float Balance = 50000.0f;
	int MAPeriod = 0;
	int MAType = 0;
	float trigRate = 0.0f;
	float fibEntry = 0.0f;
	float fibTgt = 0.0f;
	int fixTgt = 0;

	long Bias = 0;
	bool del = false;

};