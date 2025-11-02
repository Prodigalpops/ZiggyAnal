#pragma once
#include "SourceData.h"
#include "TickData.h"
#include <vector>
#include <math.h>
#include "bar.h"

using namespace std;

class Indicators
{
private:
	vector <SourceData> *SRC;

public:
	Indicators(vector <SourceData>* _SRC, int _maVal);	// , int _maVal, int _maType);
	~Indicators();

//	vector<float> atrLine;
	vector<float> mdLine;
	vector<float> sigLine;
	vector<float> histogram;

	vector<float> maLine;

	vector<float> maLineB;
	vector<float> maLineA;
	vector<float> regressU;
	vector<float> regressD;


private:
	vector <float> source;	//shared by ma and zl, set in ma = close prices
	
	vector <float> srcB;
	vector <float> srcA;
	vector <float> srcBL;
	vector <float> srcAL;
	vector <float> srcBH;
	vector <float> srcAH;

	vector <float> trange;
	vector <float> internal;
	vector <float> hma1;

	vector <float> MA1;

	vector <float> maInternal;
	
	vector <float> rgmaInternalB;
	vector <float> rgmaInternalA;

	vector <float> EMAS;
	vector <float> EMAS1;
	vector <float> EMAL;
	vector <float> EMAL1;
	vector <float> MVAI;
	vector <float> MVAI1;

	vector <float> zlInternal1;
	vector <float> zlInternal2;
	vector <float> zlInternal3;
	vector <float> zlInternal4;
	vector <float> zlInternal5;
	vector <float> zlInternal6;

	vector<float> impHi;
	vector<float> impLo;
	vector<float> impHLC;
	vector<float> impEMA1;
	vector<float> impEMA2;
	vector<float> impMDI;
	vector<float> impSignal;
	vector<float> impMacd;
	vector <float> impInternal1;
	vector <float> impInternal2;

	//vector<float> rgInternal;
	vector<float> regress_u;
	vector<float> regress_d;

	vector<float> RL;
	vector<float> rgMa;

	int sqrtPeriod;
	int sqrtPeriod1;

	//vectors for tick based data
	//vector<float> trangeTick;
	//vector <float> internalTick;
	//vector <float> hma1Tick;

//	vector<float>mdTick;
//	vector<float>sgTick;

	//vector<float> MATick;

	//float atrTick;
//	float maTick;


	vector<bar>* currentBar;

	int atrVal = 0;
	int maVal = 0;

	int rgmaVal= 0;	//for regression


	int MAType = 3;	//for moving average
	int matype = 3;	//for atr
	int matypeR = 1;	//for regression

	float k = 0.0f;	//for atr
	float k1 = 0.0f;	//for ma
	float kR = 0.0f;	//for regression



	float zk1 = 0.0f; //for zl
	float zk2 = 0.0f; //for zl
	float zk3 = 0.0f; //for zl

	int iMaLen = 0;
	int iSigLen = 0;
	float  imi = 0.0;
	float impzk1 = 0.0f; //for zl
	float impzk2 = 0.0f; //for zl




public:
//	void preloadATR();
//	void preloadImpMacd();
//	void preloadZlMacd();
	void preloadMA();
	void preloadReg(int ba);

private:
	float getTrueRange(int period);
//	float getTrueRangeTick(int period);
//	float getMedian(int period);
//	float getMedianTick(int period);

	float avg(float a, float b);
	float average(vector<float>* _tr, int from, int to);
	float lwma(vector<float>* _tr, int from, int to);
	float mva(vector<float>* _tr, int from, int to);
	float smma(vector<float>* _tr, int from, int to);
	float ema(vector<float>* _tr, int from, int to, vector<float>* _int, int _first, float _k);

	float highest(vector<float>* source, int length);
	float lowest(vector<float>* source, int length);
	float linreg(vector<float>& source, int length, int offset);

//	void zlMacd(vector<float>* _tr, int from, int to);
//	void MA(vector<float>* _tr, int from, int to);


public:
//	void prepATR(int period);
	
//	void updateATR(int period, vector<bar>* _currentBar);

//	void clearATRTick();

//	void prepZLMacd(int period);

//	void updateZLMacd(int period, vector<bar>* _currentBar);

//	void clearZLMacdTick();

//	void prepMA(int period);

//	void updateMA(int period, vector<bar>* _currentBar);

//	void clearMATick();

};

