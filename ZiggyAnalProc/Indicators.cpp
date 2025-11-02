#include "Indicators.h"

Indicators::Indicators(vector <SourceData> *_SRC, int _maVal)	//, int _maVal, int _maType)
{
	SRC = _SRC;
//	atrVal = _atrVal;
//	int _maVal = 10;
	int _maType = 1;

	maVal = _maVal;

	rgmaVal = _maVal;

	matype = 3;	//Always 3 for ATR
	MAType = _maType;	//Varies for MA

	if (matype == 2) k = (float)(2.0f / (atrVal + 1.0f));
	if (matype == 3) k = (float)(1.0f / atrVal);

	if (MAType == 2) k1 = (float)(2.0f / (maVal + 1.0f));
	if (MAType == 3) k1 = (float)(1.0f / maVal);

	zk1= (float)(2.0f / (12 + 1.0f));
	zk2= (float)(2.0f / (26 + 1.0f));
	zk3 = (float)(2.0f / (9 + 1.0f));

//	sqrtPeriod = (int)sqrt(atrVal);	//for hma calcs
	sqrtPeriod1 = (int)sqrt(maVal);

}
Indicators::~Indicators()
{
	//tidy up the arrays
//	atrLine.clear();
	mdLine.clear();
	sigLine.clear();
	histogram.clear();

	maLine.clear();

	source.clear();
	trange.clear();
	internal.clear();
	hma1.clear();
	MA1.clear();

	maInternal.clear();

	EMAS.clear();
	EMAS1.clear();
	EMAL.clear();
	EMAL1.clear();
	MVAI.clear();
	MVAI1.clear();

	zlInternal1.clear();
	zlInternal2.clear();
	zlInternal3.clear();
	zlInternal4.clear();
	zlInternal5.clear();
	zlInternal6.clear();
	
	regress_u.clear();
	regress_d.clear();
	RL.clear();
	rgMa.clear();
}


//float Indicators::getMedian(int period)
//{
//	return abs((*SRC)[period].high - (*SRC)[period].low);
//}

//float Indicators::getMedianTick(int period)
//{
//	return abs((*currentBar)[0].high - (*currentBar)[0].low);
//}

float Indicators::getTrueRange(int period)
{
	float hl = abs((*SRC)[period].high - (*SRC)[period].low);
	float hc = abs((*SRC)[period].high - (*SRC)[period-1].close);
	float lc = abs((*SRC)[period].low - (*SRC)[period - 1].close);

	float tr = hl;
	if (tr < hc)
		tr = hc;
	
	if (tr < lc)
		tr = lc;
	
	return tr;
}
/*
float Indicators::getTrueRangeTick(int period)
{
	float hl = abs((*currentBar)[0].high - (*currentBar)[0].low);
	float hc = abs((*currentBar)[0].high - (*SRC)[period - 1].close);
	float lc = abs((*currentBar)[0].low - (*SRC)[period - 1].close);

	float tr = hl;
	if (tr < hc)
		tr = hc;

	if (tr < lc)
		tr = lc;

	return tr;
}*/

float Indicators::average(vector<float>* _tr, int from, int to)
{
	float sum = 0.0f;	
	for (int t = from; t <= to; ++t)
	{
		sum += (*_tr)[t];
	}
	return(sum / (to - from + 1));
}

float Indicators::lwma(vector<float>* _tr, int from, int to)
{
	//Need the close value from the source structure

	float sum = 0.0f;
	int div = 0;
	for (int t = from; t <= to; ++t)
	{
		div += (t-from+1);
		sum += ((*_tr)[t]*(t-from+1));
	}
	return(sum / div);
}

float Indicators::ema(vector<float>* _tr, int from, int to, vector<float>* _int, int _first, float _k)
{
	float retVal = 0.0;
	if (to >= from)
	{
		float value = 0;
		float sourcePeriod = (*_tr)[to];
		if (to == from)
		{
			value = sourcePeriod;

		}
		else
		{
			value = (*_int)[to-1];
		}
		(*_int).push_back((1 - _k) * value + _k * sourcePeriod);
	}

	if (to >= _first)
		retVal = (*_int)[to];	// *10;

	return retVal;
}

//Change this as failing on max
// Function to calculate the highest high over a given length
float Indicators::highest(vector<float>* source, int length) 
{
	if (source->size() < length) return 0.0f;
	return *std::max_element(source->end() - length, source->end());
}

// Function to calculate the lowest low over a given length
float Indicators::lowest(vector<float>* source, int length) 
{
	if (source->size() < length) return 0.0f;
	return *std::min_element(source->end() - length, source->end());
}

// Function to calculate the average of two values
float Indicators::avg(float a, float b) {
	return (a + b) / 2.0f;
}


/*
//------------------------ATR
void Indicators::preloadATR()
{
	int srcMax = (*SRC).size();
	int period = 0;

	int first = atrVal + 1;
	int trfirst = 1;

	for (period = 0; period < srcMax; ++period)
	{
		if (period >= trfirst)
			trange.push_back(getTrueRange(period));
		else
			trange.push_back(abs((*SRC)[period].high - (*SRC)[period].low));

		if (period >= first)
		{
			if (matype == 1) atrLine.push_back(average(&trange, period - atrVal + 1, period));
			if (matype == 2) atrLine.push_back(ema(&trange, period - atrVal + 1, period, &internal, first, k));
			if (matype == 3) atrLine.push_back(ema(&trange, period - atrVal + 1, period, &internal, first, k));
			if (matype == 4) atrLine.push_back(lwma(&trange, period - atrVal + 1, period));
			if (matype == 5)
			{
				float val = lwma(&trange, period - (int)(atrVal / 2) + 1, period) * 2;
				hma1.push_back(val - lwma(&trange, period - atrVal + 1, period));
				atrLine.push_back(lwma(&hma1, period - sqrtPeriod + 1, period));
			}	//hma
		}
		else
		{
			internal.push_back(trange[period]);
			atrLine.push_back(trange[period]);
		}
	}
}
*/

/*void Indicators::prepATR(int period)
{
	//Preload the tick version of the true range
	int trfirst = 1;

	for (int i = period - atrVal + 1; i < period; ++i)
	{
		trangeTick.push_back(trange[i]);
		internalTick.push_back(internal[i]);
	}

	trangeTick.push_back(0.0f);
	internalTick.push_back(0.0f);
//	if (period >= trfirst)
//		trangeTick.push_back(getTrueRange(period));
//	else trangeTick.push_back(0.0f);
}*/

/*
void Indicators::updateATR(int period, vector<bar>* _currentBar)
{
	currentBar = _currentBar;
	int srcMax = trangeTick.size()-1;	
	int first = atrVal + 1;
	int trfirst = 1;

//	float pOpen = 0.0f, pClose = 0.0f;

	//Then push the current range into the last vector
	if (period >= trfirst)
		trangeTick[srcMax]=getTrueRangeTick(period);
	else trangeTick[srcMax]=0.0f;

	int trl = trangeTick.size();
	if (period >= first)
	{
		if (matype == 1) atrTick = average(&trangeTick, 0, trl - 1);
		if (matype == 2) atrTick = ema(&trangeTick, 0, trl - 1, &internalTick, first, k);
		if (matype == 3) atrTick = ema(&trangeTick, 0, trl - 1, &internalTick, first, k);
		if (matype == 4) atrTick = lwma(&trangeTick, 0, trl - 1);
		if (matype == 5)
		{
			float val = lwma(&trangeTick, 0, trl - 1) * 2;
			hma1Tick.push_back(val - lwma(&trangeTick, 0, trl - 1));
			atrTick=lwma(&hma1Tick, 0, trl - 1);
		}	//hma
	}
	else	
		atrTick = trangeTick[period];
}
*/

/*void Indicators::clearATRTick()
{
	trangeTick.clear();
	hma1Tick.clear();
}*/

/*
//------------------------Impulse MACD
void Indicators::preloadImpMacd()
{
	int srcMax = (*SRC).size();
	int period = 0;

	for (period = 0; period < srcMax; ++period)
	{
		impHLC.push_back(((*SRC)[period].high + (*SRC)[period].low + (*SRC)[period].close) / 3);

		if (period >= iMaLen+1)
		{
			impEMA1.push_back(ema(&impHLC, period - iMaLen + 1, period, &impInternal1, iMaLen+1, impzk1));
		}
		else
		{
			impInternal1.push_back((*SRC)[period].close);
			impEMA1.push_back(impHLC[period]);
		}

		if (period >= iMaLen+1)
		{
			EMAS1.push_back(ema(&EMAS, period - SN + 1, period, &zlInternal2, firstSS, zk1));
		}
		else
		{
			EMAS1.push_back(EMAS[period]);
			zlInternal2.push_back(EMAS[period]);
			//zlInternal2.push_back(0.0f);
		}

		if (period >= firstL)
		{
			EMAL.push_back(ema(&source, period - LN + 1, period, &zlInternal3, firstL, zk2));
		}
		else
		{
			EMAL.push_back((*SRC)[period].close);
			zlInternal3.push_back((*SRC)[period].close);
		}

		if (period >= firstLL)
		{
			EMAL1.push_back(ema(&EMAL, period - LN + 1, period, &zlInternal4, firstLL, zk2));

			float zerolagfast = ((2 * EMAS[period]) - EMAS1[period]);
			float zerolagslow = ((2 * EMAL[period]) - EMAL1[period]);
			mdLine.push_back(zerolagfast - zerolagslow);
		}
		else
		{
			EMAL1.push_back(EMAL[period]);
			zlInternal4.push_back(EMAL[period]);
			//zlInternal4.push_back(0.0f);
			float zerolagfast = ((2 * EMAS[period]) - EMAS1[period]);
			float zerolagslow = ((2 * EMAL[period]) - EMAL1[period]);
			mdLine.push_back(zerolagfast - zerolagslow);
			//mdLine.push_back(0.0);
		}


		if (period >= firstLLI)
		{
			// update EMA on the MACD and secondary EMA on that result
			MVAI.push_back(ema(&mdLine, period - IN + 1, period, &zlInternal5, firstLLI, zk3));
		}
		else
		{
			MVAI.push_back(mdLine[period]);
			zlInternal5.push_back(mdLine[period]);
			//zlInternal5.push_back(0.0f);
		}

		if (period >= firstLLII)
		{
			MVAI1.push_back(ema(&MVAI, period - IN + 1, period, &zlInternal6, firstLLII, zk3));
			sigLine.push_back(((2 * MVAI[period]) - MVAI1[period]));
			//--calculate histogram as a difference between MACDand signal
			histogram.push_back(mdLine[period] - sigLine[period]);
		}
		else
		{
			MVAI1.push_back(MVAI[period]);
			zlInternal6.push_back(MVAI[period]);
			//			zlInternal6.push_back(0.0f);
			sigLine.push_back(((2 * MVAI[period]) - MVAI1[period]));
			histogram.push_back(mdLine[period] - sigLine[period]);
			//			sigLine.push_back(0.0f);
			//			histogram.push_back(0.0f);

		}

	}
}
*/

/*
//------------------------ZL MACD
void Indicators::preloadZlMacd()
{
	int srcMax = (*SRC).size();
	int period = 0;

	int SN = 12;
	int LN = 26;
	int IN = 9;

	int firstS = SN + 1;
	int firstL = LN + 1;
	int firstSS = SN + SN + 1;
	int firstLL = LN + LN + 1;
	int firstLLI = LN + LN + IN + 1;
	int firstLLII = LN + LN + IN + IN + 1;

	for (period = 0; period < srcMax; ++period)
	{
//		source.push_back((*SRC)[period].close);  //already populated by MA
		if (period >= firstS)
		{
			EMAS.push_back(ema(&source, period - SN + 1, period, &zlInternal1, firstS, zk1));
		}
		else
		{
			zlInternal1.push_back((*SRC)[period].close);
			EMAS.push_back((*SRC)[period].close);
		}

		if (period >= firstSS)
		{
			EMAS1.push_back(ema(&EMAS, period - SN + 1, period, &zlInternal2, firstSS, zk1));
		}
		else
		{
			EMAS1.push_back(EMAS[period]);
			zlInternal2.push_back(EMAS[period]);
			//zlInternal2.push_back(0.0f);
		}

		if (period >= firstL)
		{
			EMAL.push_back(ema(&source, period - LN + 1, period, &zlInternal3, firstL, zk2));
		}
		else
		{
			EMAL.push_back((*SRC)[period].close);
			zlInternal3.push_back((*SRC)[period].close);
		}

		if (period >= firstLL)
		{
			EMAL1.push_back(ema(&EMAL, period - LN + 1, period, &zlInternal4, firstLL, zk2));

			float zerolagfast = ((2 * EMAS[period]) - EMAS1[period]);
			float zerolagslow = ((2 * EMAL[period]) - EMAL1[period]);
			mdLine.push_back(zerolagfast - zerolagslow);
		}
		else	
		{
			EMAL1.push_back(EMAL[period]);
			zlInternal4.push_back(EMAL[period]);
			//zlInternal4.push_back(0.0f);
			float zerolagfast = ((2 * EMAS[period]) - EMAS1[period]);
			float zerolagslow = ((2 * EMAL[period]) - EMAL1[period]);
			mdLine.push_back(zerolagfast - zerolagslow);
			//mdLine.push_back(0.0);
		}


		if (period >= firstLLI)
		{
			// update EMA on the MACD and secondary EMA on that result
			MVAI.push_back(ema(&mdLine, period - IN + 1, period, &zlInternal5, firstLLI, zk3));
		}
		else
		{
			MVAI.push_back(mdLine[period]);
			zlInternal5.push_back(mdLine[period]);
			//zlInternal5.push_back(0.0f);
		}

		if (period >= firstLLII)
		{
			MVAI1.push_back(ema(&MVAI, period - IN + 1, period, &zlInternal6, firstLLII, zk3));
			sigLine.push_back(((2 * MVAI[period]) - MVAI1[period]));
			//--calculate histogram as a difference between MACDand signal
			histogram.push_back(mdLine[period] - sigLine[period]);
		}
		else
		{
			MVAI1.push_back(MVAI[period]);
			zlInternal6.push_back(MVAI[period]);
//			zlInternal6.push_back(0.0f);
			sigLine.push_back(((2 * MVAI[period]) - MVAI1[period]));
			histogram.push_back(mdLine[period] - sigLine[period]);
//			sigLine.push_back(0.0f);
//			histogram.push_back(0.0f);

		}

	}
}
*/

/*void prepZLMacd(int period)
{

}*/

/*void updateZLMacd(int period, vector<bar>* _currentBar)
{

}*/

/*void Indicators::clearZLMacdTick()
{
	mdTick.clear();
	sgTick.clear();
}
*/
//---------------------------MA

void Indicators::preloadMA()
{
	int srcMax = (*SRC).size();
	int period = 0;

	int first = maVal + 1;

	for (period = 0; period < srcMax; ++period)
	{
		
		source.push_back((*SRC)[period].close);
		
		if (period >= first)
		{
			if (MAType == 1) maLine.push_back(average(&source, period - maVal + 1, period)); //simple
			if (MAType == 2) maLine.push_back(ema(&source, period - maVal + 1, period, &maInternal, first, k1)); //ema
			if (MAType == 3) maLine.push_back(ema(&source, period - maVal + 1, period, &maInternal, first, k1)); //rma
			if (MAType == 4) maLine.push_back(lwma(&source, period - maVal + 1, period));	//lwma
			if (MAType == 5)
			{
				float val1 = lwma(&source, period - (int)(maVal / 2) + 1, period) * 2;
				maInternal.push_back(val1 - lwma(&source, period - maVal + 1, period));
				maLine.push_back(lwma(&maInternal, period - sqrtPeriod1 + 1, period));
			}
		}
		else
		{
			maLine.push_back((*SRC)[period].close);
			maInternal.push_back((*SRC)[period].close);
		}
	}
}

//----------------------------Reg
// Function to calculate linear regression (linreg)
float Indicators::linreg(std::vector<float>& source, int length, int offset)
{
	if (source.size() < length + offset) return 0.0;
	float sumX = 0.0, sumY = 0.0, sumXY = 0.0, sumX2 = 0.0;
	int n = length;
	for (int i = 0; i < n; ++i) {
		int x = i;
//		float y = source[source.size() - 1 - i - offset];
		float y = source[source.size() - n + i - offset];
		sumX += x;
		sumY += y;
		sumXY += x * y;
		sumX2 += x * x;
	}
	float slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
	float intercept = (sumY - slope * sumX) / n;
	return slope * (n - 1) + intercept;
}


void Indicators::preloadReg(int ba)
{
	int srcMax = (*SRC).size();
	int period = 0;


	int first = rgmaVal-1;	// +1;


	for (period = 0; period < srcMax; ++period)
	{
		source.push_back((*SRC)[period].close);

		if (ba == 0)
		{
			srcB.push_back((*SRC)[period].close);
			srcBL.push_back((*SRC)[period].low);
			srcBH.push_back((*SRC)[period].high);
		}
		if (ba == 1)
		{
			srcA.push_back((*SRC)[period].aclose);
			srcAL.push_back((*SRC)[period].low);
			srcAH.push_back((*SRC)[period].high);
		}

		vector<float>* srcba = &srcB;
		vector<float>* srcbah = &srcBH;
		vector<float>* srcbal = &srcBL;
		vector<float>* rgmaInternal = &rgmaInternalB;
		vector<float>* maLineba = &maLineB;

		if (ba == 1) { srcba = &srcA; srcbah = &srcAH; srcbal = &srcAL; rgmaInternal = &rgmaInternalA; maLineba = &maLineA; }
		

		if (period >= first)	//-1)
		{
			//--get highest, lowest
			float minv = lowest(srcbal, rgmaVal-1);	// , period);
			float maxv = highest(srcbah, rgmaVal-1);	// , period);
			//--get avg close


			if (matypeR == 1) maLineba->push_back(average(srcba, period - rgmaVal + 1, period)); //simple
			if (matypeR == 2) maLineba->push_back(ema(srcba, period - rgmaVal + 1, period, rgmaInternal, first, k1)); //ema
			if (matypeR == 3) maLineba->push_back(ema(srcba, period - rgmaVal + 1, period, rgmaInternal, first, k1)); //rma
			if (matypeR == 4) maLineba->push_back(lwma(srcba, period - rgmaVal + 1, period));	//lwma
			if (matypeR == 5)
			{
				float val1 = lwma(srcba, period - (int)(rgmaVal / 2) + 1, period) * 2;
				rgmaInternal->push_back(val1 - lwma(srcba, period - rgmaVal + 1, period));
				maLineba->push_back(lwma(rgmaInternal, period - sqrtPeriod1 + 1, period));
			}


			float cl = (*maLineba)[period - (rgmaVal - 1)];		//average(srcba, period - rgmaVal + 1, period);
			float avg1 = avg(maxv, minv);
			float avg2 = avg(avg1, cl);

			if (ba == 0) regress_u.push_back(source[period] - avg2);
			if (ba == 1) regress_d.push_back(source[period] - avg2);
			//--get reg

			float b = 0.0;

			if (ba == 0) {b = linreg(regress_u, rgmaVal, 0); regressU.push_back(b); }
			if (ba == 1) {b = linreg(regress_d, rgmaVal, 0); regressD.push_back(b); }
		}
		else
		{
			if (ba == 0)
			{
				regressU.push_back(0);
				regress_u.push_back(0);
			}
			if (ba == 1)
			{
				regressD.push_back(0);
				regress_d.push_back(0);
			}
		}

		
	}

/*		if (period >= firstPeriod)
		{

			if(matype==1) ml = mathex.avg(source.close, period - N + 1, period); end	--simple
			if(matype==2) ml = ema(source.close, period - N + 1, period); end	--ema
			if(matype==3) then ml = ema(source.close, period - N + 1, period); end	--rma
			if(matype==4) then ml = mathex.lwma(source.close, period - N + 1, period); end	--lwma
			if(matype==5) then
				val= mathex.lwma(source.close, period - toInt(N/2) + 1, period) * 2;
				hma1[period]=val-mathex.lwma(source.close, period - N + 1, period);
				ml = mathex.lwma(hma1, period - sqrtPeriod + 1, period);
			end
		
		--get highest, lowest
		local minv, maxv, minpos, maxpos = mathex.minmax(source, period - N + 1, period);
		--get avg close
		local cl = mathex.avg(source.close, period - N + 1, period);
        local avg1 = avg(maxv, minv);
        local avg2 = avg(avg1, cl);
		
		regress[period]=source.close[period] - avg2;
		--get reg
		
		local b=0.0;
		local m=0.0;
		if(regType==0) then
			b=linreg(regress, N, 0);
		else
			b, m = getRegression(N, regress, period);
		end
		
		if(UD==true) then
			RL[period]=nil;
			RLD[period]=nil;

			if(b>lastRL) then
				RL[period-1]=lastRL;
				RL[period]=b;
			end
			if(b<lastRL) then
				RLD[period-1]=lastRL;
				RLD[period]=b;
			end
		else
			RL[period]=b;
		end	
		lastRL=b;
	end
	
--	val = linreg(source  -  avg(avg(highest(high, lengthKC), lowest(low, lengthKC)),sma(close,lengthKC)), lengthKC,0)
*/


}


//void Indicators::MA(vector<float>* _tr, int from, int to)
//{
//
//}

/*void Indicators::prepMA(int period)
{
	//Preload the tick version
	int first = 1;

	for (int i = period - maVal + 1; i < period; ++i)
	{
		trangeTick.push_back(trange[i]);
		internalTick.push_back(internal[i]);
	}

	trangeTick.push_back(0.0f);
	internalTick.push_back(0.0f);
	//	if (period >= trfirst)
	//		trangeTick.push_back(getTrueRange(period));
	//	else trangeTick.push_back(0.0f);

}*/

/*
void Indicators::updateMA(int period, vector<bar>* _currentBar)
{
	currentBar = _currentBar;
	int srcMax = (*SRC).size();
	int first = maVal + 1;
//	int first = 1;

	//	float pOpen = 0.0f, pClose = 0.0f;


		//Then push the current range into the last vector
	if (period >= first)
		MATick[srcMax] = getTrueRangeTick(period);
	else MATick[srcMax] = 0.0f;

	int trl = trangeTick.size();
	if (period >= first)
	{
		if (matype == 1) atrTick = average(&trangeTick, 0, trl - 1);
		if (matype == 2) atrTick = ema(&trangeTick, 0, trl - 1, &internalTick, first);
		if (matype == 3) atrTick = ema(&trangeTick, 0, trl - 1, &internalTick, first);
		if (matype == 4) atrTick = lwma(&trangeTick, 0, trl - 1);
	}
	else
		atrTick = trangeTick[period];
}

void Indicators::clearMATick()
{

}
*/

