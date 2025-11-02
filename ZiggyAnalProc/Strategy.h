//Minuteman
#pragma once

#include "Indicators.h"
#include "Account.h"
#include <math.h>
#include "ATLComTime.h"

#include <thread>
#include <future>
#include "bar.h"
#include <fstream>
#include "shlobj.h"
#include <random>

using namespace std;
/*
struct Source {
	std::vector<double> high;
	std::vector<double> low;
	std::vector<double> close;
	std::vector<double> open;
};

Source ZASKsource;
Source ZBIDsource;
*/


class TickInfo
{
public:
	TickInfo() {};
	~TickInfo() {};

	float HO=0.0f;
	float OL=0.0f;

	bool dirUpClosest=true;
	float UDPct = 0;
	int dir = 0;
	int tickTargetPct1=0;
	int tickTargetPct2=0;
	float tickTarget1 = 0.0f;
	float tickTarget2 = 0.0f;
	int nextTarget = 0;
	float maxTickSize=0.0f;
	float bodySize = 0.0f;
	float midHigh = 0.0f;
	float midLow = 0.0f;
	float upPct = 0.0f;
	float downPct = 0.0f;
	int numTicks = 0;
	int tickOffset = 0;
	float dtOffset = 0.0f;
	float minHigh = 0.0f;
	float minLow = 0.0f;
	float minClose = 0.0f;
};



class Strategy
{

public:
	//update variables to be ma length, rval, range filter and aggressive Y/N
	Strategy(Account* _oAcct, vector <SourceData>* _SRC, vector <SourceData>* _ZIG, vector <MinuteData>* _MINSRC,
			int _idx, int _zidx, int _idx2, 
//			int _rl, int _fl, float _rd, float _rd2, float _rval, float _be, int _ma,
			float _rval, int _ma, int _flim, int _agg,
			float _pipCost, float _mmr, int64_t _maxlots, int _maxloss, int _pctBalance,
			int _cSize, float _pointSize, int _lotAdj, int _procNum, int _tzo, float _truStart,
			bool _useSess, int _sessOpen, int _sessClose, int _sessOff, 
			vector<int>* _excStart, vector<int>* _excEnd,
			int _sessOverrun);
	~Strategy();

	void runBT(bool _test);


private:
	std::ofstream ofile;
	std::ofstream ofilem;

	struct _entryVals {
		bool use3Bar = true;
		bool useAgg = true;
	} entryVals;

/*		float trigRate = 0.0;
		float fibEntry = 0.0f;
		float fibStop = 0.0f;
		float fibTgt = 0.0f;*/
/*
	struct ZigZagVariables {
		std::vector<int> zBper;
		std::vector<int> zAper;
		std::vector<bool> _isUpA;
		std::vector<bool> _isDownA;
		std::vector<int> _dirA;
		std::vector<float> _zigzagA;
		std::vector<bool> _isUpB;
		std::vector<bool> _isDownB;
		std::vector<int> _dirB;
		std::vector<float> _zigzagB;
		int zBid=0;
		int zAid=0;
		int zigLimit=10;

		//		xB = 0.0,
		//		aB = 0.0,
		//		bB = 0.0,
		float cB = 0.0f;
		float dB = 0.0f;

		//		xA = 0.0,
		//		aA = 0.0,
		//		bA = 0.0,
		float cA = 0.0f;
		float dA = 0.0f;
		*//*
				xabB = 0.0,
				xadB = 0.0,
				abcB = 0.0,
				bcdB = 0.0,

				xabA = 0.0,
				xadA = 0.0,
				abcA = 0.0,
				bcdA = 0.0,
		*/

		/*
				zigLimit = 1,
				zBid = 1,
				zAid = 1,
				zBper = {},
				zAper = {},*/
/*		int x_dB = 0;
		int x_dA = 0;

		vector<float>_xabcdB;
		vector<int>_hlhlB;


		vector<float>_xabcdA;
		vector<int>_hlhlA;
	};

	ZigZagVariables zzV;
*/
	struct _regVals {	//check how many of these are used!!!
/*		int	hIdx = 1;
		int lIdx = 1;
		//--hIdx1 = 10;
		//--lIdx1 = 10;
		bool zMerge = true;
		bool zMerged = true;
		bool hRange = true;
		bool showSup = true;
		bool showRes = true;
		int maxHl = 11;

		int leftCount = 3;
		int rightCount = 3;

		int zoneInvert = 0;
		int zoneLimit = 5;
		int zoneNum = -1;
		int zoneRemoval = 10;

		float midZone = 0.0f;
		float zoneH = 0.0f;
		float zoneL = 0.0f;
		int useHedging = true;
		float hedgeVal = 0.0f;
		bool hedgeSet = false;
		float invalid = 0.0f;
*/
		bool useFixedTgt = false;
		int fixedTgt = 20;
/*
		std::vector<float> highNum;
		std::vector<float> lowNum;
		std::vector<float> highVal;
		std::vector<float> lowVal;
		std::vector<float> highPos;
		std::vector<float> lowPos;
		std::vector<float> highRange;
		std::vector<float> lowRange;
		std::vector<float> highUpCount;	//--count crosses into zone from below
		std::vector<float> lowUpCount{};
		std::vector<float> highDnCount;	//--count crosses into zone from above
		std::vector<float> lowDnCount;
		std::vector<float> highDel;
		std::vector<float> lowDel;
*/
	};

	_regVals regVals;

	struct _regress {
		//void regu = NULL;
		//void regd = NULL;
		vector<float> reguVal;
		vector<float> regdVal;

		bool useRegLine = true;
		int regLineType = 1;
		int regLineLen = 4;
		int regLineLimit = 20;
		int regLineMA = 1;
		int regLinemalen = 20;
		float regDiff = 1.5f;
		float regDiff2 = 2.0f;
		int agg_val = 0;

		bool useReg = true;
		int regDepth = 10;
		int regSrc = 1;
		int regLimit = 35;
		int regOffset = 0;
	};

	_regress regress;


	struct _indVals
	{
		//--maLimit = 0.0,
		bool useATRStop = false;

		int mauType = 3;
		float mauVal = 0.0f;
		int mauPeriod = 12;
		//void MAU = 0;

		int madType = 3;
		float madVal = 0.0f;
		int madPeriod = 12;
		//MAD = nil;
	};

	_indVals indVals;



//declarations
	bool test = false;

	void checkTradingAllowed(COleDateTime dt);
	float _round(float num, int idp);
	//int checkEntry(int ix,  float _rate);
	void checkForSetup(int period);
	void checkForOpen();

	void checkForTarget();
	void checkForClose(bool exitNow);
	void setTargets(int direction);
	void enter(int side);
	void close(int partial , float price);

//	bool validTriggerBar(bool bs, bool ss, int ax, int bx);
//	int validAO(int aix, int bix);

//	float getLastSpike(int dir);
//	bool bigWickRange(int _dir, int _idx, float _mul);
//	int hasCrossed(int bix);
//	int validArrakis(int bx, int Ord);

//	float getStop(int dir, int bx);
	float highest(float per, int prd);
	float lowest(float per, int prd);
	float _floor(float val, int idp, int sig);
	float _ceil(float val, int idp, int sig);

	int getRegUpDn(int ba, int dataset, int depth, int _prd, int offset);
	void getRegression(int depth, int ba, int dataset, int _period, float& b, float& m);
	float sourceVal(int ds, int id);

//	void upDateZigZag(int lz);
//	bool getNextZZ(std::vector<float>& src, int offset, int& id, std::vector<int>& per, int scan, float& tgt);
//	int gethlv(int BA);
//	void shiftXABCD(int BA, float zval);
//	bool getXABCD();

	void setTI(int _period, int _min, int _numTicks, /*vector <MinuteData>* _minsrc, vector <SourceData>* _src,*/ TickInfo* _ti, double bo);
	bool calculatePL(int lots, float price);
	double getPL();


	void createNewTick(int period, int min, int tick, TickInfo* ti);

	int getTicksByVol(int period);

	void update(int period, int min, int zPeriod);
	void reCreateMinute(int _minNum, int _period, /*unsigned char _minute[91],*/ double _barOpen, double* mo, double* mh, double* ml, double* mc);
	int	checkLinRegEntry(int idx);
	float lasthigh(int prd);
	float lastlow(int prd);

	//vars
	vector <SourceData> *SRC;
	vector <SourceData> *ZIG;
	vector <MinuteData> *MINSRC;

	int idx = 0;
	int idx2 = 0;
	int zidx = 0;

	Indicators* ind;
	Account acct;
	Account* result;

	bool preLoad = false;

	//vars to store current bar creation
	vector<bar> currentBar;
	vector<TickData> TICKSRC;

	float _open = 0.0f;
	float _high = 0.0f;
	float _low = 0.0f;
	float _close = 0.0f;


	int atrPeriod=10;
	float atrLimit = 0.0010f;
	vector<float> atrLine;
//	vector<float> atrTick;	//holds latest tick based values
	vector<float> MALine;
	vector<float> sgLine;
	vector<float> mdLine;

/*	int MAPeriod = 12;
	int MAType = 3;
	float trigRate = 0.5f;
	float fibEntry = 0.6f;
	float fibTgt = 0.5f;
	int fixTgt = 20;*/

//	float regDif = 1.5f;
//	float regDif2 = 2.0f;
//	float regLinelen = 4;
//	float regLinelimit = 20;
	float breakEven = 0.15f;

	bool dtHit = false;

	int stop_Val=0;
	int limit_Val=0;
	float minStop=10.0f;

	int lotAdj = 0;

	float stopTarget = 0.0f;
	float profitTarget = 0.0f;
	float lastStop = 0.0f;
	float lastProfit = 0.0f;
	float stopDistance = 0.003f;
	float openPrice = 0.0f;

	float actualStop = 0.0f;
	float actualLimit = 0.0f;
	float nPipsStop = 3.0f;

//	int buySell = 0;
	int lots = 0;
	int usedLots = 0;
	int pctProfit = 0;

	int ticket=0;

	bool orderPending = false;
	bool orderSent = false;

	bool abort = false;

	float Balance = 10000.0f;
	int PctBalance = 0;
	int minLots = 0;
	int maxLots = -1;
	int maxLossPerTradePct = 0;
	bool allowMultiTry = true;
	int tryLimit = 15;
	int attempts = 0;
	int proximityPips = 0;
	bool usePipEntryLimit = true;
	bool allowPivotStops = true;
	int stopBar = 2;
	bool closedFlag = false;

	int cTime = 0;

	bool useSessionControl = true;
	int tzofset = 0;
	float truStart = 0.0f;
	

	int sessionCloseHour = 17;
	int sessionCloseMin = 0;
	int sessionOpenHour = 7;
	int sessionOpenMin = 30;
	int sessOpen = 0;
	int sessClose = 0;
	int sessOff=0;

	int tradingDay = 0;
	bool Weekend = false;
	bool warningSet = false;
	COleDateTime dayStart = 0.0f;

	int sessionTimeOffset = 0;
	vector<int>* excStart;
	vector<int>* excEnd;
	int overRunTime = 0;
	int weekendClose = 0;

	bool brkEvenOverrun = true;
	bool inProfit = false;
	bool failedStop = false;


	bool allowPartial = true;
	float partialPips = 0.0022f;
	bool partTaken = false;
	float priorTrailPips = 0.0016f;
	float priorTrailPipsStore = priorTrailPips;

	int pbCount = 0;
	int trgrCount = 0;
	int eodCount = 0;
	bool overrunExit = false;

	bool init = false;
	bool ALLOWTRADE=true;
	int ACCOUNT=0;
	bool CANTRADE=true;

	bool CANCLOSE=true;
	int OFFER=0;
	float LIMIT=0.0f;
	float STOP=0.0f;
	int Accounts=0;
	string MAGICDCMHA = "";
	int ContractSize=1000;

	bool stopAdjusted=false;


	int64_t MaxPossibleLots = 0;
	float MMR = 0;
	float PIPCOST = 0.0f;

	int mmr = 0;

	float Bid=0.0f;
	float Ask=0.0f;

	float Point = 0.00001f;

	bool priorBarTrail = true;

	int touchBar = 0;
	int lastBar = 0;
	bool newBar = false;
	bool attempted = false;
	//float dailyBalance = 0.0f;

	int lastEntryBar = 0;
	bool tradePlaced = false;
	float trigger = 0.0f;
	const float sec1 = 0.000011574f;	//1 second as pctage of day being 86400 = 24* 60 * 60

	bool possBuy = false;
	bool possSell = false;
	int entryDir = 0;
	bool tradeActive = false;
	int state = 0;
	bool buySU = false;
	bool sellSU = false;
	double dailyBalance = 0.0;
	double startBalance = 0.0;

	int RD = 0;
	float highVal = 0.0;
	float highVal1 = 0.0;
	float lowVal = 0.0;
	float lowVal1 = 0.0;
	int highPos = 0;
	int highPos1 = 0;
	int lowPos = 0;
	int lowPos1 = 0;

	float setUpPriceS = 0.0f;
	float Entry = 0.0f;
	float setUpStopS = 0.0f;
	float stopPips = 0.0010f;
	float setUpTargetS = 0.0f;
	float limitPips = 0.00150f;

	float setUpPriceL = 0.0f;
	float setUpStopL = 0.0f;
	float setUpTargetL = 0.0f;

	double entryTime = 0.0;
	float stptgt = 0.0f;

	float targetProfit = 0.0f;
	int tgtPct = 0;
	float lotPct = 0.0025f;

	float pointSize = 0.01f;
	int scale1 = 1;
	int scale2 = 1;

	int procNum = 0;

	float wlRatio = 0.0f;
	bool brkEven = false;
	bool brkEvenSet = false;
	bool useBrkEven = false;

	int pres = 2;
	bool _hasCrossed = false;
	int crossed = 0;
	int crossedCount = 0;
	int maOrder = 0;

	bool rReached = false;
	float tryStop = 0.0f;
	float rStop = 0.0f;
	float r1Stop = 0.0f;
	float berVal = 1.05f;
	float stopMax = 80.0f;
	float rVal = 1.0f;

	float lastBid = 0.0f;
	float lastAsk = 0.0f;
	float lastZBid = 0.0f;
	float lastZAsk = 0.0f;

	int lastZBar = 0;




//	bool inProfit = false;
//	bool profited = false;

	mt19937 gen;

};


