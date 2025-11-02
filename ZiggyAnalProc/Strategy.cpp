//Minuteman
#include "Strategy.h"
#include <sstream>

#define checkvals
//------------------------------------------------------------------
//Implementation of the LUA strategy for backtesting
//Strategy is used per thread
using namespace std;


/*
#include <random>
#include <iostream>

using namespace std;

int main()
{
	random_device rd;   // non-deterministic generator
	mt19937 gen(rd());  // to seed mersenne twister.
	uniform_int_distribution<> dist(1,6); // distribute results between 1 and 6 inclusive.

	for (int i = 0; i < 5; ++i) {
		cout << dist(gen) << " "; // pass the generator to the distribution.
	}
	cout << endl;
}
*/

//update variables to be ma length, rval, range filter and aggressive Y/N
Strategy::Strategy(Account* _oAcct, vector <SourceData> *_SRC, vector <SourceData>* _ZIG, vector <MinuteData> *_MINSRC,
			int _idx, int _zidx, int _idx2, 
//			int _rl, int _fl, float _rd, float _rd2, float _rval, float _be, int _ma,
			float _rval, int _ma, int _flim, int _agg,
			float _pipCost, float _mmr, int64_t _maxlots, int _maxloss, int _pctBalance,
			int _cSize, float _pointSize, int _lotAdj, int _procNum, int _tzo, float _truStart,
			bool _useSess,	int _sessOpen, int _sessClose, int _sessOff,
			vector<int>* _excStart, vector<int>* _excEnd,
			int _sessOverrun)
{ 
	SRC = _SRC; idx = _idx; zidx = _zidx; idx2 = _idx2;
//	ZIG = _ZIG;
	MINSRC = _MINSRC;

/*	regress.regLineLen = _rl;
	regress.regLineLimit = _fl;
	regress.regDiff = _rd;
	regress.regDiff2 = _rd2;
	breakEven = _be;
	regress.regLineMA = _ma;
*/

//	atrVal = _atr;
//	atrLim = _atrl;
//	atrPeriod = atrVal;
//	atrLimit = atrLim;
//	MAPeriod = _map;
//	MAType = _mat;
//	trigRate = _tr;
//	zzV.trigRate = trigRate;
//	fibEntry = _fe;
//	zzV.fibEntry = fibEntry;
//	fibTgt = _ft;
//	zzV.fibTgt = fibTgt;
//	fixTgt = _fxt;
//	regVals.fixedTgt = fixTgt;
	

/*	stopVal = _stopVal;
	backStop = _backstop;
	rVal = _rVal;
	rVal2 = _rVal2;
	dailyTgt = _dailyTgt;
	invert = _invert;
	invVal = _invVal;
	useDailyTgt = _useDailyTgt;
//	beVal = _beVal;
*/
	PctBalance = _pctBalance;
	MMR = _mmr;
	MaxPossibleLots = _maxlots;	maxLossPerTradePct = _maxloss;
	PIPCOST = _pipCost;
	ContractSize = _cSize;
	priorTrailPipsStore = priorTrailPips;
	result = _oAcct;
//	result->atrVal = atrPeriod;
//	result->atrLimit = atrLimit;
	result->regLinelimit = _flim;
	result->rVal = _rval;
	result->regLinemalen = _ma;
	result->aggVal = _agg;

	regress.regLinemalen = _ma;


/*	result->MAPeriod = MAPeriod;
	result->MAType = MAType;
	result->trigRate = trigRate;
	result->fibEntry = fibEntry;
	result->fibTgt = fibTgt;
	result->fixTgt = fixTgt;
*/
/*	result->stopVal = stopVal;
	result->backStop = backStop;
	result->rVal = _rVal;
	result->rVal2 = _rVal2;
	result->useDailyTgt = useDailyTgt;
	result->invert = _invert;
	result->invVal = _invVal;
//	result->beVal = _beVal;
*/
	Point = _pointSize;
//	minStop = stopVal* Point * 100;

	pointSize = _pointSize;
	lotAdj = _lotAdj;
	tzofset = _tzo;
	truStart = _truStart;

	useSessionControl = _useSess;
	sessOpen = _sessOpen;
	sessClose = _sessClose;
	sessOff = _sessOff;
	excStart = _excStart;
	excEnd = _excEnd;
	overRunTime = _sessOverrun;

	procNum = _procNum;

//	stopPips = _round(stopVal/10000.0f,5);
//	limitPips = _round(limitVal/10000.0f,5);

	//preload 3 empty tick values as we will only need 3
	TickData td = TickData(0.0f, 0.0f, 0.0f);
	TICKSRC.push_back(td);
	td = TickData(0.0f, 0.0f, 0.0f);
	TICKSRC.push_back(td);
	td = TickData(0.0f, 0.0f, 0.0f);
	TICKSRC.push_back(td);
	td = TickData(0.0f, 0.0f, 0.0f);
	TICKSRC.push_back(td);
	td = TickData(0.0f, 0.0f, 0.0f);
	TICKSRC.push_back(td);

	//preload 5 empty current bars
	bar cb = bar(0.0f, 0.0f, 0.0f, 0.0f);
	currentBar.push_back(cb);
	cb = bar(0.0f, 0.0f, 0.0f, 0.0f);
	currentBar.push_back(cb);
	cb = bar(0.0f, 0.0f, 0.0f, 0.0f);
	currentBar.push_back(cb);
	cb = bar(0.0f, 0.0f, 0.0f, 0.0f);
	currentBar.push_back(cb);
	cb = bar(0.0f, 0.0f, 0.0f, 0.0f);
	currentBar.push_back(cb);

	//add array initialisation for the zig zag suff etc---->>>
/*
	for( int zlev = 0; zlev < zzV.zigLimit; ++zlev)
	{
		int zi = 0;
		zzV.zBper.push_back(zi);  //etc...
		zzV.zAper.push_back(zi);
		zzV._dirA.push_back(zi);
		zzV._dirB.push_back(zi);
		zzV._hlhlA.push_back(zi);
		zzV._hlhlB.push_back(zi);

		bool zb = false;
		zzV._isUpA.push_back(zb);
		zzV._isUpB.push_back(zb);
		zzV._isDownA.push_back(zb);
		zzV._isDownB.push_back(zb);
		float zf = 0.0f;
		zzV._zigzagA.push_back(zf);
		zzV._zigzagB.push_back(zf);

		zzV._xabcdA.push_back(zf);
		zzV._xabcdB.push_back(zf);

	}
*/





	gen=mt19937(1);  // to seed mersenne twister.

	ind = new Indicators(SRC, _ma);// , MAPeriod, MAType);	// , aoValF, aoValS);

//	ind->preloadATR();
//	ind->preloadMA();

	ind->preloadReg(0);
	ind->preloadReg(1);

	//	ind->preloadZlMacd();

//	atrLine = ind->atrLine;		//based on bar data
//	MALine = ind->maLine;
//	sgLine = ind->sigLine;
//	mdLine = ind->mdLine;

	regress.reguVal = ind->regressU;
	regress.regdVal = ind->regressD;

}

Strategy::~Strategy() 
{
	currentBar.clear();
	TICKSRC.clear();
	atrLine.clear();
	MALine.clear();
	sgLine.clear();
	mdLine.clear();
//	excStart.clear();
//	excEnd.clear();
	regress.reguVal.clear();
	regress.regdVal.clear();

	delete ind;
}

// TRAP A CORRUPTED ACCT STRUCTURE

void Strategy::runBT(bool _test)
{
	test = _test;
	//start at the first bar and progress to the last. Generate fake ticks based on volume and
	//enter trades based on entry criteria
	if (test == true)
	{
		TCHAR path[500];
		HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		PathAppend(path, "\\stats.ban");
		ofile.open(path);
		ofile.precision(10);
		//write the test fields out first...
		ofile << "BuySell Trigger Open Lots Stop Limit OpenDate Close/Partial price Lots Stop Limit Balance CloseDate" << endl;
	}

#ifdef checkvals
//	std::ofstream ofilec;
	std::ofstream ofilel;


	if (test == true)	// || procNum>=0)
	{
		TCHAR cpath[500];
		HRESULT rfh = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, cpath);
		
		TCHAR cpathm[500];
		rfh = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, cpathm);

		TCHAR cpathl[500];
		rfh = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, cpathl);

		const size_t buflen = 100;
		TCHAR buf[buflen];
		_sntprintf_s(buf, buflen - 1, _T("Linear_tick%d.ban"), procNum);
		
		PathAppend(cpathm, buf);
		ofilem.open(cpathm);
		ofilem.precision(15);
		//write the test fields out first...
		ofilem << "Proc Date Open High Low Close " << endl;

		TCHAR bufl[buflen];
		_sntprintf_s(bufl, buflen - 1, _T("Linear_reg%d.ban"), procNum);
		PathAppend(cpathl, bufl);
		ofilel.open(cpathl);
		ofilel.precision(15);
		//write the test fields out first...
		ofilel << "regu regd" << endl;
	}
#endif

/*	vector <SourceData> src = (*(SRC));
	vector <MinuteData> minsrc = (*(MINSRC));
*/
	int endPeriod = (*(SRC)).size();

	startBalance = acct.Balance;

//	int zzPeriod = 0;
	for (int period = 0; period < endPeriod; ++period)
	{
/*		zzPeriod = period / 2;
		//update zig zags if possible here
		if(zzPeriod > 1 && period % 2==0)
		{
			upDateZigZag(zzPeriod);
		}
*/
		//calculate the number of ticks to be processed based on volume.
		//Establish the up/down bias based on ending ohlc vals.

//		if (period > atrPeriod)
//			ind->prepATR(period);

		trigger = 0.0f;

#ifdef checkvals
		if (test == true)	// || procNum>=0)
		{//Save minute data for checking CHANGE VALUES....
			ofilem << procNum << " " << (*(SRC))[period].date << " " << (*(SRC))[period].open << " " << (*(SRC))[period].high << " "
				<< (*(SRC))[period].low << " " << (*(SRC))[period].close << endl;

			ofilel << (*(SRC))[period].date << " " << regress.reguVal[period] << " " << regress.regdVal[period] << endl;
		}
#endif

		double lastBo = 0.0;
		//Process a 5 min bar
		for (int min = 0; min < 5; ++min)
		{
			int numTicks = getTicksByVol(period);	//return a small part of the 5 min volume converted to random ticks per minute
			TickInfo ti;
			setTI(period, min, numTicks, /*&minsrc, &src,*/ &ti, lastBo);
			lastBo = ti.minClose;

			for (int tick = 0; tick < numTicks; ++tick)
			{
				createNewTick(period, min, tick, &ti); //place the tick vals in the TICKSRC bid/ask

				//Are we at the runstart date?
				if ((*(SRC))[period].date >= truStart)
				{
					//---------------------------------------------------------------
					//now run the "update" function and check for trade entry/closure
					update(period, min, 0/*, zzPeriod*/);
					//---------------------------------------------------------------
				}

				if (ti.nextTarget == 4) //we've reached the close price so force an exit
					tick = numTicks;

#ifdef checkvals
				if (test == true)	// || procNum>=0)
				{//Save minute data for checking CHANGE VALUES....
					ofilem << procNum << " " << TICKSRC[0].dtime << " " << currentBar[0].open << " " << currentBar[0].high << " "
						<< currentBar[0].low << " " << currentBar[0].close << endl;	// << " " << ind->atrLine[0] << endl; 
				}
#endif

				if (abort == true) break; //No point going on if we are heading for negative territory!
			}	
			//shift the final ticksrc values so we can reference them
			TICKSRC[4] = TICKSRC[3];
			TICKSRC[3] = TICKSRC[2];
			TICKSRC[2] = TICKSRC[1];
			TICKSRC[1] = TICKSRC[0];
		}
		//shift the currentBar values
		currentBar[4] = currentBar[3];
		currentBar[3] = currentBar[2];
		currentBar[2] = currentBar[1];
		currentBar[1] = currentBar[0];

		//now clear the indicator "tick vectors"
//		ind->clearATRTick();

		//std::this_thread::yield();

#ifdef checkvals
//		if (test == true)
//		{//Save the AO values for checking
//			ofilec << (*(SRC))[period].date << " " << 0.0f << " " << 0.0f << " " << 0.0f << " "	<< 0.0f << " " << 0.0f << endl;	// << " " << ind->aoLine[period] 
//		}
#endif

	}

	if (test == true)
	{
		ofile << acct.maxBalance << " " << acct.minBalance << " " << acct.maxMinPct << " Balance: " << acct.Balance << endl;
		ofile.close();
	}

#ifdef checkvals
	if (test == true)	// || procNum>=0)
	{
//		ofilec.close();
		ofilem.close();
		ofilel.close();
	}
#endif


//	result->priorPips = priorTrailPipsStore;
	result->maxBalance = acct.maxBalance;
	result->minBalance = acct.minBalance;
	result->maxMinPct = acct.maxMinPct;
	result->Balance = acct.Balance;

	int inv = 0;
//	if (result->invert == true) inv = 1;
	double key = result->Balance * 100000 + result->regLinelimit * 100 + result->regLinemalen * 10;// +inv;
	result->key = key;
	result->Date = (int)(*(SRC))[0].date;


	std::this_thread::yield();
}

void Strategy::setTI(int period, int _min, int numTicks, /*vector <MinuteData>* _minsrc, vector <SourceData>* _src,*/ TickInfo* ti, double bo)
{
//	vector<SourceData> src = *_src;
//	vector <MinuteData> minsrc = *_minsrc;
	
	double _mo = 0.0f;
	double _mh = 0.0f;
	double _ml = 0.0f;
	double _mc = 0.0f;
	double barOpen = bo;

	if(_min==0) barOpen= (*SRC)[period].open;

	reCreateMinute(_min, period, /*minsrc[period].mins,*/ barOpen, &_mo, &_mh, &_ml, &_mc);
	barOpen = _mc;
	ti->minHigh = (float)_mh;
	ti->minLow = (float)_ml;
	ti->minClose = (float)_mc;

#ifdef checkvals
	if (test == true)	// || procNum>=0)
	{//Save minute data for checking CHANGE VALUES....
		ofilem << procNum << " " << _min << " " << _mo << " " << _mh << " " << _ml << " " << _mc << endl;
	}
#endif

	double body = abs(_mo - _mc);
//	if (!(body > 0.0f)) body = 0.000001f;
	ti->HO = (float)abs(_mh - _mo);
	ti->OL = (float)abs(_mo - _ml);

	ti->dirUpClosest = ti->HO < ti->OL;
	ti->tickTargetPct1 = (int)numTicks * 30 / 100;
	ti->tickTargetPct2 = (int)numTicks * 70 / 100;
	ti->nextTarget = 1;

	/*if (ti->dirUpClosest == true)
	{
		ti->tickTarget1 = _mh;
		ti->tickTarget2 = _ml;
	}
	else
	{
		ti->tickTarget1 = _ml;
		ti->tickTarget2 = _mh;
	}*/

	ti->maxTickSize = (float)(((_mh - _ml) / numTicks) * 10);
	ti->bodySize = (float)(min(_mc, _mo) + (body / 2));
	ti->midHigh = (float)(_mh - ti->bodySize);
	ti->midLow = (float)(ti->bodySize - _ml);

	double midTot = ti->midHigh + ti->midLow;
	ti->upPct = (float)((ti->midHigh / midTot) * 80);	//body;
	ti->downPct = (float)((ti->midLow / midTot) * 80);	//body;


	if (ti->dirUpClosest == true)	//up
	{
		ti->tickTarget1 = (float)_mh;
		ti->tickTarget2 = (float)_ml;
		ti->UDPct = ti->downPct;
		ti->dir = 1;
	}
	else  //down
	{
		ti->tickTarget1 = (float)_ml;
		ti->tickTarget2 = (float)_mh;
		ti->UDPct = (float)ti->upPct;
		ti->dir = -1;
	}

	ti->numTicks = numTicks;
	ti->tickOffset = 0;
	
	ti->dtOffset = (sec1 * 60) / numTicks;	//1 minute of seconds divided by total ticks giving seconds update per tick
}

void Strategy::reCreateMinute(int _minNum, int _period,/*unsigned char _minute[91],*/ double _barOpen, double* mo, double* mh, double* ml, double* mc)
{	//recreate the ohlc for the minute from the char encoded representation and put into the return params
	double minStart = _barOpen;
	double multi = 1;
	int m_int = 1;
	int m_frac = 0;
	unsigned char *_minute;
	
	_minute = ((*(MINSRC))[_period]).mins;

	//Recover the text value of the specified minute and convert to floats
	unsigned char ov = _minute[(_minNum * 6) + 2];
	unsigned char hv = _minute[(_minNum * 6) + 3];
	unsigned char lv = _minute[(_minNum * 6) + 4];
	unsigned char cv = _minute[(_minNum * 6) + 5];

	m_int = _minute[(_minNum * 6)] - 48;
	m_frac = _minute[(_minNum * 6) + 1] - 48;
	multi = (m_int + (m_frac / 100.0f));

	*mo = minStart;
	*mh = (minStart - ((hv - 83) * multi / 10.0f));
	*ml = (minStart - ((lv - 83) * multi / 10.0f));
	*mc = (minStart - ((cv - 83) * multi / 10.0f));
}

int Strategy::getTicksByVol(int period)
{
	//Revised for minute processing as each minute needs to be 1/5th of the whole so scale down the min/max
	//Try min of 5 and max of 22
	int retVal = 0;
	
	//take the volume and take 10% of it minimum of 20 ticks and max of 300
	retVal = (int)((*SRC)[period].volume/10);
	if (retVal < 5) retVal = 5;//20
	if (retVal > 22) retVal = 22;//300
	return retVal;
}

void Strategy::createNewTick(int period, int min, int tick, TickInfo* ti)
{
//	srand(period+min);	//ensure we have a constant seeding
	//mt19937 gen(period + min);

	double newTick = 0.0f;
	//set the current tick values to the open price and use a spread of 0.3pips
	double spread = 1.0f;
	double ask = 0.0f;
	double bid = 0.0f;

	double target = 0.0f;
	int targetBias = 0;

	int dir = 0;
	int rn = 0;
	int bias = 0;
	int prob = 0;

	double tickSize = 0.0f;

	if (tick == 0) //first tick is easy as it is the open price 
	{
		/*
				if (period > 0)
				{
					//shift the ticksrc values so we can override the 0 value with a new value
					TICKSRC[4] = TICKSRC[3];
					TICKSRC[3] = TICKSRC[2];
					TICKSRC[2] = TICKSRC[1];
					TICKSRC[1] = TICKSRC[0];

					//shift the currentBar values also
					currentBar[4] = currentBar[3];
					currentBar[3] = currentBar[2];
					currentBar[2] = currentBar[1];
					currentBar[1] = currentBar[0];
				}
		*/	
		uniform_real_distribution<> rdist(1, 1.8);
//		spread = _round(((float)rand() / RAND_MAX*0.00005f) + 0.000005f, 5);
		spread = _round((float)rdist(gen), 5);

		if (min == 0)
			bid = (*SRC)[period].open;
		else
			bid = TICKSRC[0].bid;
		ask = bid + spread;
		
		TICKSRC[0].dtime = (*SRC)[period].date + (sec1 * (float)(60 * min));
		TICKSRC[0].bid = (float)bid;
		TICKSRC[0].ask = (float)ask;

		currentBar[0].open = (float)bid;
		currentBar[0].high = (float)bid;
		currentBar[0].low = (float)bid;
		currentBar[0].close = (float)bid;
	}
	else
	{
		//Which direction
		
		if (ti->nextTarget == 1) { target = ti->tickTarget1; targetBias = ti->tickTargetPct1; ti->tickOffset = tick; }
		else if (ti->nextTarget == 2) { target = ti->tickTarget2; targetBias = ti->tickTargetPct2; ti->tickOffset = tick;}
		else if (ti->nextTarget == 3) { target = ti->minClose; targetBias = 60; ti->tickOffset = tick;}
		if (target > TICKSRC[0].bid) ti->dir = 1;
		if (target <= TICKSRC[0].bid) ti->dir = -1;
		
		uniform_int_distribution<> idist(0, 100);
//		rn = (int)((float)rand()/RAND_MAX * 100);
		rn = idist(gen);

		bias = (int)(targetBias - (targetBias - pow(tick, 1.33)));
		prob = rn + bias;

		uniform_real_distribution<> rdist(0.0, ti->maxTickSize);
//		tickSize = ((float)rand() / RAND_MAX) *ti->maxTickSize;
		tickSize = (float)rdist(gen);

		//Now we choose whether we move towards target or not
		if(prob>ti->UDPct)
			bid = TICKSRC[0].bid + (tickSize*ti->dir);
		else
			bid = TICKSRC[0].bid - (tickSize*ti->dir);

		/*if (bid > (*SRC)[period].high) bid = (*SRC)[period].high;
		if (bid < (*SRC)[period].low) bid = (*SRC)[period].low;*/
		if (bid > ti->minHigh) bid = ti->minHigh;
		if (bid < ti->minLow) bid = ti->minLow;

		rdist=uniform_real_distribution<>(1.0, 1.80);
		//		spread = _round(((float)rand() / RAND_MAX*0.00005f) + 0.000005f, 5);
		spread = _round((float)rdist(gen), 5);

		ask = bid + spread;
		
		if ((ti->dir==1 && bid >= target) || (ti->dir==-1 && bid<=target))  ti->nextTarget++;
		if (ti->nextTarget == 4 || tick==ti->numTicks-1)
		{
			bid = ti->minClose;
			ask = bid + spread;
		}

		TICKSRC[0].dtime = TICKSRC[0].dtime + ti->dtOffset;
		TICKSRC[0].bid = (float)bid;
		TICKSRC[0].ask = (float)ask;
		
		if(bid>currentBar[0].high) currentBar[0].high = (float)bid;
		if(bid<currentBar[0].low) currentBar[0].low = (float)bid;
		currentBar[0].close = (float)bid;
	}

	newTick = TICKSRC[0].bid;

	//send the new tick value to the indicator routines if posible
	
//	if(period > atrPeriod) ind->updateATR(period, &currentBar);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Strategy::update(int period, int min, int zPeriod)
{

	//--set starting zigzag source vals
	if (lastBid == 99999999.0)
	{
		lastBid = (*(SRC))[period - 1].close;	 //BIDsource.close[BIDsource:size() - 1];
		lastAsk = (*(SRC))[period - 1].aclose;	 //ASKsource.close[ASKsource:size() - 1];
	}

/*	if (lastZBid == 99999999.0)
	{
		lastZBid = (*(ZIG))[zPeriod - 1].close;
		lastZAsk = (*(ZIG))[zPeriod - 1].aclose;
	}
*/

	COleDateTime dt = ((*SRC)[period].date);
	cTime = dt.GetHour() * 100 + dt.GetMinute() + min;
	tradingDay = dt.GetDayOfWeek();

	if (acct.BuySell == 0 && state > 3)
	{
		possBuy = false;
		possSell = false;
		buySU = false;
		if ((long)dayStart == 0.0) dayStart = dt;
		sellSU = false;
		state = 0;
		entryDir = 0;
		trgrCount = 0;
		tradeActive = false;
		_hasCrossed = false;
		//		maOrder = 0;
		entryDir = 0;
	}

	if (lastBar != period)
	{
		lastBar = period;
		newBar = true;
		attempted = false;
		dailyBalance = acct.Balance;
		closedFlag = false;
		if (acct.BuySell == 0) tradeActive = false;
	}
	idx = period;

	/*	if (newBar == true && dtHit == false && acct.BuySell == 0 && useDailyTgt == true)
		{
			double dailyPL = getPL();
			//core.host:trace(string.format("Balance check.. %f  %f", startBalance, dailyPL));
			if (dailyPL >= startBalance * (dailyTgt / 100))
			{
				dtHit = true;
			//core.host:trace("Daily Target reached");
			}
		}
	*/
	ALLOWTRADE = true;
	checkTradingAllowed(dt);

	/* Handled in main period loop outside of update
	if (lastZBar != zidx - 1)	//ZASKsource:size() - 1)
	{
		lastZBar = zidx - 1;	// ZASKsource: size() - 1;
		upDateZigZag(zidx - 2);
	}
	*/

	if (newBar == true) 	//--set spikes even if no trading period
	{
		float nh = 0.0f;
		float nl = 0.0f;
//		bidx = BIDsource:size() - 1;
//		aidx = ASKsource:size() - 1;
/*		--local rv = 0;
		--local rc = 0;

		--nh = getLastSpike(0, 1);
		--nl = getLastSpike(0, -1);
		--checkSpikes(nh, nl, bidx, aidx);
		--rv, rc = getCrossCount(bidx, aidx);*/
	}

	//--if a Friday and after cut off time then exit trade
	if (newBar == true && tradingDay == 6 && weekendClose > 0)
	{
		if (dt.GetHour() + (dt.GetMinute() / 100) >= weekendClose)
		{
			if (acct.BuySell != 0) overrunExit = true;
			ALLOWTRADE = false;
		}
	}
	
	//if still in a trade after trading period then don't let it go on for ever 
	if (ALLOWTRADE == false && acct.BuySell != 0 && newBar == true)
	{
		//--SET TO BREAKEVEN IF IN PROFIT SO NO LOSING ON POST SESSION MOVEMENT
/*		if (dt.GetHour() * 100 + dt.GetMinute() >= sessClose && brkEvenSet == false && inProfit == true)
		{
			brkEven = true;
			brkEvenSet = false;
			brkEvenOverrun = true;
		}
*/
		if (dt.GetHour()*100 + dt.GetMinute() >= overRunTime)
			overrunExit = true;
	}

	if (ALLOWTRADE == true && tradeActive==false && closedFlag==false && dtHit==false
		&& acct.BuySell == 0 && orderPending == false && possBuy == false && possSell == false)
	{
		checkForSetup(period);
	}

	if (acct.BuySell!= 0) checkForClose(false);
	
	if(orderPending == false && ALLOWTRADE == true && attempted==false)
		checkForOpen();
	
	if(acct.BuySell!= 0) checkForTarget();

	newBar = false;
	lastBid = (*(SRC))[period - 1].close; //BIDsource.close[BIDsource:size() - 1];
	lastAsk = (*(SRC))[period - 1].aclose; //ASKsource.close[ASKsource:size() - 1];
//	lastZBid = (*(ZIG))[zPeriod - 1].close; //ZBIDsource.close[ZBIDsource:size() - 1];
//	lastZAsk = (*(ZIG))[zPeriod - 1].aclose; //ZASKsource.close[ZASKsource:size() - 1];
}

//------------------------------------------------------------------
/*bool Strategy::checkMultiAllowed()
{
	bool retVal = false;
	bool multi = true;
	int period = idx2;
	
	//--if true allow trades up the trylimit to be attempted
		if (allowMultiTry == true && attempts >= tryLimit) multi = false;
		if (multi == true)
			retVal = true;

	return retVal;
}*/

//------------------------------------------------------------------
void Strategy::checkTradingAllowed(COleDateTime sdt)
{
	if (useSessionControl == false)
	{
		ALLOWTRADE = true;
		Weekend = false;
		return;
	}
	//--if day is 1, 6 then disallow trading if time is near close hour too soon to open on the sunday

	//int sessionClose = (sessionCloseHour * 100 + sessionCloseMin) + tzofset;
	//int sessionOpen = (sessionOpenHour * 100 + sessionOpenMin) + tzofset;
	if (tradingDay == 6 && cTime >= sessClose) ALLOWTRADE = false;
	if (tradingDay == 1 && cTime < sessOpen ) ALLOWTRADE = false;
	if (tradingDay == 7) ALLOWTRADE = false;
	if (ALLOWTRADE == false)  Weekend = true;

	// now implement any intraday restriction
	if (cTime < sessOpen || cTime >= sessClose) { Weekend = false; ALLOWTRADE = false; }
	
//if (newBar == true)
//	int yh = 1;

	//--check all the exclusion times too.
	int tid = 0;
	while (tid < 5)
	{
		if (((int*)excStart)[tid] > 0.0 && cTime >= ((int*)excStart)[tid] && cTime < ((int*)excEnd)[tid])
		{
		//	because = string.format("Restricted trading.. Time %f From %f To %f", cTime, session.excStartTime[tid], session.excEndTime[tid]);
			ALLOWTRADE = false;
		}
		tid++;
	}

	if (ALLOWTRADE == true && warningSet == true) 
	{
		warningSet = false;
		brkEvenOverrun = false;
		//core.host:trace(string.format("Trading starts.."));
		state = 0; dtHit = false; dailyBalance = acct.Balance; 
		startBalance = dailyBalance + getPL(); dayStart = sdt; buySU = false; sellSU = false;  _hasCrossed = false;
	}
	if (ALLOWTRADE == false && newBar == true && warningSet == false)
	{
		Weekend = false;
		//core.host:trace(string.format("No trading.. Time %i From %f To %f", cTime, sessionClose, sessionOpen));
		warningSet = true;
	}
}

//------------------------------------------------------------------
float Strategy::_round(float num, int idp)
{
	int mult = (int)pow(10,idp);
	if (num >= 0.0f)
		return (float)(floor(num * mult + 0.5f) / mult);
	else return (float)(ceil(num * mult - 0.5f) / mult);
}


//-------------------------------------------------------------------------------------- -
/*void Strategy::upDateZigZag(int lz)
{
	zzV.zBper[zzV.zBid] = lz;
	zzV.zAper[zzV.zAid] = lz;

	int pzB = zzV.zBid - 1;
	if (pzB == -1) pzB = zzV.zigLimit-1;
	int pzA = zzV.zAid - 1;
	if (pzA == -1) pzA = zzV.zigLimit-1;

	float hiA = max((*(ZIG))[lz].ahigh, (*(SRC))[lz - 1].ahigh);
	float loA = min((*(ZIG))[lz].alow, (*(ZIG))[lz - 1].alow);
	float hiB = max((*(ZIG))[lz].high, (*(ZIG))[lz - 1].high);
	float loB = min((*(ZIG))[lz].low, (*(ZIG))[lz - 1].low);

	zzV._isUpA[zzV.zAid] = (*(ZIG))[lz].aclose >= (*(ZIG))[lz].aopen;
	zzV._isDownA[zzV.zAid] = (*(ZIG))[lz].aclose <= (*(ZIG))[lz].aopen;

	zzV._dirA[zzV.zAid] = zzV._dirA[pzA];
	if (zzV._isUpA[pzA] && zzV._isDownA[zzV.zAid]) {
		zzV._dirA[zzV.zAid] = -1;
	}
	else if (zzV._isDownA[pzA] && zzV._isUpA[zzV.zAid]) {
		zzV._dirA[zzV.zAid] = 1;
	}

	zzV._zigzagA[zzV.zAid] = -1;
	if (zzV._isUpA[pzA] && zzV._isDownA[zzV.zAid] && zzV._dirA[pzA] != -1) {
		zzV._zigzagA[zzV.zAid] = hiA;
	}
	else if (zzV._isDownA[pzA] && zzV._isUpA[zzV.zAid] && zzV._dirA[pzA] != 1) {
		zzV._zigzagA[zzV.zAid] = loA;
	}

	zzV._isUpB[zzV.zBid] = (*(ZIG))[lz].close >= (*(ZIG))[lz].open;
	zzV._isDownB[zzV.zBid] = (*(ZIG))[lz].close <= (*(ZIG))[lz].open;

	zzV._dirB[zzV.zBid] = zzV._dirB[pzB];
	if (zzV._isUpB[pzB] && zzV._isDownB[zzV.zBid]) {
		zzV._dirB[zzV.zBid] = -1;
	}
	else if (zzV._isDownB[pzB] && zzV._isUpB[zzV.zBid]) {
		zzV._dirB[zzV.zBid] = 1;
	}

	zzV._zigzagB[zzV.zBid] = -1;
	if (zzV._isUpB[pzB] && zzV._isDownB[zzV.zBid] && zzV._dirB[pzB] != -1) {
		zzV._zigzagB[zzV.zBid] = hiB;
	}
	else if (zzV._isDownB[pzB] && zzV._isUpB[zzV.zBid] && zzV._dirB[pzB] != 1) {
		zzV._zigzagB[zzV.zBid] = loB;
	}

	zzV.zBid = zzV.zBid + 1; if (zzV.zBid == zzV.zigLimit) zzV.zBid = 0;
	zzV.zAid = zzV.zAid + 1; if (zzV.zAid == zzV.zigLimit) zzV.zAid = 0;
}*/



/*
//--------------------------------------------------------------------------------------
float Strategy::getLastSpike(int dir)
{
	int _period=idx;
//	int bidx = BIDsource:size() - 1;
//	int aidx = ASKsource:size() - 1;
	float retVal = 0.0f;



	if (dir == 1) { retVal = 0.0f; } //--up so use ask as trigger
	if (dir == -1) { retVal = 999999.0f; }	//--down so use bid as trigger

	//--local cb = (_period - 1) - backbars;
	int lp = _period - 1;

	float spread0 = TICKSRC[0].ask - TICKSRC[0].bid;
	float spread1 = TICKSRC[1].ask - TICKSRC[1].bid;
	float spread2 = TICKSRC[2].ask - TICKSRC[2].bid;
	float spread3 = TICKSRC[3].ask - TICKSRC[3].bid;
	float spread4 = TICKSRC[4].ask - TICKSRC[4].bid;

	float h0 = 0.0f;
	float l0 = 0.0f;
	float h1 = 0.0f;
	float l1 = 0.0f;
	float h2 = 0.0f;
	float l2 = 0.0f;
	float h3 = 0.0f;
	float l3 = 0.0f;
	float h4 = 0.0f;
	float l4 = 0.0f;


	//--check last "backbars" for spike highand low
	if (dir == 1)
	{

		h0 = (*(SRC))[lp].high + spread0;
		l0 = (*(SRC))[lp].low + spread0;
		h1 = (*(SRC))[lp - 1].high + spread1;
		l1 = (*(SRC))[lp - 1].low + spread1;
		h2 = (*(SRC))[lp - 2].high + spread2;
		l2 = (*(SRC))[lp - 2].low + spread2;
		h3 = (*(SRC))[lp - 3].high + spread3;
		l3 = (*(SRC))[lp - 3].low + spread3;
		h4 = (*(SRC))[lp - 4].high + spread4;
		l4 = (*(SRC))[lp - 4].low + spread4;

		if (h0 < h2 && h1 < h2 && h3 <= h2 && h4 < h2 && h2>retVal)
		{
			retVal = h2;
		}

	}
	else
	{
		h0 = (*(SRC))[lp].high;
		l0 = (*(SRC))[lp].low;
		h1 = (*(SRC))[lp - 1].high;
		l1 = (*(SRC))[lp - 1].low;
		h2 = (*(SRC))[lp - 2].high;
		l2 = (*(SRC))[lp - 2].low;
		h3 = (*(SRC))[lp - 3].high;
		l3 = (*(SRC))[lp - 3].low;
		h4 = (*(SRC))[lp - 4].high;
		l4 = (*(SRC))[lp - 4].low;

		if (l0 > l2 && l1 > l2 && l3 >= l2 && l4 > l2 && l2 < retVal)
		{
			retVal = l2;
		}
	}
return retVal;
}
*/

//-------------------------------------------------------------------------------------- -
int Strategy::getRegUpDn(int ba, int dataset, int depth, int _prd, int offset)
{
	int retVal = 0;

//	float b = 0.0f;
//	float m = 0.0f;
	int b1 = 0;
	int b2 = 0;
	float p1 = 0.0f;
	float p2 = 0.0f;

	int _period = _prd;
	int RD = depth;
	float _b = 0.0f;
	float _m = 0.0f;

	getRegression(RD, ba, dataset, _period, _b, _m);

	b1 = _period - RD + 1;
	b2 = _period;
	p1 = _m * b1 + _b;
	p2 = _m * b2 + _b;

	if (p2 > p1) retVal = 1;
	if (p2 < p1) retVal = -1;

	return retVal;
}

//-------------------------------------------------------------------------------------- -
void Strategy::getRegression(int depth, int ba, int dataset, int _period, float& b, float& m)
{
	//--local retVal = mathex.lregSlope(source, _period - depth + 1, _period);
	//--	return retVal;
	//--end

	//--function ab()
	float meanx = 0.0f;
	float meany = 0.0f;
	float meanxy = 0.0f;
	float meanx2 = 0.0f;
//	float m = 0.0f;
//	float b = 0.0f;

	//--starting with period value work backand calc means
	int idx = _period - depth + 1;
	int pos = depth;
	float totx = 0.0f;
	float toty = 0.0f;
	float totxy = 0.0f;
	float totx2 = 0.0f;

	while (idx <= _period)
	{
		float sVal= sourceVal(dataset,idx);
		totx = totx + pos;
		toty = toty + sVal;
		totxy = totxy + (pos * sVal);
		totx2 = totx2 + (pos * pos);
		idx = idx + 1;
		pos = pos - 1;
	}
	/*
		while (idx<=_period)
		{
			float sVal= sourceVal(dataset,idx);
			totx=totx+idx;
	--		toty=toty+source[idx];
			toty=toty+sVal;
	--		totxy=totxy+(pos*source[idx]);
			totxy=totxy+(idx*sVal);
			totx2=totx2+(idx*idx);
			idx=idx+1;
	--		pos=pos-1;
		}
	*/

	meanx = totx / depth;
	meany = toty / depth;
	meanxy = totxy / depth;
	meanx2 = totx2 / depth;
	float m1 = (meanx * meany) - meanxy;
	float m2 = ((meanx * meanx) - meanx2);
	m = m1 / m2;
	b = meany - m * meanx;

	//--calculate the angle here base on ends of line at periodand period - depth
	return;	// b, m;
}

//-------------------------------------------------------------------------------------- -
float Strategy::sourceVal(int ds, int id)
{
	float retVal = 0.0f;
	if (ds == 0)
	{
		retVal = (*(SRC))[id].close;
	}
	if (ds == 1)
	{
		retVal = ((*(SRC))[id].open + (*(SRC))[id].high + (*(SRC))[id].low + (*(SRC))[id].close) / 4;
}
	return retVal;
}

/*
//--------------------------------------------------------------------------------------
int Strategy::hasCrossed(int bix)	//	--look for a cross of the macd and signal lines
//--This is, of course, effectively the cross of the zero line by the histogram
{
	int retVal = 0;
	//float pres = source:getPrecision();
	//--macd uses odd indexing
	float md = _round(mdLine[bix - 1], pres);
	float sg = _round(sgLine[bix - 1], pres);
	float md2 = _round(mdLine[bix - 2], pres);
	float sg2 = _round(sgLine[bix - 2], pres);

//	float mdd = _round(mdLine[bix], pres);
//	float sgd = _round(sgLine[bix], pres);
//	float mdd2 = _round(mdLine[bix - 1], pres);
//	float sgd2 = _round(sgLine[bix - 1], pres);

	//--core.host:trace(string.format("mdu %f, sgu %f, mdu2 %f, sgu2 %f", mdu, sgu, mdu2, sgu2));

	if (md > sg && md2 <= sg2) 	//--and mdu > 0.0 and sgu >= 0.0) then
	{
		retVal = 1;
	}

	if (md < sg && md2 >= sg2)	//--and mdd < 0.0 and sgd <= 0.0) then
	{
		retVal = -1;
	}

	return retVal;
}
*/

//--------------------------------------------------------------------------------------
/*
int Strategy::validArrakis(int bx, int Ord)
{
	int retVal = 0;

	bool comparatoru = false;
	bool comparatord = false;

	_hasCrossed = false;
	float maSpread = (*(SRC))[bx - 1].aclose - (*(SRC))[bx - 1].close;

	comparatoru = (*(SRC))[bx - 1].close > MALine[bx - 1];
	comparatord = (*(SRC))[bx - 1].aclose < MALine[bx - 1] + maSpread;
	if (invert == true)
	{
		comparatoru = (*(SRC))[bx - 1].close < MALine[bx - 1];
		comparatord = (*(SRC))[bx - 1].aclose > MALine[bx - 1] + maSpread;
	}

	if (Ord == 1 && comparatoru) 	//--and BIDsource.close[bx - 1] > triggerLevel) then
	{
		touchBar = bx - 1; retVal = 1;
		lowVal = getStop(1, bx);
//		core.host:trace(string.format("Target Low %f", setUpVals.lowVal));
	}
	if (Ord == -1 && comparatord)	//--and ASKsource.close[ax - 1] < triggerLevel) then
	{
		touchBar = bx - 1; retVal = -1;
		highVal = getStop(-1, bx);
//		core.host:trace(string.format("Target High %f", setUpVals.highVal));
	}
	return retVal;
}
*/

/*//-------------------------------------------------------------------------------------- -
float Strategy::getStop(int dir, int bx)
{
	float retVal = 0.0;
	//local pres = source:getPrecision();
	float range = 0.0;

	//--first try the recent high / low.
	if (dir == 1)
	{
		retVal = lowest(backStop, bx-1);
		if (retVal == -1.0)
			retVal = (*(SRC))[bx - 1].low - (1000 * Point);
		else
			retVal = _floor(retVal, pres, 5);

		range = (*(SRC))[bx - 1].high - (*(SRC))[bx - 1].low;
		if (range / atrLine[bx - 1] > atrLim)
		{
			retVal = (*(SRC))[bx - 1].low + (range * 36.8f / 100.00f);
		}
	}

	if (dir == -1)
	{
		retVal = highest(backStop, bx-1);
		if (retVal == -1.0)
			retVal = (*(SRC))[bx - 1].ahigh + (1000 * Point);
		else
			retVal = _ceil(retVal, pres, 5);

		range = (*(SRC))[bx - 1].ahigh - (*(SRC))[bx - 1].alow;
		if (range / atrLine[bx - 1] > atrLim)
		{
			retVal = (*(SRC))[bx - 1].ahigh - (range * 36.8f / 100.00f);
		}
	}

//	if (retVal == 0.0)
//	{
//		core.host:trace(string.format("Target fuckup"));
//	}
	
	return retVal;
}
*/

//-------------------------------------------------------------------------------------- -
float Strategy::_floor(float val, int idp, int sig)
{
	//--take val and chop any value beyond sig
	float retVal = 0.0f;
//	int dp = 10 ^ -(idp | 0);
	float dp = pow(10.0f, (float)(-idp));
	float _mod = (float)fmod(val - dp, sig);
	retVal = floor(val - _mod);
	return retVal;
}


//-------------------------------------------------------------------------------------- -
float Strategy::_ceil(float val, int idp, int sig)
{
	//--take val and chop any value beyond sig
	float retVal = 0.0f;
	int dp = 0;	//--10 ^ (idp or 0);
	float _mod = (float)fmod(val - dp, sig);
	retVal = ceil(val - _mod) + sig;
	return retVal;
}


//-------------------------------------------------------------------------------------- -
float Strategy::highest(float per, int prd)	//return the first high / low moving back from start bar
{
	float hi = -1.0f;
	int t = 0;
	bool found = false;
	while (t < per && found == false)
	{
		if ((*(SRC))[prd - t - 1].ahigh < (*(SRC))[prd - t].ahigh)
		{
			hi = (*(SRC))[prd - t].ahigh; found = true;
		}
		t = t + 1;
	}
	return(hi);
}

//-------------------------------------------------------------------------------------- -
float Strategy::lowest(float per, int prd)
{
	float lo = -1.0f;
	int t = 0;
	bool found = false;
	while (t < per && found == false)
	{
		if ((*(SRC))[prd - t - 1].low > (*(SRC))[prd - t].low)
		{
			lo = (*(SRC))[prd - t].low; found = true;
		}
		t = t + 1;
	}
	return(lo);
}

//--------------------------------------------------------------------------------------
/*bool Strategy::getNextZZ(std::vector<float>& src, int offset, int& id,
				std::vector<int>& per, int scan, float& tgt) 
{
	bool retVal = false;
	float _tgt = 0.0f;

	int preper = per[id];
	int _id = id;

	bool ok = false;
	_id = (_id - offset + zzV.zigLimit) % zzV.zigLimit;

	while (!ok) 
	{
		ok = true;

		_tgt = src[_id];

		if (_tgt > 0 && scan == 0) {
			retVal = true;
			ok = false;
			break;
		}

		if (_tgt > 0 && scan == 1 && per[_id] < preper) {
			retVal = true;
			ok = false;
			break;
		}

		if (_tgt <= 0 && scan == 1) 
		{
			retVal = true;
			ok = false;
			preper = per[_id];

			_id = (_id - 1 - offset + zzV.zigLimit) % zzV.zigLimit + 1;

			if ((per[_id] > 0 && per[_id] > preper) || (per[_id] <= 0)) 
			{
				retVal = false;
				ok = true;
			}
			if (per[_id] > 0 && per[_id] < preper) 
			{
				ok = false;
			}
		}
	}
	if (retVal) {
		tgt = _tgt;
		id = _id;
	}

	return retVal;
}
*/

//--------------------------------------------------------------------------------------
/*int Strategy::gethlv(int BA) {
	int id1 = zzV.x_dB;
	int id2 = id1 - 1;
	int id3 = id2 - 1;
	int retVal = 0;

	std::vector<float> vec = zzV._xabcdB;
	if (BA == -1) vec = zzV._xabcdA;

	if (id2 < 0) {
		id2 += 5;
	}
	if (id3 < 0) {
		id3 += 5;
	}

	if (vec[id1] >= vec[id3] && vec[id1] > vec[id2] && vec[id2] < vec[id3]) {
		retVal = 1;
	}

	if (vec[id1] <= vec[id3] && vec[id1] < vec[id2] && vec[id2] > vec[id3]) {
		retVal = -1;
	}

	return retVal;
}*/

//--------------------------------------------------------------------------------------
/*void Strategy::shiftXABCD(int BA, float zval) {
	if (BA == 1 && zval != zzV._xabcdB[zzV.x_dB]) {
		zzV.x_dB = zzV.x_dB + 1;
		zzV.x_dB = ((zzV.x_dB - 1 + 5) % 5) + 1;
		zzV._xabcdB[zzV.x_dB] = zval;
		zzV._hlhlB[zzV.x_dB] = gethlv(BA);
	}

	if (BA == -1 && zval != zzV._xabcdA[zzV.x_dA]) {
		zzV.x_dA = zzV.x_dA + 1;
		zzV.x_dA = ((zzV.x_dA - 1 + 5) % 5) + 1;
		zzV._xabcdA[zzV.x_dA] = zval;
		zzV._hlhlA[zzV.x_dA] = gethlv(BA);
	}
}

bool Strategy::getXABCD()
{
	bool rv = false;
	int lid = 0;
	int _lid = (zzV.zBid - 1 + zzV.zigLimit) % zzV.zigLimit;
	float zval = 0.0f;

	rv = getNextZZ(zzV._zigzagB, 0, _lid, zzV.zBper, 0, zval);	// , lid);
	if (rv) {
		shiftXABCD(1, zval);
	}

	_lid = (zzV.zAid - 1 + zzV.zigLimit) % zzV.zigLimit;

	rv = getNextZZ(zzV._zigzagA, 0, _lid, zzV.zAper, 0, zval);	// , lid);
	if (rv) {
		shiftXABCD(-1, zval);
	}

	int a_d = zzV.x_dA;
	a_d = (zzV.x_dA - 1 + 5) % 5 + 1;
	zzV.dA = zzV._xabcdA[a_d];
	a_d = a_d - 1;
	if (a_d < 1) {
		a_d = 5;
	}
	zzV.cA = zzV._xabcdA[a_d];

	a_d = zzV.x_dB;
	a_d = (zzV.x_dB - 1 + 5) % 5 + 1;
	zzV.dB = zzV._xabcdB[a_d];
	a_d = a_d - 1;
	if (a_d < 1) {
		a_d = 5;
	}
	zzV.cB = zzV._xabcdB[a_d];

	return true;
}*/

//--------------------------------------------------------------------------------------
/*int Strategy::checkEntry(int ax, float _rate) {
	int bx = ax;
	int retVal = 0;
	float cA = zzV.cA;
	float dA = zzV.dA;
	float cB = zzV.cB;
	float dB = zzV.dB;

	float fibrangeA = 0.0f;
	float fibrangeB = 0.0f;
	float fibrange2 = 0.0f;

	float trigpoint = 0.0f;
	float entrypoint = 0.0f;
	float tgtpoint = 0.0f;
	float d2 = 0.0f;

	float maSpread = (*(SRC))[bx - 1].aclose - (*(SRC))[bx - 1].close;
	//comparatoru = (*(SRC))[bx - 1].close > MALine[bx - 1];
	//comparatord = (*(SRC))[bx - 1].aclose < MALine[bx - 1] + maSpread;

	if (dA == 0.0) {
		return retVal;
	}

	// First look for an entry on anticipated reversal before a new pivot is set if 
	// it exceeds 50% trigger of the fibrange of the current 2 zigzag points
	// As a high is last ZZ, look for a long after it drops below 50% expecting low zz to appear soon
	fibrangeA = std::abs(dA - cA);
	fibrangeB = std::abs(dB - cB);

	if (dA > cA) { // a high zig is set - wait for price to close below the trigger
		trigpoint = cA + (fibrangeA * _rate); // i.e. 50% point of range dA is high ZZ, cA is low ZZ + rate

		if ((*(SRC))[ax].aclose < trigpoint) // ASKsource.close[ax]
		{

			d2 = min((*(SRC))[ax].alow, (*(SRC))[ax - 1].alow);	// ASKsource.low[ax - 1]);
			float d2b = min((*(SRC))[bx].low, (*(SRC))[bx - 1].low); //BIDsource.low[bx], BIDsource.low[bx - 1]);

			fibrange2 = std::abs(dA - d2);
			float fibrange2b = std::abs(dB - d2b);
			entrypoint = d2 + (fibrange2 * zzV.fibEntry); // based on ASK for entry


			if ((*(SRC))[ax].ahigh > (*(SRC))[ax - 1].ahigh && (*(SRC))[ax].aclose > entrypoint) 
			{
				tgtpoint = d2b + (fibrange2b * zzV.fibTgt);


				if (tgtpoint - (*(SRC))[bx].close >= regVals.fixedTgt) 
				{
					if (regVals.useFixedTgt) 
					{
						tgtpoint = (*(SRC))[bx].close + regVals.fixedTgt;
					}

					Entry = (*(SRC))[ax].aclose;
					if (Entry > MALine[bx - 1] + maSpread)	// indVals.mauVal[ax])
					{
						setUpTargetL = tgtpoint;
						stptgt = d2b;
						retVal = 1;
					}
				}
			}
		}
		else 
		{
			entrypoint = dB - (fibrangeB * zzV.fibEntry);


			if ((*(SRC))[bx].low < (*(SRC))[bx-1].low && (*(SRC))[bx].close < entrypoint) 
			{
				tgtpoint = dA - (fibrangeA * zzV.fibTgt);


				if ((*(SRC))[ax].aclose - tgtpoint >= regVals.fixedTgt) 
				{
					if (regVals.useFixedTgt) 
					{
						tgtpoint = (*(SRC))[ax].aclose - regVals.fixedTgt;
					}

					Entry = (*(SRC))[bx].close;
					if (Entry < MALine[bx])	// + maSpreadindVals.madVal[bx]) {
					{
						setUpTargetS = tgtpoint;
						stptgt = dA;
						retVal = -1;
					}

				}
			}
		}
	}

	if (dB < cB) { // a low zig is set - wait for price to close above the trigger
		trigpoint = dB + (fibrangeB * _rate); // i.e. 50% of range

		if ((*(SRC))[bx].close > trigpoint) {

			d2 = max((*(SRC))[bx].high, (*(SRC))[bx - 1].high);
			float d2a = max((*(SRC))[ax].ahigh, (*(SRC))[ax - 1].ahigh);

			fibrange2 = std::abs(d2 - dB);
			float fibrange2a = std::abs(dA - d2a);
			entrypoint = d2 - (fibrange2 * zzV.fibEntry); // based on BID for entry


			if ((*(SRC))[bx].low < (*(SRC))[bx - 1].low && (*(SRC))[bx].close < entrypoint) 
			{
				tgtpoint = d2a - (fibrange2a * zzV.fibTgt);


				if ((*(SRC))[ax].aclose - tgtpoint >= regVals.fixedTgt) 
				{
					if (regVals.useFixedTgt) 
					{
						tgtpoint = (*(SRC))[ax].aclose - regVals.fixedTgt;
					}

					Entry = (*(SRC))[bx].close;
					if (Entry < MALine[bx])	// + maSpreadindVals.madVal[bx]) 
					{
						setUpTargetS = tgtpoint;
						stptgt = d2a;
						retVal = -1;
					}
				}
			}
		}
		else 
		{
			entrypoint = dA + (fibrangeA * zzV.fibEntry);

			if ((*(SRC))[ax].ahigh > (*(SRC))[ax - 1].ahigh && (*(SRC))[ax].aclose > entrypoint) 
			{
				tgtpoint = dB + (fibrangeB * zzV.fibTgt);


				if (tgtpoint - (*(SRC))[bx].close >= regVals.fixedTgt) 
				{
					if (regVals.useFixedTgt) 
					{
						tgtpoint = (*(SRC))[bx].close + regVals.fixedTgt;
					}

					Entry = (*(SRC))[ax].aclose;
					if (Entry > MALine[bx] + maSpread)	//indVals.mauVal[ax]) 
					{
						setUpTargetL = tgtpoint;
						stptgt = dB;
						retVal = 1;
					}

				}
			}
		}
	}

	return retVal;
}
*/

//--------------------------------------------------------------------------------------
/*not required as linreg pre calculated
void Strategy::updateLinReg(period)
{
	//--get highest, lowest
	float minv, maxv, minpos, maxpos = mathex.minmax(BIDsource, period - N + 1, period);
--get avg close
local cl = mathex.avg(BIDsource.close, period - N + 1, period);
local avg1 = avg(maxv, minv);
local avg2 = avg(avg1, cl);

regress[period] = BIDsource.close[period] - avg2;
--get reg

local b, m = getRegression(N, regress, period);

RL[period] = b;
end
*/

//--------------------------------------------------------------------------------------
float Strategy::lasthigh(int prd)	//--return the first high / low moving back from start bar with 2 bars before lower / higher
{
	float hi = 0.0f;
	int t = 0;
	bool found = false;
	while (found == false) 
	{
		if ((*(SRC))[prd - t - 1].ahigh < (*(SRC))[prd - t].ahigh && (*(SRC))[prd - t - 2].ahigh < (*(SRC))[prd - t].ahigh)
		{
			hi = (*(SRC))[prd - t].ahigh; found = true;
		}
		t = t + 1;
	}
	return hi;
}

//--------------------------------------------------------------------------------------
float Strategy::lastlow(int prd)
{
	float lo = 0.0f;
	int t = 0;
	bool found = false;
	while (found == false)
	{
		if ((*(SRC))[prd - t - 1].low > (*(SRC))[prd - t].low && (*(SRC))[prd - t - 2].low > (*(SRC))[prd - t].low)
		{
			lo = (*(SRC))[prd - t].low; found = true;
		}
		t = t + 1;
	}
	return lo;
}



//--------------------------------------------------------------------------------------
int	Strategy::checkLinRegEntry(int ix)
{
	int retVal = 0;

	float entrypoint = 0.0f;
	float tgtpoint = 0.0f;
	int pos = 0;
	int UD = 0;

	bool entryUp = false;
	bool entryDown = false;
	bool b3Up = false;
	bool b3Dn = false;
	bool aggUp = false;
	bool aggDown = false;

	float _Entry = 0.0f;	//so the target distance is correct

	//--CHANGE THIS - CHECK THE LINEAR REGRESSION VALUE AND WHERE IT IS IN RELATION TO THE + / -20 LINES
	//	--IF CHANGING TO UP(VALUE > VALUE - 1) THEN BUY AND v.v.FOR SELL
	//	--STOP IS AT RECENT SWING HIGH, TARGET 1.5R or variation based in price closing past the R value

/*
	if ((regress.reguVal[ix] - regress.reguVal[ix - 1] > regress.regDiff) &&
		regress.reguVal[ix - 2] > regress.reguVal[ix - 1] &&
		abs(regress.reguVal[ix]) > 20.0)
	{
		UD = 1;
		if (regress.useRegLine == true)
		{
			UD = getRegUpDn(1, regress.regLineType, regress.regLineLen, ix, 0);
		}
		//--set up for a BUY.
		//--add other restrictions such as + / -20 line
		Entry = (*(SRC))[ix].aclose;	// ASKsource.close[ax];
		stptgt = lastlow(ix);	//--find the recent swing low...2 bars higher lows to left
		stopPips = Entry - stptgt;
		//--if (stopPips <= 10.0) then stptgt = Entry - 10.0; end
		if (stopPips >= minStop && stopPips < stopMax)
		{
			rStop = Entry + (stopPips * rVal);
			setUpTargetL = Entry + (stopPips * (rVal * 10)); //--rVal - fixed 10 rVal target
			retVal = 1;
		}
	}

	if ((regress.regdVal[ix - 1] - regress.regdVal[ix] > regress.regDiff) &&
		regress.regdVal[ix - 2] < regress.regdVal[ix - 1] &&
		abs(regress.regdVal[ix]) > 20.0)
	{
		UD = -1;
		if (regress.useRegLine == true)
		{
			UD = getRegUpDn(-1, regress.regLineType, regress.regLineLen, ix, 0);
		}
		//--set up for a SELL.
		Entry = (*(SRC))[ix].close;	// BIDsource.close[bx];
		stptgt = lasthigh(ix);	//--find the recent swing high...2 bars lower highs to left
		stopPips = stptgt - Entry;
		//--if (stopPips < 10.0) then stptgt = Entry + 10.0; end
		if (stopPips >= minStop && stopPips < stopMax)
		{
			rStop = Entry - (stopPips * rVal);
			setUpTargetS = Entry - (stopPips * (rVal * 10)); //--force a max 10 rVal target
			retVal = -1;
		}
	}
*/

	aggUp = (((regress.reguVal[ix] - regress.reguVal[ix - 1]) > regress.regDiff2 &&
		regress.reguVal[ix - 2] < regress.reguVal[ix - 1]) ||
		((regress.reguVal[ix] - regress.reguVal[ix - 1]) > regress.regDiff2 &&
			(regress.reguVal[ix - 1] - regress.reguVal[ix - 2]) > regress.regDiff) &&
		regress.reguVal[ix - 3] < regress.reguVal[ix - 2]) &&
		(abs(regress.reguVal[ix]) > regress.regLineLimit &&
			abs(regress.reguVal[ix - 1]) < regress.regLineLimit);

	aggDown = (((regress.regdVal[ix - 1] - regress.regdVal[ix]) > regress.regDiff2 &&
		regress.regdVal[ix - 2] > regress.regdVal[ix - 1]) ||
		((regress.regdVal[ix - 1] - regress.regdVal[ix]) > regress.regDiff2 &&
			(regress.regdVal[ix - 2] - regress.regdVal[ix - 1]) > regress.regDiff) &&
		regress.regdVal[ix - 3] > regress.regdVal[ix - 2]) &&
		(abs(regress.regdVal[ix]) > regress.regLineLimit &&
			abs(regress.regdVal[ix - 1]) < regress.regLineLimit);

	if (entryVals.useAgg == false)
	{
		aggUp = false;
		aggDown = false;
	}


	entryUp = ((regress.reguVal[ix] - regress.reguVal[ix - 1] > regress.regDiff) &&
		regress.reguVal[ix - 2] > regress.reguVal[ix - 1] &&
		abs(regress.reguVal[ix]) > regress.regLineLimit);

	entryDown = ((regress.regdVal[ix - 1] - regress.regdVal[ix] > regress.regDiff) &&
		regress.regdVal[ix - 2] < regress.regdVal[ix - 1] &&
		abs(regress.regdVal[ix]) > regress.regLineLimit);

	//--compare current && bar - 2 difference..prior bars as main entry method but diff < 2 * regDiff
	if (entryVals.use3Bar == true)
	{
		float bdif = regress.reguVal[ix] - regress.reguVal[ix - 2];
		b3Up = bdif > regress.regDiff && bdif<(regress.regDiff * 2) &&
			regress.reguVal[ix - 3] > regress.reguVal[ix - 2] &&
			abs(regress.reguVal[ix - 1]) > regress.regLineLimit;
	}

	//--compare current && bar - 2 difference..prior bars as main entry method but diff < 2 * regDiff
	if (entryVals.use3Bar == true)
	{
		float bdif = regress.regdVal[ix - 2] - regress.regdVal[ix];
		b3Dn = bdif > regress.regDiff && bdif < (regress.regDiff * 2) &&
			regress.regdVal[ix - 3] < regress.regdVal[ix - 2] &&
			abs(regress.regdVal[ix - 1]) > regress.regLineLimit;
	}

	if (entryUp == true || b3Up == true || aggUp == true)
	{
		//--set up for a BUY.
		//--add other restrictions such as + / -20 line(regress.regLineLimit)
		UD = 1;
		if (regress.useRegLine == true)
		{
			UD = getRegUpDn(1, regress.regLineType, regress.regLineLen, ix, 0);
		}

		//Entry = (*(SRC))[ix].aclose;
		Entry = (*(SRC))[ix].close;
		stptgt = lastlow(ix);	//--find the recent swing low...2 bars higher lows to left
		stopPips = Entry - stptgt;
		//--if (stopPips <= 10.0) then stptgt = Entry - 10.0; end
		if (stopPips >= minStop && stopPips < stopMax && UD == 1)
		{
			rStop = Entry + (stopPips * rVal);
			setUpTargetL = Entry + (stopPips * (rVal * 10)); //--rVal - fixed 10 rVal target
			retVal = 1;
		}
	}

	if (entryDown == true || b3Dn == true || aggDown == true)
	{	//--set up for a SELL.
		UD = -1;
		if (regress.useRegLine == true)
		{
			UD = getRegUpDn(-1, regress.regLineType, regress.regLineLen, ix, 0);
		}
		//--set up for a SELL.
		//Entry = (*(SRC))[ix].close;	// BIDsource.close[bx];
		Entry = (*(SRC))[ix].aclose;
		stptgt = lasthigh(ix);	//--find the recent swing high...2 bars lower highs to left
		stopPips = stptgt - Entry;
		//--if (stopPips < 10.0) then stptgt = Entry + 10.0; end
		if (stopPips >= minStop && stopPips < stopMax && UD == -1)
		{
			rStop = Entry - (stopPips * rVal);
			setUpTargetS = Entry - (stopPips * (rVal * 10)); //--force a max 10 rVal target
			retVal = -1;
		}
	}
	
	return retVal;
}


//--------------------------------------------------------------------------------------
void Strategy::checkForSetup(int period)
{
	//	float nh = 0.0;
	//	float nl = 0.0;

		//float bidx = BIDsource:size() - 1;
		//float aidx = ASKsource:size() - 1;

	float spread = TICKSRC[0].ask - TICKSRC[0].bid;
	//	bool atrOK = true;

	float sdist = 0.0f;

	int _Entry = 0;


	//float *hmaSlope=nullptr;
	//bool regOK = true;
/*	if (useATR == true)
	{
		atrOK = false;
		if (atrVal[period] > 0.0005) atrOK = true;
	}
*/
/*	if (useReg == true)
	{
		float* mval=nullptr;
		float radeg = (180 / 3.14159265f);
		getRegression(RD, spread, period, *hmaSlope, *mval );
		float angle = (std::atan((*mval)*radeg)) / (Point * 10);
		if (std::abs(angle) > 35)
			regOK = false;
	}
*/
/*
	nh = getLastSpike(1);
	nl = getLastSpike(-1);
	if (nh > 0.0)
	{
		if (period - 3 > highPos)
		{
			if (highPos > highPos1)
			{
				highVal1 = highVal;
				highPos1 = highPos;
				//--core.host:trace(string.format("ASK HighVal1 %f, HighVal %f ", highVal1, highVal));
			}
			highVal = nh;
			highPos = period - 3;
		}
	}
	if (nl < 999999.0)
	{
		if (period - 3 > lowPos)
		{
			if (lowPos > lowPos1)
			{
				lowVal1 = lowVal;
				lowPos1 = lowPos;
				//--core.host:trace(string.format("BID LowVal1 %f, LowVal %f ", lowVal1, lowVal));
			}
			lowVal = nl;
			lowPos = period - 3;
		}
	}
*/

	//getXABCD();

	if (newBar == true)
	{
		//_Entry = checkEntry(idx - 1, zzV.trigRate);
		_Entry = checkLinRegEntry(idx - 1);

		if (_Entry == 1)
		{
			buySU = true;
			state = 2;
			rReached = false;
		}
		if (_Entry == -1)
		{
			sellSU = true;
			state = 2;
			rReached = false;
		}

	}


/*
//increase the counts first.
	if (newBar == true && _hasCrossed==false)
	{
		crossed = hasCrossed(period);
		if (crossed != 0)	//--and crossed~= maOrder) then
		{
			maOrder = crossed;
			_hasCrossed = true;
			crossedCount = 0;
		}
		//--linesInLimits = false;
//		core.host:trace(string.format("Has Crossed trigger %f", triggerLevel));
	}


	if (newBar == true && buySU == false && sellSU == false
		&& _hasCrossed == true)			//--and linesInLimits == true) then
	{
//		entryDir = validArrakis(period, maOrder);
		if (entryDir > 0) buySU = true;
		if (entryDir < 0) sellSU = true;
	}
*/

/*
	//MAYBE CHECK THE PULLBACK DOES NOT GO BEYOND THE TRIGGER BAR HIGH / LOW
	if (state > 0 && newBar == true)
	{
		if (buySU == true && (*(SRC))[idx - 1].close < (*(SRC))[touchBar].low)
			state = 0;
	//	core.host:trace(string.format("Buy trigger bar breach"));

		if (sellSU == true && (*(SRC))[idx - 1].aclose > (*(SRC))[touchBar].ahigh)
			state = 0;
	//	core.host:trace(string.format("Sell trigger bar breach"));
	}
*/

/*/
//	---------------------------- - Setup
	if (state == 0 && entryDir != 0)
	{
		if (entryDir > 0)	// then--and validTriggerBar(true, false, aidx, bidx) == true) then
		{
			rReached = false;
			buySU = true;
			//--touchBar = aidx - 1;
			state = 1;
			entryDir = 0;
			trgrCount = 0;
			//core.host:trace(string.format("0 state -> Buy state %i %f", state, ASKsource.close[aidx - 1]));
		}


		if (entryDir < 0)	//then--and validTriggerBar(false, true, aidx, bidx) == true) then
		{
			rReached = false;
			sellSU = true;
			//--touchBar = bidx - 1;
			state = 1;
			entryDir = 0;
			trgrCount = 0;
			//core.host:trace(string.format("0 state -> Sell state %i %f", state, BIDsource.close[bidx - 1]));
		}
	}
*/

//	---------------------------- - Enter
	if (state == 2)
	{
		//--if we get this far we are ready to enter a trade
		if (buySU == true)
		{
			possBuy = true;
			setUpPriceL = (*(SRC))[idx].aclose;	//--entry is at ASK so adjust accordingly
			Entry = setUpPriceL;

/*			if (regVals.useHedging == false)
			{
				setUpStopL = setUpVals.lowVal;	//--used as a hedge here
				sdist = Entry - setUpStopL;
				if (sdist < minStop)
				{
					//--set initial distance, { floor, { reset sdist
					setUpStopL = _floor(Entry - minStop, pres, 5);
					sdist = Entry - setUpStopL;	//--should be at least minStop(10) pips
				}
				//--		setUpTargetL = Entry + (sdist * _rv);
			}
*/
			//rStop = Entry + (sdist * berVal);	//--point at which 1R is trapped
			r1Stop = Entry + 2.0f;	//--sdist;
			brkEven = false;
			brkEvenSet = false;
			//--touchBar = -1;
			//--]]
			buySU = false;
			_hasCrossed = false;
			//--linesInLimits = false;
			entryDir = 0;
			state = 3;
			//core.host:trace(string.format("Buy Triggered %f Spread %f Ask %f ", ASKsource.close[aidx], spread, Entry));
			//--core.host:trace(string.format("Stop %f Target %f", setUpStopL, setUpTargetL));
		}
	

		if (sellSU == true) 
		{
			possSell = true;
			setUpPriceS = (*(SRC))[idx].close; 	//--entry is at BID so adjust accordingly
			Entry = setUpPriceS;

/*			if (regVals.useHedging == false)
			{
				setUpStopS = setUpVals.highVal;
				sdist = setUpStopS - Entry;
				if (sdist < minStop) 
				{
					//--set initial distance, { ceil, { reset sdist
					setUpStopS = _ceil(Entry + minStop, pres, 5);
					sdist = setUpStopS - Entry;	//--should be at least minStop(10) pips
				}
			}
*/
			//--			setUpTargetS = Entry - (sdist * _rv);
			//rStop = Entry - (sdist * berVal);	//--point at which 1R is trapped
			r1Stop = Entry - 2.0f;	//--sdist;
			brkEven = false;
			brkEvenSet = false;
			//--touchBar = -1;

			sellSU = false;
			_hasCrossed = false;
			//--linesInLimits = false;
			entryDir = 0;
			state = 3;
			//core.host:trace(string.format("Sell Triggered %f Spread %f Bid %f ", BIDsource.close[bidx], spread, Entry));
			//--core.host:trace(string.format("Stop %f Target %f", setUpStopS, setUpTargetS));
		}
	}

/*
	//-------------------------- - look for the trigger price to be reached
	if (state == 2)
	{
		//if we get this far we are ready to enter a trade

		if (buySU == true)
		{
			possBuy = true;
			setUpPriceL = (*(SRC))[idx - 1].aclose;	//entry is at ASK so adjust accordingly
			Entry = setUpPriceL;

			setUpStopL = lowVal;
			sdist = Entry - setUpStopL;
			if (sdist < minStop)
			{	//set initial distance, then floor, then reset sdist
				setUpStopL = _floor(Entry - minStop, pres, 5);
				sdist = Entry - setUpStopL;	//should be at least minStop(10) pips
			}
			//setUpTargetL = Entry + (sdist);// *_rv);
			
			rStop = Entry + (sdist * berVal);	//--point at which 1R is trapped
			r1Stop = Entry + 2.0f;	// sdist;
			brkEven = false;
			brkEvenSet = false;
//			touchBar = -1;
			buySU = false;
			_hasCrossed = false;
			entryDir = 0;
			state = 3;
		}

		if (sellSU == true)
		{
			possSell = true;
			setUpPriceS = (*(SRC))[idx - 1].close;	//entry is at BID so adjust accordingly
			Entry = setUpPriceS;
			setUpStopS = highVal;
			sdist = setUpStopS - Entry;
			if (sdist < minStop)
			{	//set initial distance, then ceil, then reset sdist
				setUpStopS = _ceil(Entry + minStop, pres, 5);
				sdist = setUpStopS - Entry;	//should be at least minStop(10) pips
					//if (sdist < minStop) { sdist = minStop; setUpStopS = Entry + sdist; }
			}
			//setUpTargetS = Entry - (sdist);	// * _rv);
			rStop = Entry - (sdist * berVal);	//--point at which 1R is trapped
			r1Stop = Entry - 2.0f;	// sdist;
			brkEven = false;
			brkEvenSet = false;
			//touchBar = -1;
			sellSU = false;
			_hasCrossed = false;
			entryDir = 0;
			state = 3;
		}
	}
*/

/*	//-------------------------- - set the stop && entry.Adjust if subsequent bars
	if (state >= 2 && newBar == true)
	{
		if (buySU == true)
		{
			highVal = (*(SRC))[idx - 1].high + 0.00001f; 	//entry level
			lowVal = (*(SRC))[idx - 1].low - 0.00005f;		//stop level
			if (highVal - lowVal < 0.0005f) lowVal = highVal - 0.00051f; // offset for the 1 point diff from the high too
			state = 3;
			//core.host:trace(string.format("Buy state %i %f %f", state, setUpVals.highVal, setUpVals.lowVal));
		}

		if (sellSU == true)
		{
			lowVal = ((*(SRC))[idx - 1].alow) - 0.00001f; 	//entry level
			highVal = ((*(SRC))[idx - 1].ahigh) + 0.00005f;		//stop level
			if (highVal - lowVal < 0.0005f) highVal = lowVal + 0.00051f; // offset for the 1 point diff from the high too
			state = 3;
			//core.host:trace(string.format("Sell state %i %f", state, setUpVals.lowVal, setUpVals.highVal));
		}
	}

	//-------------------------- Has the next bar retraced
	if (state == 1 && newBar == true)
	{
		if (buySU == true && (*(SRC))[idx - 1].close < (*(SRC))[idx - 1].open)
		{
			state = 2;
			pbCount = 0;
			//core.host:trace(string.format("Buy state %i", state));
		}

		if (sellSU == true && (*(SRC))[idx - 1].aclose > (*(SRC))[idx - 1].aopen)
		{
			state = 2;
			pbCount = 0;
			//core.host:trace(string.format("Sell state %i", state));
		}
	}
	*/
/*
	if (state == 0 && newBar == true && atrLine[idx - 1] > 0.003)
	{
		//Look for a volume spike at least x1.7 of previous bar
		if ((*(SRC))[idx - 1].volume > (*(SRC))[idx - 2].volume * vMul && (*(SRC))[idx - 1].close > (*(SRC))[idx - 1].open
			// && BIDsource.volume[bidx - 3] > BIDsource.volume[bidx - 2] && BIDsource.volume[bidx - 4] > BIDsource.volume[bidx - 3] && BIDsource.volume[bidx - 5] > BIDsource.volume[bidx - 4]
			&& bigWickRange(1, idx - 1, 2) == false
			)
		{
			//			setUpVals.highVal = BIDsource.high[bidx - 1] + 0.00001;
			buySU = true;
			touchBar = idx - 1;
			state = 1;
			trgrCount = 0;
			//core.host:trace(string.format("0 state -> Buy state %i %f  %f %f", state, BIDsource.high[bidx - 1], BIDsource.volume[bidx - 1], BIDsource.volume[bidx - 2]));
		}


		if ((*(SRC))[idx - 1].volume > (*(SRC))[idx - 2].volume * vMul && (*(SRC))[idx - 1].aclose < (*(SRC))[idx - 1].aopen
			//&& ASKsource.volume[aidx - 3]>ASKsource.volume[aidx - 2] && ASKsource.volume[aidx - 4] > ASKsource.volume[aidx - 3] && ASKsource.volume[aidx - 5] > ASKsource.volume[aidx - 4]
			&& bigWickRange(-1, idx - 1, 2) == false
			)
		{
			//			setUpVals.lowVal = ASKsource.low[aidx - 1] - 0.00001;
			sellSU = true;
			touchBar = idx - 1;
			state = 1;
			trgrCount = 0;
			//core.host:trace(string.format("0 state -> Sell state %i %f  %f %f", state, ASKsource.high[aidx - 1], ASKsource.volume[aidx - 1], ASKsource.volume[aidx - 2]));
		}
	}
*/
}

//------------------------------------------------------------------

/*
//--------------------------------------------------------------------------------------
bool Strategy::bigWickRange(int _dir, int _idx, float _mul)
{
//is the wick running opposite to desired direction or strong full body 90 % of range
float bod = 0.0f;
float wick = 0.0f;
float range = 0.0f;
float ohilo = 0.0f;
bool retVal = false;

float spread = TICKSRC[0].ask - TICKSRC[0].bid;

//if (debugVars.use3Wick == true) then
	if (_dir == 1)
	{
		range = (*(SRC))[_idx].high - (*(SRC))[_idx].low;
		if (range == 0.0) range = 0.00001f;	//minimum range of 1 point to avoid div by 0

		//if a buy then don't want a large top wick
		bod = abs((*(SRC))[_idx].close - (*(SRC))[_idx].open);
		wick = (*(SRC))[_idx].high - max((*(SRC))[_idx].close, (*(SRC))[_idx].open);
		//ohilo = BIDsource.open[_idx] - BIDsource.low[_idx];
	}

	if (_dir == -1)
	{
		range = ((*(SRC))[_idx].high + spread) - ((*(SRC))[idx].low + spread);
		if (range == 0.0) range = 0.00001f;	//minimum range of 1 point to avoid div by 0

		//if a sell then don't want a large bottom wick
		bod = abs(((*(SRC))[_idx].open + spread) - ((*(SRC))[_idx].high + spread));
		wick = min(((*(SRC))[_idx].close + spread), ((*(SRC))[_idx].open + spread)) - (*(SRC))[_idx].low + spread;
		//ohilo = ASKsource.high[_idx] - BIDsource.open[_idx];
	}

//	if (state == 0 && wick - (bod * _mul) > 0.0) then
//		retVal = true;
//core.host:trace(string.format("Spider rejection"));
//end

	if (state < 3 && bod != 0.0 && bod / range >= 0.95)
	{
		if (_dir == 1 && (*(SRC))[_idx].open > (*(SRC))[_idx].close)
		{
			retVal = true;
			//core.host:trace(string.format("Range rejection Buy"));
		}
		if (_dir == -1 && (*(SRC))[_idx].open + spread < (*(SRC))[_idx].close + spread)
		{
			retVal = true;
			//core.host:trace(string.format("Range rejection Sell"));
		}
	}

	return retVal;
//end
}
*/

//------------------------------------------------------------------
void Strategy::checkForOpen()
{
	dailyBalance = acct.Balance;

	if (possBuy == true)
	{
		entryTime = TICKSRC[0].dtime;
//		stptgt = setUpStopL;
		setTargets(1);
		orderPending = true;
//		profited = false;
		inProfit = false;
		enter(1);
		//lastStop = stopTarget;
		//lastProfit = profitTarget;
		tradePlaced = true;
		_hasCrossed = false;
		possBuy = false;
		state = 4;
		attempted = true;
	}
	if (possSell == true)
	{
		entryTime = TICKSRC[0].dtime;
//		stptgt = setUpStopS;
		setTargets(-1);
		orderPending = true;
//		profited = false;
//		inProfit = false;
		enter(-1);
//		lastStop = stopTarget;
//		lastProfit = profitTarget;
		tradePlaced = true;
		_hasCrossed = false;
		possSell = false;
		state = 4;
		attempted = true;
	}
}

double Strategy::getPL()	//daily pl calculation
{
	double retVal = acct._PL;
	//need to store the start balance a change of day and compare to current balance. 
	/*
	local enum, row;
	local pnl = 0.0;
	local sesdt = 0.0;

	enum = core.host :findTable("Closed Trades") : enumerator();
	row = enum :next();

	while row~= nil do
		sesdt = core.host:execute("convertTime", core.TZ_EST, core.TZ_LOCAL, row.OpenTime);
	if (row.AccountID == ACCOUNT and row.OfferID == OFFER and sesdt >= session.dayStart) then
		pnl = pnl + row.GrossPL;

	--core.host:trace(string.format("Trade %s PL %f %f %f", row.TradeID, row.GrossPL, sesdt, session.dayStart));
	end
		row = enum :next();
	end
		return pnl;
		*/
	return retVal;
}

//------------------------------------------------------------------
/*void Strategy::partialExit(float price)
{
	int newLots = acct.lots;
	if (newLots >= ContractSize * 2 && partTaken == false)
	{
		newLots = (int)_round((float)((newLots / ContractSize) / 2), 0)*ContractSize;
		close(acct.lots - newLots, price);
		partTaken = true;
	}
}
*/

//------------------------------------------------------------------
void Strategy::checkForTarget()
{
	string type = "";
	int side = 0;
	bool changed = false;
	float newStop = 0.0f;
	float newProf = 0.0f;

	TickData td = TICKSRC[0];
	bool forceUpdate = false;

	/*
		//--set profited status
			inProfit = false;
		if (acct.BuySell > 0 && td.bid > openPrice ||
			acct.BuySell < 0 && td.ask < openPrice) inProfit = true;
	*/

	/*
		//close half the position
		if (allowPartial == true)
		{
			if (partTaken == false && abs(acct.openPrice - td.bid) >= partialPips)
			{
				newStop = acct.openPrice;
				newProf = acct.limit;
				changed = true;
				float price = td.bid;
				if(acct.BuySell==1) price = td.ask;
				partialExit(price);
			}
		}

		//Trail the stops and put target out of reach
		if (priorBarTrail == true && changed==false)
		{
			if (acct.BuySell < 0 && (acct.openPrice - td.bid >= priorTrailPips))
			{
				float nsh = 1; //DCMFIX max(HAHigh[idx - 2], HAHigh[idx - 1]);
				if (nsh < lastStop && nsh>td.bid)
				{
					newStop = nsh;
					newProf = 1; //DCMFIX  HAClose[idx - 1] - 0.0100f;	//put it 100 pips away
					changed = true;
					type = "Sell Trailing pre bar";
					side = -1;
				}
			}

			if (acct.BuySell > 0 && (td.bid - acct.openPrice >= priorTrailPips))
			{
				float nsl = 1; //DCMFIX  min(HALow[idx - 2], HALow[idx - 1]);
				if (nsl > lastStop && nsl < td.bid)
				{
					newStop = nsl;
					newProf = 1; //DCMFIX  HAClose[idx - 1] + 0.0100f;	//put it 100 pips away
					changed = true;
					type = "Buy Trailing pre bar";
					side = 1;
				}
			}
		}
	*/

	/*	if (wlRatio >= 2.0) //--if price is >= 70 % of target move stop to 1.5:1 price
		{
			if (acct.BuySell > 0)
			{
				float _to = lastProfit - openPrice;
				float _tc = lastProfit - td.bid;		//BIDsource.close[bidx];
				//--core.host:trace(string.format("_tc %f, _to %f  pct %f current %f", _tc, _to, (_tc / _to) * 100.0, BIDsource.close[bidx]));
				if ((_tc / _to) * 100.0 <= 30.0)  brkEven = true;
			}
			if (buySell < 0)
			{
				float _to = openPrice - lastProfit;
				float _tc = td.ask- lastProfit;	//
				//--core.host:trace(string.format("_tc %f, _to %f  pct %f current %f", _tc, _to, (_tc / _to) * 100.0, ASKsource.close[aidx]));
				if ((_tc / _to) * 100.0 <= 30.0) brkEven = true;
			}
		}
	*/

	/*
		if (brkEven == false)
		{
			if (acct.BuySell > 0)
			{
				float _to = lastProfit - openPrice;
				float _tc = lastProfit - td.bid;		// BIDsource.close[bidx];
				//--core.host:trace(string.format("_tc %f, _to %f  pct %f current %f", _tc, _to, (_tc / _to) * 100.0, BIDsource.close[bidx]));
				if ((_tc / _to) * 100.0 <= 10.0) brkEven = true;
			}
			if (buySell < 0)
			{
				float _to = openPrice - lastProfit;
				float _tc = td.ask - lastProfit;		//ASKsource.close[aidx]
				//--core.host:trace(string.format("_tc %f, _to %f  pct %f current %f", _tc, _to, (_tc / _to) * 100.0, ASKsource.close[aidx]));
				if ((_tc / _to) * 100.0 <= 10.0) brkEven = true;
			}
		}

		if (brkEven == true && brkEvenSet == false)
		{
			//--move the stop to breakeven + 2 pips or 1.5:1 price
			if (buySell > 0)
			{
				newStop = openPrice + 0.0002f;
				if (wlRatio >= 2.0f)	newStop = openPrice + (stopPips * 1.5f);
				brkEvenSet = true;
				brkEven = true;
				newProf = lastProfit;
				lastStop = 0.0f;
				side = acct.BuySell;
				forceUpdate = true;
				changed = true;
			}


			if (buySell < 0)
			{
				newStop = openPrice - 0.0002f;
				if (wlRatio >= 2.0f) newStop = openPrice - (stopPips * 1.5f);
				brkEvenSet = true;
				brkEven = true;
				newProf = lastProfit;
				lastStop = 0.0f;
				side = acct.BuySell;
				forceUpdate = true;
				changed = true;
			}
		}
	*/

	/*
		if ((brkEvenOverrun == true) && brkEven == true && brkEvenSet == false)
		{
			if (acct.BuySell > 0)
			{
				newStop = r1Stop;
				brkEvenSet = true;
				//--brkEven = true;
				newProf = lastProfit;
				lastStop = 0;
				side = acct.BuySell;
				//trailing = true;
				forceUpdate = true;
				changed = true;
			}

			if (acct.BuySell < 0)
			{
				newStop = r1Stop;
				brkEvenSet = true;
				//--brkEven = true;
				newProf = lastProfit;
				lastStop = 0;
				side = acct.BuySell;
				//trailing = true;
				forceUpdate = true;
				changed = true;
			}
		}
	*/

	//------------------------------------------------------------------------------

		//--move the stop to R if the bar closes beyond R and set next trigger to R + 0.5
		//--THE STOP NEEDS TO BE FAR ENOUGH AWAY FROM CURRENT PRICE - CHECK BID / ASK BEING USED CORRECTLY
	if (orderPending == false && newBar == true || failedStop == true)
	{
		//--core.host:trace(string.format("Failed stop retry %f  %f  %f", rStop, BIDsource.close[bidx], lastStop));

		if (acct.BuySell > 0 && (*(SRC))[idx].close > rStop)
		{
			float rdiff = (float)floor((((*(SRC))[idx].close - Entry) - 2.0f) / stopPips);
			//--how far is price from the starting rVal
			//--e.g. if price has closed 3.2R and rVal is 1 need to calc the nearest rVal which is 3.0
			newStop = Entry + (stopPips * rdiff);
			if (newStop > lastStop)
			{
				tryStop = newStop;
				newProf = lastProfit;
				rStop = Entry + (stopPips * rdiff);
				side = acct.BuySell;
				lastStop = 0;
				forceUpdate = true;
				changed = true;
				failedStop = true;
				//core.host:trace(string.format("Trailing RVal %f  c %f  ns %f  ls%f", rdiff, BIDsource.close[bidx], newStop, lastStop));
			}
		}

		if (acct.BuySell < 0 && (*(SRC))[idx].aclose < rStop)
		{	//--how far is price from the starting rVal
			//--e.g. if price has closed 3.2R and rVal is 1 need to calc the nearest rVal which is 3.0
			float rdiff = (float)floor(((Entry - (*(SRC))[idx].aclose) + 2.0f) / stopPips);
			newStop = Entry - (stopPips * rdiff);
			if (newStop < lastStop)
			{
				tryStop = newStop;
				newProf = lastProfit;
				rStop = Entry - (stopPips * (rdiff));
				side = acct.BuySell;
				lastStop = 0;
				forceUpdate = true;
				changed = true;
				failedStop = true;
				//core.host:trace(string.format("Trailing RVal %f  c %f  ns %f  ls %f", rdiff, ASKsource.close[aidx], newStop, lastStop));
			}
		}
	}



		//--move stop to breakeven if price moves above 1r
	if (forceUpdate==false && orderPending == false && useBrkEven == true && brkEvenSet == false)
	{
		//--core.host:trace(string.format("Failed stop retry %f  %f  %f", rStop, BIDsource.close[bidx], lastStop));

		if (acct.BuySell > 0 && (*(SRC))[idx].close > Entry + stopPips && actualStop < Entry)	//-- > 1R
		{	//--local rdiff = math.floor((BIDsource.close[bidx] - 2.0 - Entry) / stopPips);
			newStop = Entry + (stopPips * berVal);
			tryStop = newStop;
			newProf = lastProfit;
			//--rStop = Entry + (stopPips * rdiff);
			side = acct.BuySell;
			lastStop = 0;
			forceUpdate = true;
			changed = true;
			//--failedStop = true;
			brkEvenSet = true;
			//core.host:trace(string.format("Breakeven  c %f ns %f ls %f", BIDsource.close[bidx], newStop, lastStop));
		}
	

		if (acct.BuySell < 0 && (*(SRC))[idx].aclose < Entry - stopPips && actualStop>Entry)
		{	//--local rdiff = math.floor((Entry - ASKsource.close[aidx] + 2.0) / stopPips);
			newStop = Entry - (stopPips * berVal);
			tryStop = newStop;
			newProf = lastProfit;
			//--rStop = Entry - (stopPips * (rdiff));
			side = acct.BuySell;
			lastStop = 0;
			forceUpdate = true;
			changed = true;
			//--failedStop = true;
			brkEvenSet = true;
			//core.host:trace(string.format("Breakeven c %f ns %f ls  %f", ASKsource.close[aidx], newStop, lastStop));
		}

	}
//------------------------------------------------------------------------------

	newStop = _round(newStop, 5);
	newProf = _round(newProf, 5);

	//--Update trade if stops changed or if they don't match the recorded values!
	//--if the stop has somehow found it's way to 0 make sure we reinstate one!		

	if (acct.lots>0 && newStop > 0.0f)
	{
		if ((acct.BuySell == -1 && (acct.stop > newStop || acct.limit > newProf) && newStop > td.ask) ||
			(acct.BuySell == 1 && (acct.stop < newStop || acct.limit < newProf) && newStop < td.bid))
		{
			changed = true;
			type = "remedial 1";
			side = acct.BuySell;
			newProf = (*(SRC))[idx - 1].close + 0.01f;
			forceUpdate = true;
		}
	}
		//-- or if there is no stop or limit at all.
	if (acct.lots>0 && (acct.stop == 0 || acct.limit == 0))
	{
		//--if no stop at all use a default of the stopdistance - put something in place!
		if (acct.BuySell < 0 && lastStop == 0) lastStop = acct.openPrice + stopDistance;
		if (acct.BuySell > 0 && lastStop == 0) lastStop = acct.openPrice - stopDistance;

		changed = true;
		type = "remedial 2";
		side = acct.BuySell;
		//--only change if no value in the new target variables
		if (newStop == 0) newStop = lastStop;
		if (newProf == 0) newProf = (*(SRC))[idx - 1].close + 0.01f;
		forceUpdate = true;
	}

	if ((changed == true && newStop != lastStop) || (forceUpdate == true && newStop != lastStop))
	{
		if (acct.lots>0)
		{
			acct.stop = newStop;
			acct.limit = newProf;
		}

		stopTarget = newStop;
		profitTarget = newProf;
	}
		
	changed = false;
	lastStop = stopTarget;
	lastProfit = profitTarget;
}

//------------------------------------------------------------------
void Strategy::checkForClose(bool exitNow)
{
	bool bail = false;
	string closedBecause = "";

	if (acct.BuySell < 0) //then a sell
	{
		if (useSessionControl == true && ALLOWTRADE == false && Weekend == true)
		{
			bail = true;
			closedBecause = "TIME Close";
		}

		if (overrunExit == true)
		{
			bail = true;
			overrunExit = false;
			closedBecause = "Overrun Close";
		}

		//--Sell so stop above price - so needs to be less when trailing
		if (failedStop == true && actualStop > tryStop)
		{	//--If current close is beyond the "newStop" then exit as it is failing to modify the stop!
			if (TICKSRC[0].ask <= tryStop)
			{
				//--				bail = true;
				failedStop = false;
				closedBecause = "Couldn't place a fixed trailing stop";
			}
		}

/*		if (TICKSRC[0].ask <= profitTarget || TICKSRC[0].ask >= stopTarget)
		{
			acct.BuySell = 0;
			orderPending = false;
			orderSent = false;
			closedFlag = true;
		}
*/
		
		if (TICKSRC[0].ask <= rStop && rReached == false) // have we hit the base rVal
		{	//core.host:trace(string.format("rReached  %f, %f", rStop, r1Stop));
			rReached = true;
			brkEven = true;
		}

		if (bail == true)
		{
			close(0, TICKSRC[0].ask);
			acct.BuySell = 0;
			orderPending = false;
			orderSent = false;
//			profited=false;
//			inProfit = false;
			state = 0;
		}
	}

	if (acct.BuySell > 0) //then a buy
	{
		if (useSessionControl == true && ALLOWTRADE == false && Weekend == true)
		{
			bail = true;
			closedBecause = "TIME Close";
		}

		if (overrunExit == true)
		{
			bail = true;
			overrunExit = false;
			closedBecause = "Overrun Close";
		}


		if (failedStop == true && actualStop < tryStop)
		{
			//--If current close is beyond the "newStop" then exit as it is failing to modify the stop!
			if (TICKSRC[0].bid >= tryStop)
			{	
				//--				bail = true;
				failedStop = false;
				closedBecause = "Couldn't place a fixed trailing stop";
			}
		}


/*		if (TICKSRC[0].bid >= profitTarget || TICKSRC[0].bid <= stopTarget)
		{
			acct.BuySell = 0;
			orderPending = false;
			orderSent = false;
			closedFlag = true;
		}
*/

		if (TICKSRC[0].bid >= rStop && rReached == false) // have we hit the base rVal
		{	//core.host:trace(string.format("rReached  %f, %f", rStop, r1Stop));
			rReached = true;
			brkEven = true;
		}

		if (bail == true)
		{
			close(0, TICKSRC[0].bid);
			acct.BuySell = 0;
			orderPending = false;
			orderSent = false;
//			profited = false;
//			inProfit = false;
			state = 0;
		}
	}

	//Now the closes due to hitting the stop or limit. Adjust based on the stop/limit and spread rather than bid/ask which may be
	//less accurate than reality
	if ((acct.BuySell == 1 && TICKSRC[0].bid >= acct.limit) || (acct.BuySell == -1 && TICKSRC[0].ask <= acct.limit))
	{
//		float spread = TICKSRC[0].ask - TICKSRC[0].bid;
		float _price = acct.limit;	// -spread;
//		if (acct.BuySell == -1) _price = acct.limit + spread;
		close(0, _price);
		acct.BuySell = 0;
		closedFlag = true;
		orderPending = false;
		orderSent = false;
//		profited = false;
//		inProfit = false;
		state = 0;
	}
	else if ((acct.BuySell == 1 && TICKSRC[0].bid <= acct.stop) || (acct.BuySell == -1 && TICKSRC[0].ask >= acct.stop))
	{
//		float spread = TICKSRC[0].ask - TICKSRC[0].bid;
		float _price = acct.stop;	// +spread;
//		if(acct.BuySell==-1) _price = acct.stop - spread;
		close(0, _price);
		acct.BuySell = 0;
		closedFlag = true;
		orderPending = false;
		orderSent = false;
//		profited = false;
//		inProfit = false;
		state = 0;
	}
}

//------------------------------------------------------------------
void Strategy::setTargets(int direction)
{
	int period = idx;
	float price = 0.0f;
	float extreme = 0.0f;

	float spread = TICKSRC[0].ask - TICKSRC[0].bid;
	TickData td=TICKSRC[0];

	//targetProfit = acct.Balance * tgtPct / 100;


	//--Note STOP and LIMIT are for setting the trade entry up
	if (direction == -1) //sell        
	{
		price = Entry;
		extreme = stptgt;

		STOP = (extreme - price) * 10000;
		stopTarget = price + (STOP / 10000);

		LIMIT = (price - setUpTargetS) * 10000;
		profitTarget = price - (LIMIT / 10000);
	}

	if (direction == 1) //buy
	{
		price = Entry;
		extreme = stptgt;

		STOP = (price - extreme) * 10000;
		stopTarget = price - (STOP / 10000);

		LIMIT = (setUpTargetL - price) * 10000;
		profitTarget = price + (LIMIT / 10000);
	}
	wlRatio = LIMIT / STOP;

//	/* MAYBE TAKE FLOOR VALUES IN PREFERENCE??
	float _acctBal=acct.Balance * PctBalance / 100;
	float maxLoss=_acctBal * maxLossPerTradePct / 100;
	int maxCalcLots=(int)_round((_acctBal/MMR),0);
	float divisor=abs(stopTarget-price)*ContractSize*lotAdj;
	float lossCost=divisor*PIPCOST;
	int64_t calcLots=(int)_round(maxLoss/lossCost,0);
	
	if (calcLots>maxCalcLots) calcLots=maxCalcLots;
	if (calcLots<minLots) calcLots=minLots;
	if (calcLots>MaxPossibleLots/ContractSize) calcLots=MaxPossibleLots/ContractSize;
	if (maxLots > 1 && calcLots > maxLots) calcLots=maxLots;
	if (maxLots == -2) calcLots = (int64_t)floor(_acctBal*lotPct);
	lots=(int)_round((float)calcLots,0)*ContractSize;


/*
	float _acctBal2 = acct.Balance * PctBalance / 100;
	float maxLoss2 = _acctBal2 * maxLossPerTradePct / 100;
	int maxCalcLots2 = (int)_round((_acctBal2 / MMR) / 100, 0) * 100;
	float divisor2 = (abs(stopTarget - price) / Point / 10);
	float lossCost2 = divisor2*PIPCOST;
	int64_t calcLots2 = (int)floor((maxLoss2 / lossCost2)/100)*100;

	if (calcLots2>maxCalcLots2) calcLots2 = maxCalcLots2;
	if (calcLots2 > MaxPossibleLots) calcLots2 = MaxPossibleLots;

	if (maxLots > 1 && calcLots2 > maxLots) calcLots2 = maxLots;
	if (maxLots == -2) calcLots2 = (int)floor(_acctBal2 * lotPct);

	lots = (int)(_round((float)calcLots2, 0)) * ContractSize;
*/
}

//------------
//adjustements if using mt4 involve use of scale to adjust pipcost and maxposslots
/*float _pipcost = PIPCOST / scale1;			//scale1
int64_t mpl = MaxPossibleLots * scale2;			//scale2

float _acctBal = acct->Balance * PctBalance / 100.0f;
float maxLoss = _acctBal * maxLossPerTradePct / 100.0f;
int maxCalcLots = (int)_round(_acctBal / MMR, 0);
float divisor = abs(stopTarget - price) / pointSize / 10;		//pointSize
float lossCost = divisor * _pipcost;
int64_t calcLots = (int)floor(maxLoss / lossCost);

if (calcLots > maxCalcLots) calcLots = maxCalcLots;
if (calcLots > MaxPossibleLots) calcLots = MaxPossibleLots;
if (maxLots != -1 && calcLots > maxLots) calcLots = maxLots;

lots = (int)(_round((float)calcLots, 0)) * 1000;
*/
//------------


//------------------------------------------------------------------
void Strategy::enter(int side)
{
	acct.lots = lots;
	acct.BuySell = side;
	//based on direction whether bid or ask value
	//SWAPPED 27/8/22 by changing the acct.BuySell to == -1 from == 1
	if (acct.BuySell == -1) acct.openPrice = TICKSRC[0].bid;
	else acct.openPrice = TICKSRC[0].ask;

	acct.stop = stopTarget;
	acct.limit = profitTarget;

	orderPending = false;
	tradeActive = true;
	
	if(test==true)
		ofile << acct.BuySell << " " << acct.trigger << " " << acct.openPrice << " " << acct.lots << " " << acct.stop << " " << acct.limit << " " << TICKSRC[0].dtime << " ";

}

//------------------------------------------------------------------
void Strategy::close(int partial, float price) //	--closes the position
{
	//valuemap vmap;

	if (acct.lots > 0)
	{
		//Calculate any close profit/loss and apply to the balance. Update the lot size if partial 
		if (partial > 0) //it's a partial close
		{
			abort = calculatePL(partial, price);
			acct.lots = acct.lots - partial;
			if (acct.lots == 0) acct.BuySell = 0;
			if (test == true)
			{
				ofile << "P" << " " << price << " " << acct.lots << " " << acct.stop << " " << acct.limit << " " << acct.Balance << " " << TICKSRC[0].dtime << endl;
				ofile << acct.BuySell << " " << acct.openPrice << " " << acct.lots << " " << acct.stop << " " << acct.limit << " " << TICKSRC[0].dtime << " ";
			}
		}
		else
		{
			abort = calculatePL(acct.lots, price);

			if (test == true)
			{
				ofile << "C" << " " << price << " " << acct.lots << " " << acct.stop << " " << acct.limit << " " << acct.Balance << " " << TICKSRC[0].dtime << endl;
			}
			acct.lots = 0;
			acct.openPrice = 0.0f;
			acct.limit = 0.0f;
			acct.stop = 0.0f;
			acct.BuySell = 0;

		}
	}
}

bool Strategy::calculatePL(int lots, float price)
{
	//Take the pip difference (accounting for direction) and multiply by lots.
		//but we want a consistent benchmark

	float pips = acct.openPrice - price;
	if (acct.BuySell == 1) pips = price - acct.openPrice;

	float PL = (pips*acct.lots-(acct.commission*acct.lots/1000))*PIPCOST;
	acct._PL = (double)PL;

	//Update balance - we don't bother with equity as we're not tracking while trade is active
	acct.Balance = acct.Balance + PL;

	if (acct.Balance > acct.maxBalance)
	{
		acct.maxBalance = acct.Balance;
		acct.maxMinPct = (acct.maxBalance - acct.minBalance) / acct.maxBalance;
	}
	if (acct.Balance < acct.minBalance)
	{
		acct.minBalance = acct.Balance;
		acct.maxMinPct = (acct.maxBalance - acct.minBalance) / acct.maxBalance;
	}
	
	if (acct.Balance < 20000)
		return true;
	else
		return false;

}


