//Minuteman
#pragma once

#include "stdafx.h"
#include "Strategy.h"
#include <vector>
#include <ctime>
#include "ATLComTime.h"
#include <chrono>
#include <fstream>
#include "shlobj.h"
#include "History.h"


vector <SourceData> ohlcvData;
vector <SourceData> zigData;
vector <TickData> tickData;
vector <Account*> results;
vector <MinuteData> minData;

vector <History> hist;
vector <Histdata> histd;

vector <Account*> top5;
Account finalParams[5];


/*void analyse(int _atr, float _atrl, int _map, int _mat,
			float _stopVal, float _backstop, float _rVal,
			float _dailyTgt, bool _useDailyTgt, bool _invert,
			Account* acct, int _procNum, int _tzo, float _truStart,
			bool _useSess,	int _sessOpen, int _sessClose, int _sessOff, 
			vector<int>* _excStart, vector<int>* _excEnd,
			int _sessOverrun);*/

//void analyse(int _rl, int _fl, float _rd, float _rd2, float _rval, float _be, int _ma, 
//			Account* acct, int _procNum);

void analyse(float _rval, int _ma, int _flim, int _agg, Account* acct, int _procNum);

bool testAnalysis();
bool getOHLCV();
//bool processZigData(vector <SourceData>* _SRC);
void shiftTop5(int t);
void getTop5();
void sortResults(void* dat, int which);

int partition(int _start, int _end, int which, void* dat);
void quickSort(int _start, int _end, int which, void* dat);

void backUpRestoreSummation(int br);
void rollBackUps();
void loadBlankHistory();
//void reGenBias();
void sortHist(void*);
bool dirExists(const std::string& dirName_in);
void saveCurrentFiles();

int start = 0;
int end = 0;


float minRegdif = 0.0f, maxRegdif = 0.0f;
float minRegdif2 = 0.0f, maxRegdif2 = 0.0f;
int minReglinelen = 0, maxReglinelen = 0;
int minReglinelimit = 0, maxReglinelimit = 0;
int minReglinema = 0, maxReglinema = 0;
float minRVal = 0.0f, maxRVal = 0.0f;
int minReglinemalen = 0;
int maxReglinemalen = 0;
int aggVal = 0;

/*int minATR, maxATR;
float minATRLimit, maxATRLimit;
int minMAPeriod, maxMAPeriod, minMAType, maxMAType;
float minStop, maxStop,  minBackStop, maxBackStop;
float minRval, maxRval, minRval2, maxRval2, minDailyTgt, maxDailyTgt;
bool useDailyT, useDailyF, invert, invVal;
*/


//int MAPeriod = 12;
//int MAType = 3;

/*float trigRate = 0.5f;
float fibEntry = 0.6f;
float fibTgt = 0.5f;
int fixTgt = 20;
*/

float regDif = 1.5f;
float regDif2 = 2.0f;
int regLinelen = 4;
int regLinelimit = 20;
int regLinema = 1;
float rVal = 1.0f;
float breakEven = 0.15f;

int minStop = 0, stopMax = 0;

float pipCost = 0.0f;

float rval = 1.1f;
int ma = 20;
int flim = 20;
int agg = 0;


float mmr = 0;
int64_t maxLots = 0;
int maxloss = 0;
int pctBalance = 0;
int contractSize = 0;

float pointSize = 0.01f;
int lotAdj = 1;
//int scale1 = 1;
//int scale2 = 1;

int tzofset = 0;
float truStart = 0.0f;

bool useSessionControl = true;
int sessionCloseHour = 17;
int sessionCloseMin = 0;
int sessionOpenHour = 7;
int sessionOpenMin = 0;
int sessOpen = 0;
int sessClose = 0;
int sessOff = 0;
vector<int> excStart;
vector<int> excEnd;
int overRunTime = 0;



string instrument = "";

//create threads in an array;
thread thrd[11];

