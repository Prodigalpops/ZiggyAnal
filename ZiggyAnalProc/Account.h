#pragma once
#include <string>
using namespace std;

class Account
{
public:
	Account();
	~Account();

	double key = 0.0;
	int Date = 0;
	float Balance = 50000.0f;

	int BuySell = 0;
	int lots = 0;
	float openPrice = 0.0f;
	float stop = 0.0f;
	float limit = 0.0f;
	float trigger = 0.0f;
	float commission = 0.0f;

	float maxBalance = 50000.0f;
	float minBalance = 50000.0f;
	float maxMinPct = 0.0f;
	double _PL=0.0;

	//Used parameters 
/*	int MAPeriod = 12;
	int MAType = 3;
	float trigRate = 0.5f;
	float fibEntry = 0.6f;
	float fibTgt = 0.5f;
	int fixTgt = 20;
*/



//	float regDif = 1.5f;
//	float regDif2 = 2.0f;
//	int regLinelen = 4;
	int regLinelimit = 20;
//	int regLinema = 1;
	float rVal = 1.0f;
	int regLinemalen = 20;
	int aggVal = 0;

//	int ma = 20;
//	int flim = 20;
//	int agg = 0;

	float breakEven = 0.15f;	//point where stop moved to after it breaches 1R


};





