//Minuteman
#pragma once

#include "stdafx.h"

#include <math.h>
#define INDICORE2_EXTKIT
#include "indicore2.h"
#include <vector>
#include <ctime>
#include "ATLComTime.h"
#include <fstream>
#include "shlobj.h"
#include <random>

#include <SourceData.h>



std::ofstream ofile;
int setup(lua_State *L);
int setTimes(lua_State* L);
int storeData(lua_State *L);
int getTriggerStart(lua_State* L);
int setTriggerStart(lua_State* L);

void processData();
int processVolData(double _date);
int getStartDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMin);
//void DatePlusDays(struct tm* date, int days);
void createMinute(int minNum, float minOpen, float minHigh, float minLow, float minClose, float _last);
void reCreateMinute(int _minNum, unsigned char _minute[31], float _barOpen, float* mo, float* mh, float* ml, float* mc);
void createMissingMinutes(int _from, int _to, int origTo, float minOpen, float minHigh, float minLow, float minClose, int minute);
int minuteCreator(int From, int _to, int To, int oldOne, float mo, float mh, float ml, float mc, float barAdjustC, float barAdjustH, float barAdjustL);
void getMinVal(int _from, float* mo, float* mh, float* ml, float* mc);
void saveBar();
void cleanVars();
float _round(float num, int idp);

COleDateTime today;
COleDateTime last;

int population = 90;	//period of days to backtest over
int hPopulation = 0;
int frequency = 90;		//how often changes are applied
int month = 1;		//starting month
int day = 1;		//starting day

bool gap = false;

float minRegdif=0.0f, maxRegdif=0.0f;
float minRegdif2=0.0f, maxRegdif2=0.0f;
int minReglinelen=0, maxReglinelen=0;
int minReglinelimit = 0, maxReglinelimit = 0;
int minReglinema = 0, maxReglinema = 0;
float minRVal = 0.0f, maxRVal = 0.0f;
int minReglinemalen = 0, maxReglinemalen = 0;
int aggVal = 0;

int minStop=0, stopMax=0;

float pipCost = 0.0f;
float mmr = 0;
int64_t maxLots = 0;
float maxloss = 1.0f;
int pctBalance = 80;
int contractSize = 10000;

unsigned setNumber = 0;
float pointSize = 0.01f;
int lotAdj = 1;
std::string instrument="";
int tzofset = 0;

//int MAPeriod = 12;
//int MAType = 3;

//float trigRate = 0.5f;
//float fibEntry = 0.6f;
//float fibTgt = 0.5f; 
//int fixTgt = 20;

float regDif = 1.5f;
float regDif2 = 2.0f;
int regLinelen = 4;
int regLinelimit = 20;
int regLinema = 1;
int regLinemalen = 20;
float rVal = 1.0f;
int ma = 20;
int flim = 20;
int agg=0;


int oneOfBar = 0;	//Count of the 5 1 minute bars so we can create the short term string that holds the represents the up/down move
int priorOne = -1;		//
float minOpen = 0.0f;
float priorOpen = 0.0f;
float minHigh = 0.0f;
float minLow = 0.0f;
float minClose = 0.0f;
float priorClose = 0.0f;
int minVolume = 0;
double minDate = 0.0;
float highVal = 0.0f;
float lowVal = 0.0f;

float oSpread = 0.0f;
float hSpread = 0.0f;
float lSpread = 0.0f;
float cSpread = 0.0f;

unsigned char minDetails[31];	//76
double barStartDate = 0.0;
double nextDate = 0.0f;
float barOpen = 0.0f;
float lastOpen = 0.0f;
int missingBars = 0;
bool savePartial = false;
//DCM 5 min dependency! valid as setting 1 minute...but could just precalc minute value;
float minSize = 0.00069444f / 5;

int ftnVolume = 0;
double ftnDate = 0.0;

float truStartDate = 0.0f;
int maxMAPeriod = 20;

struct session
{
	bool useSessionControl=false;
	int sessionOpenHour=0;
	int sessionOpenMin=0;
	int sessionCloseHour=0;
	int sessionCloseMin=0;
	int timeOffset=0;
	std::vector<int> excStartTime;
	std::vector<int> excEndTime;
	int overRunTime=0;
} sess;

struct SourceFile
{
	std::vector<SourceData> ohlcvData;
	std::vector<MinuteData> minData;
} sf;

std::vector <VolSourceData> volData;

std::vector <SourceFile> dataArray;




