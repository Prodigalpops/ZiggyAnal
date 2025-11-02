//Minuteman
// ZiggyAnalProc.cpp : Defines the entry point for the console application.
//
//MINUTE DATA IS NOW PASSED TO EACH THREAD AS A SEPARATE REFERENCE. INCLUDING IT AS PART OF THE OHLCDATA MEANT A BAD SLOW DOWN (2 -> 20 MINS)
//PROBABLY TRYING TO COPY THE CHAR DATA RATHER THAN REFERENCING IT.

//NOTE - DEC 2024 - IMPLEMENTS THE LINEAR_B3 CODE. LINREG JUST MODIFIED. NEEDS PARAMETERS TO BE ADJUSTED IN THE ANALYSIS AND SOURCE DATA CREATION CODE
//					THIS NEEDS TO FEED THROUGH TO THE PROC CODE. MAY STILL HAVE MEMORY CORRUPTION IN THE THREADING OF ACCT STRUCTURE

#define testing
#include "ZiggyAnalProc.h"
#include "Shellapi.h"
using namespace std;


/*void analyse(int _atr, float _atrl, int _map, int _mat,
				float _stopVal, float _backstop, float _rVal,
				float _dailyTgt, bool _useDailyTgt, bool _invert,
				Account* acct, int _procNum, int _tzo, float _truStart,
				bool _useSess,	int _sessOpen, int _sessClose, int _sessOff, 
				vector<int>* _excStart, vector<int>* _excEnd,
				int _sessOverrun)*/

/*void analyse(int _map, int _mat, float _tr, float _fe, float _ft, int _fxt,
				Account* acct, int _procNum)*/
//void analyse(int _rl, int _fl, float _rd, float _rd2, float _rval, float _be, int _ma, Account* acct, int _procNum)
void analyse(float _rval, int _ma, int _flim, int _agg, Account* acct, int _procNum)
{


	Strategy* strat = new Strategy(acct, &ohlcvData, &zigData, &minData, 
				0, 0, 0,
			//	_rl, _fl, _rd, _rd2, _rval, _be, _ma,
				_rval, _ma, _flim, _agg,
				pipCost, mmr, maxLots, maxloss, pctBalance,
				contractSize, pointSize, lotAdj, _procNum, tzofset, truStart,
				useSessionControl, sessOpen, sessClose, sessOff, &excStart, &excEnd,
				overRunTime);
				
	//Now run the tests
	bool runOk = false;
	bool added = false;

	if (acct != nullptr)
	{
		while (runOk == false)
		{

			strat->runBT(false);
			try
			{
				added = false;
				//ditch any result with no positive balance other than the first so we always have at least 1 return
				if ((results.size() == 0 && acct->Balance >= 50000) || acct->Balance > 50000)
				{
					results.push_back(acct);
					added = true;
				}


				if (added == true && results.size() > 0 && results[results.size() - 1] != acct)
					results.pop_back();
				else
					runOk = true;
			}
			catch (...)
			{
				int vv = 1;
			}

		}
	}

	try
	{
		delete strat;
	}
	catch (...)
	{
		int vv = 1;
	}
}

int main(int argc, char** argv)
{
	instrument = argv[1];

	if (argc == 3 )
	{	
		if(getOHLCV() == true)
			return(testAnalysis());
	}


	unsigned tmax = thread::hardware_concurrency() - 1;	//Don't want to be greedy and likely a better processing time overall!
	if (tmax > 10) tmax = 10;
//	tmax = 4;
	unsigned tnum = 0;
	int procNum = 0;
//	bool useInv = invertT;
//	int invertNum = 0;
// maxMAPeriod = 100;

/*	float minRegDiff = 0.5f;
	float maxRegDiff = 2.5f;
	float minBeVal = -0.5f;
	float maxBeVal = 0.5f;
*/
	int minAgg = 0;
	int maxAgg = 1;

	float rd2 = 2.0f;
	int rl = 4;


	if (getOHLCV() == true)
	{	//PUT THE TESTS IN HERE
		//Loop for... range filter >=15 <=30, mavg >=15 <=30, rval >=1.0 <=2.0, agg Y/N

		for (float rv = minRVal; rv <=maxRVal; rv += 0.1f)	//rval
		{
			for (int _mavg = minReglinemalen; _mavg <= maxReglinemalen; ++_mavg)
			{
				for (int flm = minReglinelimit; flm <= maxReglinelimit; ++flm)
				{
					for (int ag = minAgg; ag <= maxAgg; ++ag)
					{

						Account* acct = new Account();
						//update variables to be ma length, rval, range filter and aggressive Y/N
						thrd[tnum++] = thread(analyse, rv, _mavg, flm, ag, acct, procNum++);

						if (tnum >= tmax)
						{
							//Now join them so we can restart with a new set when they are done
							for (unsigned t = 0; t < tnum; ++t) { thrd[t].join(); }
							tnum = 0;
						}
					}
				}
			}
		}

		//Tidy up any remaining threads
		for (unsigned t = 0; t < tnum; ++t) { thrd[t].join(); }
	}

/*
	try 
	{

		//-----------------------------------------------------------------------------------------------------------
				//Write out the profitable processed combinations...
				//read old values and write out again with new values from specific date so there is a rolling window
				//store datetime, Balance, Period, Type, Invert, Rval

		rollBackUps();	//copy current files to prior backup

		//Now sort the results by balance etc.
		sortResults(NULL, 1);
*/
/*
		//Two files, historic analysis data - the dates and balances for 30 days (analysis), 
		//and historic data - the count and bias for each param set (history)
		TCHAR path[500];

		char strtemp[10];
		bool loaded = false;

		HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		const size_t buflen = 100;
		TCHAR buf[buflen];
		_sntprintf_s(buf, buflen - 1, _T("%sanalysis.ban"), instrument.c_str());

		PathAppend(path, buf);
		ifstream ifile(path);

		int recount = 0;

		if (!ifile.is_open())	//No file so create a default
		{
			ifile.close();
			ofstream ofile(path);
			ofile << "DateTime Balance Period Type "<< endl; //Invert InvVal RVal RVal2 Bias" << endl;

			int _bias = 1;
			float oldBal = results[0]->Balance;
			for (int ix = 0; ix < (int)results.size(); ++ix)
			{
				if (results[ix]->Balance > 50000.0f)
				{
					if (oldBal != results[ix]->Balance) { oldBal = results[ix]->Balance;  ++_bias; }
					ofile << truStart << " " << results[ix]->Balance << " " << results[ix]->MAPeriod << " "
						<< results[ix]->MAType << " " 
						// << results[ix]->invert << " " << results[ix]->invVal << " "
						//<< results[ix]->rVal << " " << results[ix]->rVal2 << " "
						<< _bias << endl;
				}
			}
			ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
			ofile.close();

			//now add the new data to the end
			_bias = 1;
			oldBal = results[0]->Balance;
			for (int ix = 0; ix < (int)results.size(); ++ix)
			{
				if (results[ix]->Balance > 50000.0f)
				{
					if (oldBal != results[ix]->Balance) { oldBal = results[ix]->Balance;  ++_bias; }
					Histdata hstd = Histdata();
					hstd.Date = truStart;
					hstd.Balance = results[ix]->Balance;
					hstd.MAPeriod = results[ix]->MAPeriod;
					hstd.MAType = results[ix]->MAType;
//					hstd.Invert = results[ix]->invert;
//					hstd.InvVal = results[ix]->invVal;
//					hstd.rVal = results[ix]->rVal;
//					hstd.rVal2 = results[ix]->rVal2;
					hstd.Bias = _bias;
					hstd.del = false;
					histd.push_back(hstd);
				}
			}

		}
		else
		{	//subsequent runs - load the raw data, but flag for deletion if it is older than current truStart-30 days.
			//Also skip if the date is the same as the current date as will be replaced
			//Then add new data and write back out
			//dump the header
			ifile >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp;

			loaded = false;
			while (loaded == false)
			{
				Histdata hstd = Histdata();

				//read the history file
				ifile >> hstd.Date >> hstd.Balance >> hstd.MAPeriod >> hstd.MAType >> 
					//hstd.Invert >> hstd.InvVal >> hstd.rVal >> hstd.rVal2 >> 
					hstd.Bias;
				if (hstd.Date < 0) loaded = true;
				else
				{
					if (hstd.Date < truStart - 30)	//include but use for modifying totals
						hstd.del = true;	//dump data older than 30 days - flag for deletion
					if ((int)hstd.Date < (int)truStart)	//skip if current date
						histd.push_back(hstd);
				}
			}
			ifile.close();

			//now add the new data to the end
			int _bias = 1;
			float oldBal = results[0]->Balance;
			for (int ix = 0; ix < (int)results.size(); ++ix)
			{
				if (results[ix]->Balance > 50000.0f)
				{
					if (oldBal != results[ix]->Balance) { oldBal = results[ix]->Balance;  ++_bias; }
					Histdata hstd = Histdata();
					hstd.Date = truStart;
					hstd.Balance = results[ix]->Balance;
					hstd.MAPeriod = results[ix]->MAPeriod;
					hstd.MAType = results[ix]->MAType;
//					hstd.Invert = results[ix]->invert;
//					hstd.InvVal = results[ix]->invVal;
//					hstd.rVal = results[ix]->rVal;
//					hstd.rVal2 = results[ix]->rVal2;
					hstd.Bias = _bias;
					hstd.del = false;
					histd.push_back(hstd);
				}
			}

			//Now rewrite the historic analysis file without those flagged for deletion
			//This could fail if there are no balance results > 50000!!
			ofstream ofile(path);
			ofile << "DateTime Balance Period Type 
					//Invert InvVal RVal RVal2 
					Bias" << endl;
			
			for (int ix = 0; ix < (int)histd.size(); ++ix)
			{
				if (histd[ix].del == true) continue;
				if (histd[ix].Balance > 50000.0f)
					ofile << histd[ix].Date << " " << histd[ix].Balance << " " << histd[ix].MAPeriod << " "
					<< histd[ix].MAType << " " 
					// << histd[ix].Invert << " " << histd[ix].InvVal << " "
					// << histd[ix].rVal << " " << histd[ix].rVal2 << " " 
					<< histd[ix].Bias << endl;
					++recount;
			}
			ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
			ofile.close();
			if (recount == 0) return 0;	//i.e. histd has data
		}


		//		//Now establish the top 5 results and then reprocess with the two remaining parameters
		//		getTop5();
		//		double bal = finalParams[0].Balance;

				//now look at the historic param data
		rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		_sntprintf_s(buf, buflen - 1, _T("%shistory.ban"), instrument.c_str());
		PathAppend(path, buf);
		ifile.open(path);
		if (!ifile.is_open())	//No file so create a default
		{
			ifile.close();
			loadBlankHistory();*/
			/*			ofstream ofile(path);
						ofile << "Key Period Type Invert Count Bias 0" << endl;

						long key = 0;
						for (int map = minMAPeriod; map <= maxMAPeriod; map += 5)
						{
							for (int mat = minMAType; mat <= maxMAType; ++mat)
							{
								for (int inv = 1; inv <= 2; ++inv)	//adjust by 1 so true/false have a define value rather than 0 for false - but adjust when saving
								{
									key = map*100 + mat * 10 + inv;
									ofile << key << " " << map << " " << mat << " " << inv-1 << " " << 0 << " " << 0 << endl;
								}
							}
						}
						ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
						ofile.close();
			*/

/*		}

		//Check the start date for the data analysed. If it matches the lastrun date then resinstate the backup file
		//update the history
		// re/open the history file and dump the header line
		if (!ifile.is_open()) ifile.open(path);
		float lastrun = 0;
		ifile >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> lastrun;

		if ((int)lastrun == (int)histd[histd.size() - 1].Date)	//run date already been processed so use the backup and reporcess it
		{
			ifile.close();
//			loadBlankHistory();
			backUpRestoreSummation(1);	//0=backup, 1=restore
			ifile.open(path);	//reopen the file
			ifile >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> lastrun;
		}

		loaded = false;
		while (loaded == false)
		{
			History hst = History();

			//read the history file
			ifile >> hst.Key >> hst.Period >> hst.Type >> 
			//hst.Invert >> hst.InvVal >> 
			hst.Count >> hst.Bias;
			if (hst.Key == -1) loaded = true;
			else
				hist.push_back(hst);

		}
		ifile.close();

		//Next - run through the positive results in histd and update the history table with the bias and increment the count for each match
		vector<History>::iterator it;
		for (int ix = 0; ix < (int)histd.size(); ++ix)
		{
			//ignore anything less than the lastRun date, but include lastRun or more
			if (histd[ix].Date < lastrun) continue;

//			int inv = 1;
//			if (histd[ix].Invert == true) inv = 2;
//			int inv2 = 1;
//			if (histd[ix].InvVal == true) inv2 = 2;

			long key = histd[ix].MAPeriod * 1000 + histd[ix].MAType * 100;// +inv * 10 + inv2;

			it = find_if(hist.begin(), hist.end(), History::Finder(key));
			if (it != hist.end())
			{
				if (histd[ix].del == true)	//reduce the count for those being deleted
				{
					it->Count--;	//reduce the count 
					it->Bias -= histd[ix].Bias;	//reduce the bias by the deleted item
				}
				else
				{
					it->Bias += histd[ix].Bias;
					it->Count++;
				}
			}
		}

		backUpRestoreSummation(0);		//0=backup, 1=restore

		sortHist(&hist);


		//write out the historic summations
		ofstream ofile(path);
		ofile << "Key Period Type Invert InvVal Count Bias " << truStart << endl;

		long key = 0;
		for (int ix = 0; ix < (int)hist.size(); ++ix)
		{
			ofile << hist[ix].Key << " " << hist[ix].Period << " " << hist[ix].Type << " " 
			// << hist[ix].Invert << " " << hist[ix].InvVal << " " 
			<< hist[ix].Count << " " << hist[ix].Bias << endl;
		}
		ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
		ofile.close();

		//		backUpRestoreSummation(0);		//0=backup, 1=restore


		//------------------------------------------------------------------------------------
				//Now analyse the historic summations and compare the best in the current data to the 
				//historic
				//Scan the histd data and pick the highest count/lowest bias from hist

		//		sortResults(&hist, 2);



		vector<Histdata> topVals;
		float lastBal = 0.0f;	// = histd[0].Balance;
		//topVals.push_back(histd[0]);
		int balcount = 1;

		//get the data associated with the top two balances
		int ix = 0;
		while (ix<histd.size() && histd[ix].Date < truStart) ++ix;
		if (ix == histd.size()) return 1;	//fails here


		lastBal = histd[ix].Balance;

		while (ix < (int)histd.size() && balcount < 3)
		{
			if (histd[ix].Date < truStart) { ++ix; continue; }
			if (histd[ix].Balance < lastBal) { lastBal = histd[ix].Balance; ++balcount; }
			topVals.push_back(histd[ix++]);
		}

		//Now have a subset of the balances and sorted historic totals.
		//Now extract a subset of the totals
		vector<History> topTots;

		for (int ix = 0; ix < (int)topVals.size(); ++ix)
		{
			//lookup the matching hist records
//			int inv = 1;
//			if (topVals[ix].Invert == true) inv = 2;
//			int inv2 = 1;
//			if (topVals[ix].InvVal == true) inv2 = 2;
//			long key = topVals[ix].MAPeriod * 1000 + topVals[ix].MAType * 100 + inv * 10 + inv2;

			it = find_if(hist.begin(), hist.end(), History::Finder(key));
			if (it != hist.end())
			{
				topTots.push_back(*it);
			}
		}

		//now resort by count
		sortHist(&topTots);

		vector<int> bestidx;;
		float ratio = 0.0f;
		float bestRatio = 100.0f;
		int bestCount = 0;
		//Now calc the best ratio and select the best params
		for (int ix = 0; ix < (int)topTots.size(); ++ix)
		{
			if (topTots[ix].Bias != 0)
				ratio = (float)topTots[ix].Bias / topTots[ix].Count;
			if (ratio > 0.0f && ratio < bestRatio && (float)topTots[ix].Count>2) 
			{ bestRatio = ratio; bestidx.push_back(ix); ++bestCount; }
		}

		//now have a possible list of best indices with the best ratio - take the params from that
		//the ratio normalises the average position in the balance list, so the more times it's been profitable and
		//the closer to the max it is the lower the ratio is. i.e. 
		// 5 counts at pos bias 1 =   5 / 1 = 5 = better
		// 5 counts at pos bias 10 = 5 / 10 = 2


		if (bestidx.size() > 0)	//can be zero if first run or no profitable trades
		{
			int bestid = bestidx[bestidx.size() - 1];

			//prepare the output file
			rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
			_sntprintf_s(buf, buflen - 1, _T("%sparams.ban"), instrument.c_str());

			PathAppend(path, buf);
			//		PathAppend(path, "\\params.ban");

			ofile.open(path);
			ofile.precision(10);
			//write the new params out... 
			ofile << 1 << " " 
				<< topTots[bestid].Period << " " << topTots[bestid].Type << " "
			// << results[0]->stopVal << " " << results[0]->backStop << " "
			//	<< results[0]->rVal << " " << results[0]->rVal2 << " " << results[0]->dailyTgt << " " << results[0]->useDailyTgt << " "
			//	<< topTots[bestid].Invert << " " << topTots[bestid].InvVal << " "
				<< endl;

			ofile.close();
		}

		saveCurrentFiles();

		topVals.clear();
		topTots.clear();
	}
	catch (...)
	{
		MessageBox(NULL, "Aww crap! Trying to write params.", "Boom!", 0);
	}
*/
	try
	{
		//Now find the best and send the updated parameters back to the dll so the strategy can be updated.
		//Need to make sure that parameters are read in from file in the strategy
		getTop5();

		//prepare the output file
		TCHAR path[500];
		HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
		const size_t buflen = 100;
		TCHAR buf[buflen];
		_sntprintf_s(buf, buflen - 1, _T("%sparams.ban"), instrument.c_str());
		PathAppend(path, buf);
//		PathAppend(path, "\\params.ban");
		ofstream ofile;
		ofile.open(path);
		ofile.precision(10);

		//WHY IS REGlINEMALEN BEING SAVE AS 1 ???

		//write the test params out first...
		ofile << 1 << " " << finalParams[0].rVal << " " << finalParams[0].regLinemalen 
			<< " " << finalParams[0].regLinelimit << " " << finalParams[0].aggVal << endl;

		ofile.close();
	}
	catch (...)
	{
		MessageBox(NULL, "Aww crap! Trying to write params.", "Boom!", 0);
	}

/*	try
	{
		int ts = results.size()-1;
		for (int t = ts; t >=0; --t)
		{
			//if (results[t]!=nullptr)
			delete results[t];
		}

		results.clear();
		
		for(int t=0; t<5; ++t)
			top5[t]=nullptr;

		top5.clear();
	}
	catch (...)
	{
		int vv = 1;
	}
*/
	
	return 0;
}

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

void rollBackUps()
{
	//Read the history.ban file header and get the lastrun date.
	//Compare with truStart and don't backup if the same
	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%shistory.ban"), instrument.c_str());
	PathAppend(path, buf);
	ifstream ifile(path);
	float lastrun = 0;
	char strtemp[10];
	if (ifile.is_open())
	{
		ifile >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> lastrun;
		if (lastrun == truStart) return;
	}

	//move the historic backups to the folder before andover write the most recent backup with the current files
	//first check for directories and crate if absent
	TCHAR path1[500];
	TCHAR path2[500];
	TCHAR path3[500];

	const size_t buflen1 = 500;
	TCHAR buf1[buflen1];

	const size_t buflen2 = 500;
	TCHAR buf2[buflen2];

	const size_t buflen3 = 500;
	TCHAR buf3[buflen3];

	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path1);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path2);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path3);

	_sntprintf_s(buf1, buflen1 - 1, _T("%s_BackUp_1"), instrument.c_str());
	PathAppend(path1, buf1);

	_sntprintf_s(buf2, buflen2 - 1, _T("%s_BackUp_2"), instrument.c_str());
	PathAppend(path2, buf2);

	_sntprintf_s(buf3, buflen3 - 1, _T("%s_BackUp_3"), instrument.c_str());
	PathAppend(path3, buf3);

	string command1="";
	string command2="";
	string command3="";
	//check the directories exist
	if (dirExists(path1) == false) { command1 = "mkdir "; command1 = command1 + path1; system(command1.c_str());}//make dir 
	if (dirExists(path2) == false) { command2 = "mkdir "; command2 = command2 + path2; system(command2.c_str());}//make dir
	if (dirExists(path3) == false) { command3 = "mkdir "; command3 = command3 + path3; system(command3.c_str());}//make dir

	//now copy files from 2->3, 1->2 and write new files to 1 later
	command1 = "copy ";
	command1 = command1 + path2+"\\*.* ";
	command1 = command1 + path3;
	system(command1.c_str());

	command2 = "copy ";
	command2 = command2 + path1 + "\\*.* ";
	command2 = command2 + path2;
	system(command2.c_str());
}


void saveCurrentFiles()
{
	TCHAR path1[500];
	TCHAR path2[500];
	TCHAR path3[500];
	TCHAR path4[500];
	TCHAR path5[500];

	const size_t buflen1 = 500;
	TCHAR buf1[buflen1];

	const size_t buflen2 = 500;
	TCHAR buf2[buflen2];

	const size_t buflen3 = 500;
	TCHAR buf3[buflen3];

	const size_t buflen4 = 500;
	TCHAR buf4[buflen4];

	const size_t buflen5 = 500;
	TCHAR buf5[buflen5];


	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path1);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path2);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path3);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path4);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path5);

	_sntprintf_s(buf1, buflen1 - 1, _T("%sanalysis.ban "), instrument.c_str());
	PathAppend(path1, buf1);

	_sntprintf_s(buf2, buflen2 - 1, _T("%shistory.ban "), instrument.c_str());
	PathAppend(path2, buf2);

	_sntprintf_s(buf3, buflen3 - 1, _T("%shistory.bak "), instrument.c_str());
	PathAppend(path3, buf3);

	_sntprintf_s(buf4, buflen4 - 1, _T("%sparams.ban "), instrument.c_str());
	PathAppend(path4, buf4);

	_sntprintf_s(buf5, buflen5 - 1, _T("% s_BackUp_1"), instrument.c_str());
	PathAppend(path5, buf5);



	string command1 = "";

	command1 = "copy /Y ";
	command1 = command1 + path1;
	command1 = command1 + path5;
	system(command1.c_str());

	command1 = "copy /Y ";
	command1 = command1 + path2;
	command1 = command1 + path5;
	system(command1.c_str());

	command1 = "copy /Y ";
	command1 = command1 + path3;
	command1 = command1 + path5;
	system(command1.c_str());

	command1 = "copy /Y ";
	command1 = command1 + path4;
	command1 = command1 + path5;
	system(command1.c_str());
}


void loadBlankHistory()
{
	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];

	_sntprintf_s(buf, buflen - 1, _T("%shistory.ban"), instrument.c_str());
	PathAppend(path, buf);
//	ifstream ifile(path);

	ofstream ofile(path);
	ofile << "Key Period Type Invert InvVal Count Bias 0" << endl;

	long key = 0;
/*	for (int map = minMAPeriod; map <= maxMAPeriod; map += 5)
	{
		for (int mat = minMAType; mat <= maxMAType; ++mat)
		{
			for (int inv = 1; inv <= 2; ++inv)	//adjust by 1 so true/false have a define value rather than 0 for false - but adjust when saving 
			{
				for (int inv2 = 1; inv2 <= 2; ++inv2)	//adjust by 1 so true/false have a define value rather than 0 for false - but adjust when saving 
				{
					key = map * 1000 + mat * 100 + inv * 10 + inv2;
					ofile << key << " " << map << " " << mat << " " << inv - 1 << " " << inv2 - 1 << " " << 0 << " " << 0 << endl;
				}
			}
		}
	}
*/
	ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
	ofile.close();
}


/*TO BE COMPLETED
//Load the analysis file and recalculate the bias numbers USE PARAM REGEN
void reGenBias()
{
	TCHAR path[500];

	char strtemp[10];
	bool loaded = false;

	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sanalysis.ban"), instrument.c_str());

	PathAppend(path, buf);
	ifstream ifile(path);

	ifile >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp >> strtemp;

	loaded = false;
	while (loaded == false)
	{
		Histdata hstd = Histdata();

		//read the history file
		ifile >> hstd.Date >> hstd.Balance >> hstd.MAPeriod >> hstd.MAType >> hstd.Invert >> hstd.rVal >> hstd.Bias;
		if (hstd.Date < 0) loaded = true;
		else
		{
			if (hstd.Date < truStart - 30)	//include but use for modifying totals
				hstd.del = true;	//dump data older than 30 days - flag for deletion
			if (hstd.Date < truStart)	//skip if current date
				histd.push_back(hstd);
		}
	}
	ifile.close();


	ofstream ofile(path);
	ofile << "DateTime Balance Period Type Invert RVal Bias" << endl;

	for (int ix = 0; ix < (int)histd.size(); ++ix)
	{
		if (histd[ix].del == true) continue;
		if (histd[ix].Balance > 50000.0f)
			ofile << histd[ix].Date << " " << histd[ix].Balance << " " << histd[ix].MAPeriod << " "
			<< histd[ix].MAType << " " << histd[ix].Invert << " " << histd[ix].rVal << " " << histd[ix].Bias << endl;
	}
	ofile << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << " " << -1 << endl;
	ofile.close();

}
*/

void backUpRestoreSummation(int br)
{
	//Save the previous summations file
	TCHAR path[500];
	TCHAR path2[500];

	const size_t buflen = 500;
	TCHAR buf[buflen];

	const size_t buflen2 = 500;
	TCHAR buf2[buflen2];

	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path2);

	_sntprintf_s(buf, buflen - 1, _T("%shistory.ban "), instrument.c_str());
	PathAppend(path, buf);

	_sntprintf_s(buf2, buflen2 - 1, _T("%shistory.bak "), instrument.c_str());
	PathAppend(path2, buf2);

	string command = "copy /Y ";
	command = command + path;
	command = command + path2;
	if (br == 1)
	{
		command = "copy /Y ";
		command = command + path2;
		command = command + path;
	}
	system(command.c_str());
}


int partition(int start, int end, int which, void* dat)
{
	double pivot;
//	double pivot2;
//	double _date;
	long pivot3;
	long pivot4;

	int count = 0;

	switch (which)
	{
		case 1:
		{
			pivot = results[start]->key;
//			_date = results[start]->Date;

			for (int i = start + 1; i <= end; i++)
			{
				if (results[i]->key >= pivot) count++;	//(results[i]->Date >=_date && results[i]->key >= pivot)
			}

			// Giving pivot element its correct position
			int pivotIndex = start + count;
			std::swap(results[pivotIndex], results[start]);

			// Sorting left and right parts of the pivot element
			int i = start, j = end;

			while (i < pivotIndex && j > pivotIndex)
			{
				while (results[i]->key >= pivot) { i++; }	//(results[i]->Date >= _date && results[i]->key >= pivot)
				while (results[j]->key < pivot) { j--; }	//(results[j]->Date < _date && results[j]->key < pivot)
				if (i < pivotIndex && j > pivotIndex) { std::swap(results[i++], results[j--]); }
			}
			return pivotIndex;
			break;
		}
		case 2:
		{			
			vector<History>* hist = (vector<History>*)dat;
			pivot3 = (*(hist))[start].Count;
			pivot4 = (*(hist))[start].Bias;

			for (int i = start + 1; i <= end; i++)
			{
				if ((*(hist))[i].Count <= pivot3 && (*(hist))[i].Bias >= pivot4) count++;	//(results[i]->Date >=_date && results[i]->key >= pivot)
			}

			// Giving pivot element its correct position
			int pivotIndex = start + count;
			std::swap((*(hist))[pivotIndex], (*(hist))[start]);

			// Sorting left and right parts of the pivot element
			int i = start, j = end;

			while (i < pivotIndex && j > pivotIndex)
			{
				while ((*(hist))[i].Count <= pivot3 && (*(hist))[i].Bias >= pivot4) { i++; }	//(results[i]->Date >= _date && results[i]->key >= pivot)
				while ((*(hist))[j].Count > pivot3 && (*(hist))[j].Bias < pivot4) { j--; }	//(results[j]->Date < _date && results[j]->key < pivot)
				if (i < pivotIndex && j > pivotIndex) { std::swap((*(hist))[i++], (*(hist))[j--]); }
			}
			return pivotIndex;
			break;

		}
	}
	return 0;
}

void quickSort(int start, int end, int which, void* dat)
{

	// base case
	if (start >= end)
		return;

	// partitioning the array
	int p = partition(start, end, which, dat);

	// Sorting the left part
	quickSort(start, p - 1, which, dat);

	// Sorting the right part
	quickSort(p + 1, end, which, dat);
}

void sortResults(void* dat, int which)
{
	switch (which)
	{
		case 1:
			quickSort(0, (int)results.size() - 1, which, dat);
			break;

		case 2:
			quickSort(0, (int)hist.size() - 1, which, dat);
			break;
	}

}


void getTop5()
{
	int t5 = 0;
	unsigned t = 0;
	bool placed = false;
	//prep top5
	top5.push_back(nullptr);
	top5.push_back(nullptr);
	top5.push_back(nullptr);
	top5.push_back(nullptr);
	top5.push_back(nullptr);

	while (t < results.size())
	{
		t5 = 0;
		placed = false;

		while (placed == false && t5 < 5)
		{
			if (top5[t5] == nullptr) { top5[t5] = results[t]; placed = true; }
			else
			{
				if (results[t]->Balance > top5[t5]->Balance)	// && results[t]->minBalance >= top5[t5]->minBalance)
				{
					shiftTop5(t5);
					top5[t5] = results[t];
					placed = true;
				}
				else
					++t5;
			}
		}
		++t;
	}

	//copy the top 5 into a new array so we don't lose the data when we zap the results
	for (int t = 0; t < 5; ++t)
	{
		if (top5[t] != nullptr)
		{
			finalParams[t] = *top5[t];
		}
	}
}

void shiftTop5(int t)
{
	for (int y = 4; y > t; --y)
	{
		top5[y] = top5[y - 1];
	}
}


void sortHist(void *dat)
{
	vector<History>* hist = (vector<History>*)dat;
	
	int oc = 0;
	int ob = 0;
	for (int x = 0; x < (int)(*(hist)).size(); ++x)
	{
		for (int y = 0; y < (int)(*(hist)).size(); ++y)
		{
//			if (x == y) continue;
			if ((*(hist))[x].Count > (*(hist))[y].Count) std::swap((*(hist))[x], (*(hist))[y]);
		}
	}

	for (int x = 0; x < (int)(*(hist)).size(); ++x)
	{
		oc = (*(hist))[x].Count;
		for (int y = 0; y < (int)(*(hist)).size(); ++y)
		{
//			if (x == y) continue;
			if ((*(hist))[x].Bias < (*(hist))[y].Bias && (*(hist))[y].Count==oc) std::swap((*(hist))[x], (*(hist))[y]);
		}
	}
}



bool testAnalysis()
{
	bool retVal = false;
	//get the params from the param file and launch the back test with printed output for the param set
	TCHAR path[500];
	HRESULT rf;
	rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);
	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sparams.ban"), instrument.c_str());

	PathAppend(path, buf);
//	PathAppend(path, "\\params.ban");
	ifstream ifile(path);
	int type = 0;
	int partialPips = 0;
	int priorTrail = 0;

	ifile >> type >> rVal >> ma >> flim >> agg;

	ifile.close();
	Account* acct = new Account();


	Strategy strat(acct, &ohlcvData, &zigData, &minData, 
		0, 0, 0,
		//regLinelen, regLinelimit, regDif, regDif2, rVal, breakEven, regLinema,
		rval, ma, flim, agg,
		pipCost, mmr, maxLots, maxloss, pctBalance, 
		contractSize, pointSize, lotAdj, 0, tzofset, truStart, 
		useSessionControl, sessOpen, sessClose, sessOff,
		&excStart, &excEnd,
		overRunTime);

	strat.runBT(true);
	retVal = true;

	return retVal;
}

bool getOHLCV()
{	
	TCHAR path[500];
	HRESULT rf = SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path);

	const size_t buflen = 100;
	TCHAR buf[buflen];
	_sntprintf_s(buf, buflen - 1, _T("%sohlc.ban"), instrument.c_str());

	PathAppend(path, buf);	//"HAMAO_tick.ban");

//	PathAppend(path, "\\ohlc.ban");
	ifstream ifile(path);
	if (!ifile.is_open())
	{
		MessageBox(NULL, "Do not touch that which you do not understand.", "Unauthorised access!", 0);
		return false;
	}
	
	//Read the params in...
//	ifile >> minATR >> maxATR >> minRval >> maxRval >> minVmul >> maxVmul >> minStop >> maxStop >> pipCost >> mmr >> maxLots >> 
//			maxlosspct >> pctBalance >> contractSize >> pointSize >> scale1 >> scale2 >> tzofset >> truStart;
	//Prep the session structure
	excStart.push_back((int)0);
	excEnd.push_back((int)0);
	excStart.push_back((int)0);
	excEnd.push_back((int)0);
	excStart.push_back((int)0);
	excEnd.push_back((int)0);
	excStart.push_back((int)0);
	excEnd.push_back((int)0);
	excStart.push_back((int)0);
	excEnd.push_back((int)0);


	ifile >> minRegdif >> maxRegdif >> minRegdif2 >> maxRegdif2
		>> minReglinelen >> maxReglinelen
		>> minReglinelimit >> maxReglinelimit
		>> minReglinema >> maxReglinema
		>> minRVal >> maxRVal
		>> minReglinemalen >> maxReglinemalen >>  agg
		>> minStop >> stopMax
		>> pipCost >> mmr >> maxLots >> maxloss >> pctBalance >> contractSize
		>> pointSize >> lotAdj >> tzofset >> truStart
		>> useSessionControl >> sessionOpenHour >> sessionOpenMin
		>> sessionCloseHour >> sessionCloseMin >> sessOff
		>> excStart[0] >> excEnd[0]
		>> excStart[1] >> excEnd[1]
		>> excStart[2] >> excEnd[2]
		>> excStart[3] >> excEnd[3]
		>> excStart[4] >> excEnd[4]
		>> overRunTime;
/*
	ifile >> minATR >> maxATR >> minATRLimit >> maxATRLimit
		>> minMAPeriod >> maxMAPeriod >> minMAType >> maxMAType
		>> minStop >> maxStop
		>> minBackStop >> maxBackStop
		>> minRval >> maxRval >> minRval2 >> maxRval2 >> minDailyTgt >> maxDailyTgt
		>> useDailyT >> useDailyF >> invertT >> invertF >> invValT >> invValF
		>> pipCost >> mmr >> maxLots >> maxloss >> pctBalance >> contractSize
		>> pointSize >> lotAdj >> tzofset >> truStart
		>> useSessionControl >> sessionOpenHour >> sessionOpenMin
		>> sessionCloseHour >> sessionCloseMin >> sessOff
		>> excStart[0] >> excEnd[0]
		>> excStart[1] >> excEnd[1]
		>> excStart[2] >> excEnd[2]
		>> excStart[3] >> excEnd[3]
		>> excStart[4] >> excEnd[4]
		>> overRunTime;
*/
	sessOpen = sessionOpenHour * 100 + sessionOpenMin + tzofset;
	sessClose = sessionCloseHour * 100 + sessionCloseMin + tzofset;

	bool loaded = false;
	float iVal1 = 0.0f;
	float iVal2 = 0.0f;
	float iVal3 = 0.0f;
	float iVal4 = 0.0f;
	float iVal1a = 0.0f;
	float iVal2a = 0.0f;
	float iVal3a = 0.0f;
	float iVal4a = 0.0f;
	int iVal5 = 0;
	double iVal6 = 0.0f;
	char iVal7[31];

	while (loaded == false)
	{
		SourceData sd = SourceData();
		MinuteData md = MinuteData();

		ifile >> iVal1 >> iVal2 >> iVal3 >> iVal4 >> iVal1a >> iVal2a >> iVal3a >> iVal4a >> iVal7 >>iVal5 >> iVal6;
		if (iVal1 < 0.0f) loaded = true;
		else
		{
			sd.open = iVal1;
			sd.high = iVal2;
			sd.low = iVal3;
			sd.close = iVal4;
			sd.aopen = iVal1a;
			sd.ahigh = iVal2a;
			sd.alow = iVal3a;
			sd.aclose = iVal4a;
			for (int t = 0; t < 31; ++t) { md.mins[t]=iVal7[t]; }
			sd.volume = iVal5;
			sd.date = iVal6;
			ohlcvData.push_back(sd);
			minData.push_back(md);
		}
	}
	ifile.close();

	//Now process the minute data into zig zag data
	//processZigData(&ohlcvData);
	
	//Tidy up the transfer file
#ifndef testing
	remove(path);
#endif // !testing
	return true;
}

/*bool processZigData(vector <SourceData>* SRC)
{
	//
	//based on different time frame. Take the source data and establish the values for the higher time frame
	//e.g. if base is 5 and higher 10 min bars, need the o from first source bar, c from second source bar and max/min of the 2 bars from the 2 bars
	//
	int endPeriod = (*(SRC)).size();
	int inc = 2;

	for (int period = 0; period < endPeriod; period+=inc)
	{
		SourceData sd = SourceData();
		sd.open = (*(SRC))[period].open;

		//MODIFY THE +1 AS WON'T EXIST AT LAST ENTRY
		float ehigh = 0.0f;
		float ehigha = 0.0f;
		float elow = 0.0f;
		float elowa = 0.0f;
		float eclose = 0.0f;
		float eclosea = 0.0f;
		int evol = 0;

		if (period + 1 >= endPeriod)
		{
			ehigh = (*(SRC))[period].high;
			elow = (*(SRC))[period].low;
			ehigha = (*(SRC))[period].ahigh;
			elowa = (*(SRC))[period].alow;
			evol = (*(SRC))[period].volume;
		}
		else
		{
			ehigh = (*(SRC))[period + 1].high;
			elow = (*(SRC))[period + 1].low;
			eclose = (*(SRC))[period + 1].close;
			ehigha = (*(SRC))[period + 1].ahigh;
			elowa = (*(SRC))[period + 1].alow;
			eclosea = (*(SRC))[period + 1].aclose;
			evol = (*(SRC))[period + 1].volume;
		}


		sd.high = max((*(SRC))[period].high, ehigh);
		sd.low = min((*(SRC))[period].low, elow);
		sd.close = eclose;
		sd.aopen = (*(SRC))[period].aopen;
		sd.ahigh = max((*(SRC))[period].ahigh, ehigha);
		sd.alow = min((*(SRC))[period].alow, elowa);
		sd.aclose = eclosea;
		sd.volume = (*(SRC))[period].volume + evol;
		sd.date = (*(SRC))[period].open;

		zigData.push_back(sd);
	}

	return false;
}
*/