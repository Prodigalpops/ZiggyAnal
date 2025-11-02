//Minuteman
// HAMAOanal.cpp : Defines the exported functions for the DLL application.
//
#define testing
#define minuteman

#include "ZiggyAnal.h"
#include <cmath>
using namespace std;

mt19937 gen;



int setup(lua_State *L) {
	gen = mt19937(1);  // to seed mersenne twister.
	//ensure we start with empty storage as could have been restarted from trading platform
	//	MessageBox(NULL, "Setup!", "Setup!", 0);

	int currentYear;
	int currentMonth;
	int currentDay;
	int currentHour;
	int currentMin;

	cleanVars();

	int n = lua_gettop(L);    // number of arguments
	if (n != 37	)
		return luaL_error(L, "Insufficient parameters for setup.");
	int idx = 1;
	population = (int)lua_tonumber(L, idx++);
	hPopulation = (int)lua_tonumber(L, idx++);
	frequency = (int)lua_tonumber(L, idx++);
	month = (int)lua_tonumber(L, idx++);
	day = (int)lua_tonumber(L, idx++);
	currentYear = (int)lua_tonumber(L, idx++);
	currentMonth = (int)lua_tonumber(L, idx++);
	currentDay = (int)lua_tonumber(L, idx++);
	currentHour = (int)lua_tonumber(L, idx++);
	currentMin = (int)lua_tonumber(L, idx++);

	minRegdif = (float)lua_tonumber(L, idx++);
	maxRegdif = (float)lua_tonumber(L, idx++);
	minRegdif2 = (float)lua_tonumber(L, idx++);
	maxRegdif2 = (float)lua_tonumber(L, idx++);
	minReglinelen = (int)lua_tonumber(L, idx++);
	maxReglinelen = (int)lua_tonumber(L, idx++);
	minReglinelimit = (int)lua_tonumber(L, idx++);
	maxReglinelimit = (int)lua_tonumber(L, idx++);
	minReglinema = (int)lua_tonumber(L, idx++);
	maxReglinema = (int)lua_tonumber(L, idx++);

	minRVal = (float)lua_tonumber(L, idx++);
	maxRVal = (float)lua_tonumber(L, idx++);

	minReglinemalen = (int)lua_tonumber(L, idx++);
	maxReglinemalen = (int)lua_tonumber(L, idx++);
	aggVal = (int)lua_tonumber(L, idx++);

	minStop = (int)lua_tonumber(L, idx++);
	stopMax = (int)lua_tonumber(L, idx++);

	pipCost = (float)lua_tonumber(L, idx++);
	mmr = (float)lua_tonumber(L, idx++);
	maxLots = (int64_t)lua_tonumber(L, idx++);
	maxloss = (float)lua_tonumber(L, idx++);
	pctBalance = (int)lua_tonumber(L, idx++);

	contractSize = (int)lua_tonumber(L, idx++);
	pointSize = (float)lua_tonumber(L, idx++);
	lotAdj = (int)lua_tonumber(L, idx++);	
	instrument = (string)lua_tostring(L, idx++);
	tzofset = (int)lua_tonumber(L, idx);

	//NEXT STATEMENT FAILS FOR NAS100
	int info = instrument.find("/");
	if(info>0)
		instrument = instrument.replace(instrument.find("/"), 1, "");

	int sdate = getStartDate(currentYear, currentMonth, currentDay, currentHour, currentMin);
	lua_pushnumber(L, sdate);
	barStartDate = 0.0f;
	priorOne = -1;
	lastOpen = 0.0f;

	return 1;                   // number of results 
}

int setTimes(lua_State* L)
{
	int n = lua_gettop(L);    // number of arguments
	if (n != 17)
		return luaL_error(L, "Insufficient parameters for setTimes.");
	int idx = 1;
	sess.useSessionControl = (bool)lua_toboolean(L, idx++);
	sess.sessionOpenHour = (int)lua_tonumber(L, idx++);
	sess.sessionOpenMin = (int)lua_tonumber(L, idx++);
	sess.sessionCloseHour = (int)lua_tonumber(L, idx++);
	sess.sessionCloseMin = (int)lua_tonumber(L, idx++);
	sess.timeOffset = (int)lua_tonumber(L, idx++);
	sess.excStartTime.push_back((int)lua_tonumber(L, idx++));
	sess.excEndTime.push_back((int)lua_tonumber(L, idx++));
	sess.excStartTime.push_back((int)lua_tonumber(L, idx++));
	sess.excEndTime.push_back((int)lua_tonumber(L, idx++));
	sess.excStartTime.push_back((int)lua_tonumber(L, idx++));
	sess.excEndTime.push_back((int)lua_tonumber(L, idx++));
	sess.excStartTime.push_back((int)lua_tonumber(L, idx++));
	sess.excEndTime.push_back((int)lua_tonumber(L, idx++));
	sess.excStartTime.push_back((int)lua_tonumber(L, idx++));
	sess.excEndTime.push_back((int)lua_tonumber(L, idx++));
	sess.overRunTime = (int)lua_tonumber(L, idx++);


	lua_pushnumber(L, 1);
	return 1;                   // number of results 

}

void cleanVars() //ensure we start with a clean sheet
{
	gap = false;
	setNumber = 0;

	minRegdif = 0.0f;
	maxRegdif = 0.0f;
	minRegdif2 = 0.0f;
	maxRegdif2 = 0.0f;
	minReglinelen = 0;
	maxReglinelen = 0;
	minReglinelimit = 0;
	maxReglinelimit = 0;
	minReglinema = 0;
	maxReglinema = 0;
	minRVal = 0.0f;
	maxRVal = 0.0f;
	minReglinemalen = 0;
	maxReglinemalen = 0;

	minStop = 0;
	stopMax = 0;

	oneOfBar = 0;	//Count of the 5 1 minute bars so we can create the short term string that holds the represents the up/down move
	priorOne = -1;		//
	minOpen = 0.0f;
	priorOpen = 0.0f;
	minHigh = 0.0f;
	minLow = 0.0f;
	minClose = 0.0f;
	priorClose = 0.0f;
	minVolume = 0;
	minDate = 0.0;
	highVal = 0.0f;
	lowVal = 0.0f;

	for (int t = 0; t < 31; ++t) { minDetails[t] = '!'; }

	barStartDate = 0.0;
	nextDate = 0.0f;
	barOpen = 0.0f;
	lastOpen = 0.0f;
	missingBars = 0;
	savePartial = false;

	ftnVolume = 0;
	ftnDate = 0.0;
	truStartDate = 0.0f;

	sess.useSessionControl = false;
	sess.sessionOpenHour = 0;
	sess.sessionOpenMin = 0;
	sess.sessionCloseHour = 0;
	sess.sessionCloseMin = 0;
	sess.timeOffset = 0;
	sess.excStartTime.clear();
	sess.excEndTime.clear();
	sess.overRunTime = 0;

}

int storeData(lua_State *L)
{
	//check we have required parameters being the source of ohlcv data
	int n = lua_gettop(L);
	if (n != 8)
		return luaL_error(L, "Insufficient parameters for data storage.");

	setNumber = 0;


	minOpen = (float)lua_tonumber(L, 1);
	minHigh = (float)lua_tonumber(L, 2);
	minLow = (float)lua_tonumber(L, 3);
	minClose = (float)lua_tonumber(L, 4);
	minVolume += (int)lua_tonumber(L, 5);
	minDate = (double)lua_tonumber(L, 6);

	setNumber = (unsigned)lua_tonumber(L, 7);	//Which set do we store this in
	nextDate = (double)lua_tonumber(L, 8);

	gap = false;
	if (sf.ohlcvData.size() > 0)
	{
		double prior = sf.ohlcvData[sf.ohlcvData.size() - 1].date;
		//DCM 5 min dependency!
		gap = ((minDate - prior) / 0.00347f) > 184;	//184 accommodates weekends being approx 46 hours
	}
	processData();

	lua_pushnumber(L, 0);

	return 1;
}


int storeVolData(lua_State* L)
{
	//check we have required parameters being the source of ohlcv data
	int n = lua_gettop(L);
	if (n != 2)
		return luaL_error(L, "Insufficient parameters for vol data storage.");

	ftnVolume = (int)lua_tonumber(L, 1);
	ftnDate = (double)lua_tonumber(L, 2);

	VolSourceData vsd = VolSourceData();
	vsd.volume = ftnVolume;
	vsd.date = ftnDate;
	volData.push_back(vsd);

	lua_pushnumber(L, 0);

	return 1;
}


void processData()
{

	if (dataArray.size() < setNumber)
	{	//Create a new dataset in the source file array
		dataArray.push_back(sf);
		sf = SourceFile();
		priorOne = -1;
	}

	//set oneOfBar to the current minute mod 5
	int minute = ((COleDateTime)minDate).GetMinute();
//DCM 5 min dependency!
	oneOfBar = minute % 5; //0 (5) is the start up to minute 4	
	if (priorOne == -1 || oneOfBar == priorOne + 1)	//Need to save the partial set minDate>42647.77 && minDate<42625if not the next in sequence
	{
//		if (minDate <= 43353 && minDate >= 43350.7)
//		{
//			int x;
//			x = 1;
//		}
		if (oneOfBar == 0)	//new minute start
		{
			float minspread = pointSize * 100.0f;
			float maxspread=pointSize * 1.8f * 100.0f;

			uniform_real_distribution<> rdist(minspread, maxspread);	// (0.00001, 0.00016);
			oSpread = _round((float)rdist(gen), 5);
			
			highVal = minOpen;
			lowVal = minOpen;
			barOpen = minOpen;
			minVolume = 0;
			barStartDate = minDate;	//ensure we set the date

			createMinute(oneOfBar, minOpen, minHigh, minLow, minClose, 0.0f);
		}
		else if (priorOne == -1) //a missing start point so recreate from prior close
		{
			int _from = 0;
			int _to = oneOfBar;
			createMissingMinutes(_from, _to, _to, minOpen, minHigh, minLow, minClose, minute);
		}
		else
		{
			createMinute(oneOfBar, minOpen, minHigh, minLow, minClose, 0.0f);
		}
		priorOne = oneOfBar;
	}
	else
	{
		//If we have a one minute or more gap then fill with an average between the prior and current bar
//DCM 5 min dependency!
		if (abs(oneOfBar - priorOne) > 1 || minDate - barStartDate>0.00347)
		{
			int _from = priorOne;
			int _to = oneOfBar;
			createMissingMinutes(_from, _to, _to, minOpen, minHigh, minLow, minClose, minute);

			priorOne = oneOfBar;
		}
		else
			savePartial = true;
	}

	//create a list of the data
	saveBar();

	priorOpen = minOpen;
	priorClose = minClose;
}

//------------------------------------------------------------------
float _round(float num, int idp)
{
	int mult = (int)pow(10, idp);
	if (num >= 0.0f)
		return (float)(floor(num * mult + 0.5f) / mult);
	else return (float)(ceil(num * mult - 0.5f) / mult);
}

void createMissingMinutes(int _from, int _to, int origTo, float minOpen, float minHigh, float minLow, float minClose, int minute)
{
	//Use values of the "from" minute and create minute data up to the "to" minute - values for to in the ohlc params
	//average the data from the stored data

	float mo = minOpen; //store the "to" values
	float mh = minHigh;
	float ml = minLow;
	float mc = minClose;

	int oldOne = oneOfBar;
	oneOfBar = _from;
	int From = _from;
	int To = _to;

	int bdiff = To - From;

	float dateDiff = 0.0f;

	bool extend = false;
	bool mExtend = false;

	float barAdjustO = 0.0f;	//How to get from _from.open to _to.open
	float barAdjustC = 0.0f;	//How to get from _from.close to _to.close
	float barAdjustH = 0.0f;	//How to get from _from.high to _to.high
	float barAdjustL = 0.0f;	//How to get from _from.low to _to.low

	if (From == 0 && priorOne != 0) // get data from previous save bar
	{
		int lastRec = sf.ohlcvData.size() - 1;
		minOpen = sf.ohlcvData[lastRec].close;
		//DCM 5 min dependency!
		if (gap == false)
			barStartDate = sf.ohlcvData[lastRec].date + 0.00347f; //approximation of +15mins
		else //If due to a gap then set the date to the new gapped bar start
			barStartDate = minDate - (minSize * (minute % 5));
	}
//DCM 5 min dependency!
	int missingBars = (int)((minDate - barStartDate) / 0.00347f);
	if (missingBars > 0 && missingBars < 100)
	{
		bdiff = (missingBars * 5) + (minute % 5);
		mExtend = true;
		To = 4;
	}
	//DCM 5 min dependency!
	if (bdiff < 0 || (missingBars>100 && From == 0)) //we have an end that is in the next bar
	{
		bdiff = bdiff + 5;	//missing data extends into next bar so need to extend
		extend = true;
		To = 4; //force to the end of the current bar
	}

	//Get a starting point
	if (From == 0 && priorOne != 0) // get data from previous save bar
	{
		highVal = minOpen;
		lowVal = minOpen;
		barOpen = minOpen;
		minVolume = 0;

		dateDiff = (float)(minDate - barStartDate);
//DCM 5 min dependency!
		if (dateDiff > 0.00347f && dateDiff<1.93f)
		{
			barStartDate = minDate - (minSize * (5 * missingBars) + (minSize * (minute % 5)));
		}


		//For the first bar we don't have the hlc so base off the open and calc the average values for the missing points
		if (bdiff != 0)	//ensure we don't divide by zero
		{
			barAdjustC = (mo - minClose) / bdiff;
			barAdjustH = (mo - minHigh) / bdiff;
			barAdjustL = (mo - minLow) / bdiff;
		}

		createMinute(0, minOpen, minHigh, minLow, minClose, minOpen);
		oneOfBar = 0;
		saveBar();
	}
	else //create average bars between the two bars
	{
		getMinVal(From, &minOpen, &minHigh, &minLow, &minClose);  //get the ohlc values from the _from need to decrypt from the letters for the _from data

		if (bdiff != 0)	//ensure we don't divide by zero
		{
			barAdjustC = (mc - minClose) / bdiff;
			barAdjustH = (mh - minHigh) / bdiff;
			barAdjustL = (ml - minLow) / bdiff;
		}

		//Just have to hope these weren't the max extremes for the 15min bar.
		//Then need to loop from _from to _to and pass the calulated values through to the minute creator.
		minOpen = minClose;	// mo - barAdjustO;		
		minClose = mc - barAdjustC;
		minHigh = mh - barAdjustH;
		minLow = ml - barAdjustL;
	}

	oldOne = minuteCreator(From, _to, To, oldOne, mo, mh, mh, mc, barAdjustC, barAdjustH, barAdjustL);

	while (extend == true || mExtend == true)
	{
		priorOne = -1;
		From = 0;
		if (mExtend == false) To = _to;
		saveBar();

		int lastRec = sf.ohlcvData.size() - 1;
//DCM 5 min dependency!
		barStartDate = sf.ohlcvData[lastRec].date + 0.00347f; //approximation of +15mins

		if (missingBars>100 && From == 0)
		{
			//		extend = false;
			//reset the ohlc values from mo etc. and then start a new bar....
			minOpen = mo;
			minHigh = mh;
			minLow = ml;
			minClose = mc;
//DCM 5 min dependency!
			barStartDate = minDate - (minSize * (minute % 5));

			barAdjustC = 0;
			barAdjustH = 0;
			barAdjustL = 0;
			bdiff = To - From;
			if (bdiff != 0)	//ensure we don't divide by zero
			{
				barAdjustC = (mo - minClose) / bdiff;
				barAdjustH = (mo - minHigh) / bdiff;
				barAdjustL = (mo - minLow) / bdiff;
			}

			oldOne = minuteCreator(From, _to, To, oldOne, mo, mh, mh, mc, barAdjustC, barAdjustH, barAdjustL);
		}
		else
		{
			if (mExtend == true) To = origTo;

			createMissingMinutes(From, To, origTo, minOpen, minHigh, minLow, minClose, minute);
		}
		extend = false;
//DCM 5 min dependency!
		//THIS FAILS IF THE DIFFERENCE IS ALWAYS > .00347 - WHY??? 1 MIN DATA ERROR 2024 28/11
		if (minDate - barStartDate < 0.00347f) mExtend = false;
	}
}

int minuteCreator(int From, int _to, int To, int oldOne, float mo, float mh, float ml, float mc, float barAdjustC, float barAdjustH, float barAdjustL)
{
	int retVal = oldOne;
	for (int minNum = From + 1; minNum < To; ++minNum)
	{
		if (minNum + 1 == To) minClose = mo;
		createMinute(minNum, minOpen, minHigh, minLow, minClose, minOpen);
		oneOfBar = minNum;

		saveBar();

		minOpen = minClose;	// minOpen + barAdjustO;
		minClose = minClose + barAdjustC;
		minHigh = minHigh + barAdjustH;
		minLow = minLow + barAdjustL;
	}

	oldOne = To;
//DCM 5 min dependency!
	if (From > 0 && To == 4) //missing end so need to populate pos 14
	{
		oldOne = 4;
		retVal = 4;
	}

	//Reset to the known minute values
	if (oldOne != 0 || (oldOne == 0 && minDetails[oldOne] == '!'))
	{
		minOpen = mo;
		minHigh = mh;
		minLow = ml;
		minClose = mc;
		createMinute(oldOne, minOpen, minHigh, minLow, minClose, 0.0f);
	}
	oneOfBar = oldOne;

	return retVal;
}

void getMinVal(int _from, float* mo, float* mh, float* ml, float* mc)
{
	//first calc the min open starting for the bar open
	float baseOpen = barOpen;
	float minStart = baseOpen;
	unsigned char offsetVal = 0;
	unsigned char offsetVal2 = 0;
	float offsetTotal = 0.0f;
	float multi = 1;
	float multi2 = 1;
	int m_int = 1;
	int m_frac = 0;
	for (int i = 0; i < _from; ++i)
	{
		m_int = minDetails[(i * 6)] - 48;
		m_frac = minDetails[(i * 6) + 1] - 48;
		multi = (float)(m_int + (m_frac / 100.0f));

		m_int = minDetails[((i + 1) * 6)] - 48;
		m_frac = minDetails[((i + 1) * 6) + 1] - 48;
		multi2 = (float)(m_int + (m_frac / 100.0f));

		offsetVal = minDetails[(i * 6) + 5];
		offsetTotal += (float)((offsetVal - 83) * multi / 10.0f);
		if (i < _from)
		{
			offsetVal2 = minDetails[((i + 1) * 6) + 2];	//add any opening adjustment 
			offsetTotal += (float)((offsetVal2 - 83) * multi2 / 10.0f);
		}
	}
	minStart = baseOpen - offsetTotal;
	//Recover the text value of the specified minute and convert to floats
	//	char ov = minDetails[(_from * 6) + 2];
	unsigned char hv = minDetails[(_from * 6) + 3];
	unsigned char lv = minDetails[(_from * 6) + 4];
	unsigned char cv = minDetails[(_from * 6) + 5];

	m_int = minDetails[(_from * 6)] - 48;
	m_frac = minDetails[(_from * 6) + 1] - 48;
	multi = (float)(m_int + (m_frac / 100.0f));

	*mo = minStart;	// -(ov - 83) * multi / 100000.0f;
					//Ensure high and low aren't invalid first
	*mh = minStart - (hv - 83) * multi / 10.0f;
	*ml = minStart - (lv - 83) * multi / 10.0f;
	*mc = minStart - (cv - 83) * multi / 10.0f;
}

void createMinute(int minNum, float minOpen, float minHigh, float minLow, float minClose, float _last)
{
	float range = 0.0f;
	float multiplier = 1;

	if (_last > 0.0f) lastOpen = _last;
	float minspread = pointSize * 100.0f;
	float maxspread = pointSize * 1.8f * 100.0f;

	uniform_real_distribution<> rdist(minspread, maxspread);	// (0.00001, 0.00016);

	//ensure the highs/lows match against open close
	if (minOpen > minHigh) minHigh = minOpen;
	if (minOpen < minLow) minLow = minOpen;
	if (minClose > minHigh) minHigh = minClose;
	if (minClose < minLow) minLow = minClose;

	//DCM 5 min dependency!
	//Calculate the 5 mins high/low values
	if (highVal < minHigh) {highVal = minHigh; hSpread = _round((float)rdist(gen), 5);}
	if (lowVal > minLow) {lowVal = minLow;  lSpread = _round((float)rdist(gen), 5);}

	range = minHigh - minLow;
	//Where is the nearest extreme to the open

	if (lastOpen == 0.0f || oneOfBar == 0) lastOpen = minOpen;
	float opn = minOpen - lastOpen;
	float highest = minOpen - minHigh;
	float lowest = minOpen - minLow;
	float closest = minOpen - minClose;

	//Using ascii 40 to 126 being ( to /  0-9  :;<=>?  A-Z  a-z  {|}~ so we have 42 +/- and one for 0 values to use for points
	//Leaves space, !"#$%&' free
	//Mid point is 83 S representing 0
	//Use ! as an invalid value

	int m_int;
	int m_fraction;
	multiplier = max(1.0f, (range * 10) / 42);
	m_int = (unsigned char)multiplier;		//Arbitrarily start at 1
	m_fraction = (unsigned char)((multiplier - m_int) * 100) + 48;
	m_int += 48;
	//ensure multiplier matches the int/fraction version
	multiplier = (float)((m_int - 48) + ((m_fraction - 48) / 100.0f));

	//DCM change multiplier to handle floating point by taking the value calculated as a float, then
	//scale put int part in one char and floating up to 100 in another char

	unsigned char mo = 83 + (unsigned char)roundf((opn * 10.0f) / multiplier);
	unsigned char hi = 83 + (unsigned char)roundf((highest * 10.0f) / multiplier);
	unsigned char lo = 83 + (unsigned char)roundf((lowest * 10.0f) / multiplier);
	unsigned char cl = 83 + (unsigned char)roundf((closest * 10.0f) / multiplier);

	//Regardless of value force to be between  anticipated values;
/*	if (mo < 40) mo = 40;
	if (mo > 126) mo = 126;
	if (hi < 40) mo = 40;
	if (hi > 126) mo = 126;
	if (lo < 40) mo = 40;
	if (lo > 126) mo = 126;
	if (cl < 40) mo = 40;
	if (cl > 126) mo = 126;
*/
	if (mo < 40) mo = 40;
	if (mo > 126) mo = 126;
	if (hi < 40) hi = 40;
	if (hi > 126) hi = 126;
	if (lo < 40) lo = 40;
	if (lo > 126) lo = 126;
	if (cl < 40) cl = 40;
	if (cl > 126) cl = 126;

	minDetails[(minNum * 6)] = (unsigned char)(m_int);		//Integer part of multiplier as the ascii value 48=0
	minDetails[(minNum * 6) + 1] = (unsigned char)(m_fraction);	//Fractional part of multiplier as ascii value 48=0
	minDetails[(minNum * 6) + 2] = mo;						//open offset (either from open or previous close)
	minDetails[(minNum * 6) + 3] = hi;						//high
	minDetails[(minNum * 6) + 4] = lo;						//low
	minDetails[(minNum * 6) + 5] = cl;						//close

	lastOpen = minClose;

	/*	For testing bar recreation
	float _mo = 0.0f;
	float _mh = 0.0f;
	float _ml = 0.0f;
	float _mc = 0.0f;
	reCreateMinute(minNum, minDetails, barOpen, &_mo, &_mh, &_ml, &_mc);
	*/
}

void reCreateMinute(int _minNum, unsigned char _minute[31], float _barOpen, float* mo, float* mh, float* ml, float* mc)
{	//recreate the ohlc for the minute from the char encoded representation and put into the return params
	float baseOpen = _barOpen;
	float minStart = baseOpen;
	unsigned char offsetVal = 0;
	unsigned char offsetVal2 = 0;
	float offsetTotal = 0.0f;
	float multi = 1;
	float multi2 = 1;
	int m_int = 1;
	int m_frac = 0;
	for (int i = 0; i < _minNum; ++i)	//get the minute and next minute scales and total until target minute reached
	{
		m_int = _minute[(i * 6)] - 48;
		m_frac = _minute[(i * 6) + 1] - 48;
		multi = (float)(m_int + (m_frac / 100.0f));

		m_int = _minute[((i + 1) * 6)] - 48;
		m_frac = _minute[((i + 1) * 6) + 1] - 48;
		multi2 = (float)(m_int + (m_frac / 100.0f));

		offsetVal = _minute[(i * 6) + 5];		//get last offset adjustment
		offsetTotal += (float)((offsetVal - 83) * multi / 10.0f);
		if (i < _minNum)
		{
			offsetVal2 = _minute[((i + 1) * 6) + 2];	//add any opening adjustment 
			offsetTotal += (float)((offsetVal2 - 83) * multi2 / 10.0f);
		}
	}
	minStart = baseOpen - offsetTotal;
	//Recover the text value of the specified minute and convert to floats
	//	char ov = minDetails[(_from * 6) + 2];
	unsigned char hv = _minute[(_minNum * 6) + 3];
	unsigned char lv = _minute[(_minNum * 6) + 4];
	unsigned char cv = _minute[(_minNum * 6) + 5];

	m_int = _minute[(_minNum * 6)] - 48;
	m_frac = _minute[(_minNum * 6) + 1] - 48;
	multi = (float)(m_int + (m_frac / 100.0f));

	*mo = minStart;
	*mh = (float)(minStart - ((hv - 83) * multi / 10.0f));
	*ml = (float)(minStart - ((lv - 83) * multi / 10.0f));
	*mc = (float)(minStart - ((cv - 83) * multi / 10.0f));
}

void saveBar()
{
//DCM 5 Min dependency
	if (oneOfBar == 4 || savePartial == true)
	{
		SourceData sd = SourceData();
		MinuteData md = MinuteData();
		sd.open = barOpen;
		sd.high = highVal;
		sd.low = lowVal;
		sd.close = minClose;
		
		//change these random limits as needs to be based on pointsize
		float minspread = pointSize * 100.0f;
		float maxspread = pointSize * 1.8f * 100.0f;

		uniform_real_distribution<> rdist(minspread, maxspread);	// 0.00001, 0.00016);
		cSpread=_round((float)rdist(gen), 5);

		sd.aopen = barOpen + oSpread;
		sd.ahigh = highVal + hSpread;
		sd.alow = lowVal + lSpread;
		sd.aclose = minClose + cSpread;

		sd.volume = minVolume;
		for (int t = 0; t < 31; ++t) { md.mins[t] = minDetails[t]; minDetails[t] = '!'; }
		minDetails[30] = '\0';
		md.mins[30] = '\0';
		sd.date = barStartDate;
		sf.ohlcvData.push_back(sd);
		sf.minData.push_back(md);
		oneOfBar = 0;
		priorOne = -1;
		savePartial = false;
	}
	else
	{
		minDetails[((oneOfBar + 1) * 6)] = '!';
		minDetails[((oneOfBar + 1) * 6) + 1] = '!';
		minDetails[((oneOfBar + 1) * 6) + 2] = '!';
		minDetails[((oneOfBar + 1) * 6) + 3] = '!';
		minDetails[((oneOfBar + 1) * 6) + 4] = '!';
		minDetails[((oneOfBar + 1) * 6) + 5] = '!';
	}
}

int processVolData(double _date)
{
	for (int j = 0; j < (int)(volData.size()); ++j)
	{
		VolSourceData vd = volData[j];
		if (vd.date == _date) return vd.volume;
	}
	return -1;
}


int start(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 1)
		return luaL_error(L, "Insufficient parameters for parameter retrieval.");

	string _instrument = (string)lua_tostring(L, 1);
	int info = _instrument.find("/");
	if (info > 0)
		_instrument = _instrument.replace(_instrument.find("/"), 1, "");

	int retVal = 1;
#ifndef testing
	if (today == last) //we run an analysis
	{
#endif
		//	MessageBox(NULL, "Starting!", "Starting", 0);
		//Ensure last set stored
		dataArray.push_back(sf);

		//prepare the output file
		TCHAR path[500];
		HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);

		const size_t buflen = 100;
		TCHAR buf[buflen];
		_sntprintf_s(buf, buflen - 1, _T("%sohlc.ban"), _instrument.c_str());

		PathAppend(path, buf);	

//		PathAppend(path, "\\ohlc.ban");
		ofile.open(path);
		ofile.precision(10);
		//write the test params out first...
		ofile << minRegdif << " " << maxRegdif << " " << minRegdif2 << " " << maxRegdif2 << " "
			<< minReglinelen << " " << maxReglinelen << " " 
			<< minReglinelimit << " " << maxReglinelimit << " "
			<< minReglinema << " " << maxReglinema << " "
			<< minRVal << " " << maxRVal << " "
			<< minReglinemalen << " " << maxReglinemalen << " " << agg << " "
			<< minStop << " " << stopMax << " "
			<< pipCost << " " << mmr << " " << maxLots << " " << maxloss << " " << pctBalance << " " << contractSize << " "
			<< pointSize << " " << lotAdj << " " << tzofset << " "	<< truStartDate << " "
			<< sess.useSessionControl << " " << sess.sessionOpenHour << " " << sess.sessionOpenMin << " "
			<< sess.sessionCloseHour << " " << sess.sessionCloseMin << " " << sess.timeOffset << " "
			<< sess.excStartTime[0] << " " << sess.excEndTime[0] << " "
			<< sess.excStartTime[1] << " " << sess.excEndTime[1] << " "
			<< sess.excStartTime[2] << " " << sess.excEndTime[2] << " "
			<< sess.excStartTime[3] << " " << sess.excEndTime[3] << " "
			<< sess.excStartTime[4] << " " << sess.excEndTime[4] << " "
			<< sess.overRunTime << " ";

		for (int i = dataArray.size() - 1; i >= 0; --i)
		{
			SourceFile _sf = dataArray[i];
			unsigned char mins[31];
			
			for (int j = 0; j < (int)(_sf.ohlcvData.size()); ++j)
			{
				SourceData sd = _sf.ohlcvData[j];
				MinuteData md = _sf.minData[j];
				
				int _vol=processVolData(sd.date);
				if (_vol != -1) sd.volume = _vol;

				for (int t = 0; t < 31; ++t) mins[t] = md.mins[t];
				//Write the arrays to file so we can pick it up for the analysis
				ofile << sd.open << " " << sd.high << " " << sd.low << " " << sd.close << " " << 
						sd.aopen << " " << sd.ahigh << " " << sd.alow << " " << sd.aclose << " " << 
						mins << " " << sd.volume << " " << sd.date << endl;
			}
		}
		//Write out the file closure record
		ofile << -1.0f << " " << -2.0f << " " << -3.0f << " " << -4.0f << " " << -5 << " " << -6 << " ";	//end with a single -1 in the open vector
		ofile.close();
		//Write data file out - agricultural but easy!!
		HINSTANCE hi = ShellExecute(NULL, "open", "Ziggylauncher.exe", _instrument.c_str(), NULL, SW_HIDE);

		sess.excStartTime.clear();
		sess.excEndTime.clear();
		sf.ohlcvData.clear();
		sf.minData.clear();
		dataArray.clear();
#ifndef testing
	}
	else retVal = 0;
#endif
	lua_pushnumber(L, (int)retVal);
	return retVal;
}

int getparam(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 2)
		return luaL_error(L, "Insufficient parameters for parameter retrieval.");

	//float _rval, int _ma, int _flim, int _agg,
	float retVal1 = 0.0f;
	int retVal2 = 0;
	int retVal3 = 0;
	int retVal4 = 0;
//	int retVal5 = 0;
//	float retVal6 = 0.0f;
//	float retVal7 = 0.0f;
//	float retVal8 = 0.0f;
//	bool retVal9 = false;
//	bool retVal10 = false;
//	bool retVal10a = false;


	int item = (int)lua_tointeger(L, 1);
	string _instrument = (string)lua_tostring(L, 2);
	_instrument=_instrument.replace(_instrument.find("/"), 1, "");

	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sparams.ban"), _instrument.c_str());

	PathAppend(path, buf);
//	PathAppend(path, "\\params.ban");
	ifstream ifile(path);
	if (ifile.is_open())
	{
		int type = 0;
		//look for a flag file - if found then load and update new params
		ifile >> type >> rVal >> ma >> flim >> agg;
		ifile.close();
		switch (item)
		{
		case 0:
			retVal1 = rVal; 
			retVal2 = ma;
			retVal3 = flim;
			retVal4 = agg;
			break;
		}
	}

	lua_pushnumber(L, retVal1);
	lua_pushnumber(L, retVal2);
	lua_pushnumber(L, retVal3);
	lua_pushnumber(L, retVal4);
//	lua_pushnumber(L, retVal5);
//	lua_pushnumber(L, retVal6);
//	lua_pushnumber(L, retVal7);
//	lua_pushnumber(L, retVal8);
//	lua_pushboolean(L, retVal9);
//	lua_pushboolean(L, retVal10);
//	lua_pushboolean(L, retVal10a);
	return 4;

}

int params(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 6)
		return luaL_error(L, "Insufficient parameters for parameter checking.");
	int idx = 1;

	int update = (int)lua_tonumber(L, idx++);
	rVal = (float)lua_tonumber(L, idx++);
	ma = (int)lua_tonumber(L, idx++);
	flim = (int)lua_tonumber(L, idx++);
	agg = (int)lua_tonumber(L, idx++);
	string _instrument = (string)lua_tostring(L, idx);
	int info = _instrument.find("/");
	if (info > 0)
		_instrument=_instrument.replace(_instrument.find("/"), 1, "");

	//Try and find a param file to load. If first param is 1 it is a new update. If 0 it is stored. 
	//If file does not exist create a default
	int type = 0;

	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sparams.ban"), _instrument.c_str());

	PathAppend(path, buf);
//	PathAppend(path, "\\params.ban");
	ifstream ifile(path);
	if (!ifile.is_open())	//No file so create a default
	{
		ifile.close();
		ofstream ofile(path);
		ofile << 0 << " " << rVal << " " << ma << " " << flim << " " << agg;
		ofile.close();
	}
	else // type will be 1 if created by the analysis
	{
		//look for a flag file - if found then load and update new params

		ifile >> type >> rVal >> ma >>  flim >> agg;
		ifile.close();
		if (type == 1 && update==1)
		{
			//Write it back out as a type 0 so we don't reload it as a "new" set of params
			ofstream ofile(path);
			ofile << 0 << " " << rVal << " " << ma << " " << flim << " " << agg;
			ofile.close();
		}
	}
	
	lua_pushnumber(L, type);
	lua_pushnumber(L, rVal);
	lua_pushnumber(L, ma);
	lua_pushnumber(L, flim); 
	lua_pushnumber(L, agg);
	return 5;
}


int setTriggerStart(lua_State* L)
{
	//write the desired value to the trigger file and return the value
	int n = lua_gettop(L);
	if (n != 2)
		return luaL_error(L, "Insufficient parameters for getTriggerStart checking.");

	int setVal = (int)lua_tonumber(L, 1);

	//open the trigger file and return the value
	string _instrument = (string)lua_tostring(L, 2);
	int info = _instrument.find("/");
	if (info > 0)
		_instrument = _instrument.replace(_instrument.find("/"), 1, "");

	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sTriggerStarts.ban"), _instrument.c_str());

	PathAppend(path, buf);
	//	PathAppend(path, "\\params.ban");
	ofstream ofile(path);
	ofile << setVal << endl;
	ofile.close();

	lua_pushnumber(L, setVal);
	return 1;
}

int getTriggerStart(lua_State* L)
{
	int retVal = 0;	//0 = stop, 1 = start

	//open the trigger file and return the value
	string _instrument = (string)lua_tostring(L, 1);
	int info = _instrument.find("/");
	if (info > 0)
		_instrument = _instrument.replace(_instrument.find("/"), 1, "");

	int type = 0;

	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sTriggerStarts.ban"), _instrument.c_str());

	PathAppend(path, buf);
	ifstream ifile(path);
	if (!ifile.is_open())	//No file so create a default
	{
		ifile.close();
		ofstream ofile(path);
		ofile << 0 << endl;
		ofile.close();
	}
	else //look for a 1 to start the analysis
	{
		ifile >> retVal;
		ifile.close();
		if (retVal == 1)
		{
			//Write it back out as a stop 0 so we don't reload it as a start again
			ofstream ofile(path);
			ofile << 0 << endl;
			ofile.close();
		}
	}
	lua_pushnumber(L, retVal);
	return 1;
}


int getStartDate(int currentYear, int currentMonth, int currentDay, int currentHour, int currentMin)
{
	int indMax = 0; //ensure we have more than enough bars for the indicators
	//we have a zl macd so we have multiples of the 12/26/9... but also an MA
	// if more than one indicator use the max of the maxInd e.g. max(maxATR, maxAO);
	indMax = max(26 * 4, maxMAPeriod * 2);
	//Note that indMax is bars not days...
	int indAdj = indMax * 3;	// 0.01042f;	//Ensure we have sufficient room for indicators to work

	//Change so we return the date for the period up to the current day

	today = COleDateTime(currentYear, currentMonth, currentDay, currentHour, currentMin, 0);

	/*COleDateTime start(currentYear, month, day, 0, 0, 0);

	int diff = (int)(today - start);

	int diffFreq = (int)diff / frequency;
	int lastBegan = frequency*diffFreq - diff;

	last = today;
	COleDateTimeSpan dadj(lastBegan);
	last = last + dadj;
	*/

	int _pop = population;
	int _hpop = 0;
	if (hPopulation != 0)
	{
		_hpop = hPopulation;
		_pop = 0;
	}
	COleDateTimeSpan dadj(_pop, _hpop, 0, 0);	//population offset

	COleDateTimeSpan idadj(0, 0, indAdj, 0);	//indicator offset
	COleDateTime next = today;
	next = next - dadj;

	//adjust further for weekends

	int tradingDay = next.GetDayOfWeek();
	truStartDate = (float)next;

	int wea = 0;
	if (tradingDay == 1) wea = 2;
	if (tradingDay == 7) wea = 1;
	if (wea > 0)
	{	
		COleDateTimeSpan weadj(wea, 0, 0, 0);
		truStartDate = (float)(next-weadj);
	}
	truStartDate = (float)(truStartDate - idadj);
	return (int)truStartDate;	//convert to int to truncate and give a small buffer extra of pre processing time
}

/*
int setRestrictionFile(lua_State *L)
{
	int retVal = 0;
	int n = lua_gettop(L);
	if (n != 1)
		return luaL_error(L, "Insufficient parameters for setRestrictionFile.");

	filename = "\\" + (string)lua_tostring(L, 1);

	retVal = loadRestrictions();
	lua_pushnumber(L, retVal);

	return 1;
}


void DatePlusDays(struct tm* date, int days)
{
const time_t ONE_DAY = 24 * 60 * 60;

// Seconds since start of epoch
time_t date_seconds = mktime(date) + (days * ONE_DAY);

localtime_s(date, &date_seconds);
}*/

/*
int loadRestrictions()
{
	string sDate;
	string sHMS;
	string eDate;
	string eHMS;
	int totRecs = 0;

	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	LPCSTR fn = "";
	fn = filename.c_str();

	PathAppend(path, fn);
	ifstream ifile(path);
	if (!ifile.is_open())	//No file so create a default
	{
		ifile.close();
		ofstream ofile(path);
		ofile << "Start dd/mm/yyyy" << ", HH:MM:SS" << ", End dd/mm/yyyy" << ", HH:MM:SS" << endl;
		ofile.close();
	}
	else
	{
		while (!ifile.eof())
		{
			ifile >> sDate >> sHMS >> eDate >> eHMS;
			totRecs++;
			processDateTime(sDate, sHMS, eDate, eHMS);
		}
		ifile.close();
	}
	return totRecs;
}

int getRestriction(lua_State *L)
{
	int n = lua_gettop(L);
	if (n != 1)
		return luaL_error(L, "Insufficient parameters for setRestrictionFile.");

	int recNo = (int)lua_tonumber(L, 1);

	lua_pushnumber(L, datelist[recNo].startDate.year);
	lua_pushnumber(L, datelist[recNo].startDate.month);
	lua_pushnumber(L, datelist[recNo].startDate.day);
	lua_pushnumber(L, datelist[recNo].startDate.hour);
	lua_pushnumber(L, datelist[recNo].startDate.min);
	lua_pushnumber(L, datelist[recNo].startDate.sec);

	lua_pushnumber(L, datelist[recNo].endDate.year);
	lua_pushnumber(L, datelist[recNo].endDate.month);
	lua_pushnumber(L, datelist[recNo].endDate.day);
	lua_pushnumber(L, datelist[recNo].endDate.hour);
	lua_pushnumber(L, datelist[recNo].endDate.min);
	lua_pushnumber(L, datelist[recNo].endDate.sec);
	return 12;
}

void processDateTime(string sDate, string sHMS, string eDate, string eHMS)
{
	restrictedDates rd;
	int _year = 0;
	int _month = 0;
	int _day = 0;
	int _hour = 0;
	int _min = 0;
	int _sec = 0;

	COleDateTime sd, ed, st, et;
	sd.ParseDateTime(sDate.c_str());
	st.ParseDateTime(sHMS.c_str());
	ed.ParseDateTime(eDate.c_str());
	et.ParseDateTime(eHMS.c_str());


	rd.startDate.year = sd.GetYear();
	rd.startDate.month = sd.GetMonth();
	rd.startDate.day = sd.GetDay();
	rd.startDate.hour = st.GetHour();
	rd.startDate.min = st.GetMinute();
	rd.startDate.sec = st.GetSecond();

	rd.endDate.year = ed.GetYear();
	rd.endDate.month = ed.GetMonth();
	rd.endDate.day = ed.GetDay();
	rd.endDate.hour = et.GetHour();
	rd.endDate.min = et.GetMinute();
	rd.endDate.sec = et.GetSecond();

	datelist.push_back(rd);

}
*/

static const luaL_reg lib_functions[] =
{
	{ "setup", setup },
	{ "setTimes", setTimes },
	{ "storeData", storeData },
	{ "storeVolData", storeVolData },
	{ "start", start },
	{ "getparam", getparam },
	{ "params", params },
	{ "getTriggerStart", getTriggerStart },
	{ "setTriggerStart", setTriggerStart },
//	{ "setRestrictionFile", setRestrictionFile },
//	{ "getRestriction", getRestriction },
	{ NULL, NULL }
};

extern "C" int __declspec(dllexport) luaopen_ZiggyAnal(lua_State *L)
{
	luaL_register(L, "ZiggyAnal", lib_functions);
	lua_getglobal(L, "ZiggyAnal");
	if (lua_isnil(L, -1) || !lua_istable(L, -1)) {
		luaL_error(L, "the ZiggyAnal library is not registered");
	}
	return 1;
}