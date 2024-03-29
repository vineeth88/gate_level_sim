#include "incl.h"
//#include "gateLevelSim.h"
#include "rtLevelSim.h"
#include <algorithm>
#include <ctime>
#include <sstream>

vecIn_t genSequence(rtLevelCkt *ffCkt, rtLevelCkt *fvCkt);

void RandomVecIn(vecIn_t& vecIn, int size) {

	vecIn = string(size, '0');
    for (uint i = 0; i < vecIn.length(); i++) {
		if((i+1) % NUM_INPUT_BITS == 0)
			vecIn[i] = '0';
		else
			vecIn[i] = (rand() & 0x01) + '0';

		#if defined(__b12)
		if (i%NUM_INPUT_BITS == 0) {
			if (rand() % 10002 > 10000)
				vecIn[i] = '1';
			else
				vecIn[i] = '0';
		}
		#endif
	}

	assert(vecIn.length() % NUM_INPUT_BITS == 0);
}

int main(int argc, char** argv) {
		
//	string ffStr(NUM_STATE_BITS, '0');
//	string fvStr(NUM_STATE_BITS, '0');

	Vtop* ffCktVar = new Vtop;
	Vtop* fvCktVar = new Vtop;
	rtLevelCkt *ffCkt = NULL, *fvCkt = NULL;

	uint seed = time(NULL) << 2 ;
	srand(seed);

	ffCkt = new rtLevelCkt(ffCktVar);
	fvCkt = new rtLevelCkt(fvCktVar);
	// rtLevelCkt *ffCkt = new rtLevelCkt(ffCktVar);
	// rtLevelCkt *fvCkt = new rtLevelCkt(fvCktVar);
	
	char cktName[80];
	if (argc < 2) 
		strcpy(cktName, benchCkt);
	else
		strcpy(cktName, argv[1]);
	
	#ifndef OnlySimulate
	char sTableName[256];
	sprintf(sTableName, "%s.ustb", cktName);
	cout << "Reading states from " << sTableName << endl;

	ifstream sTableFile;
	sTableFile.open(sTableName, ios::in);
	if (sTableFile == NULL) {
		cerr << "Unable to open file " << sTableName << endl
			 << "Exiting ... " << endl;
	}

	int NUM_UNIQUE_STATES = 0;
	string line;
	stringstream ss;
	getline(sTableFile, line);
	ss << line;
	ss >> NUM_UNIQUE_STATES;

//	sTableFile >> NUM_UNIQUE_STATES;
//	cout << NUM_UNIQUE_STATES << endl;
	vector<string> uStateVec(NUM_UNIQUE_STATES);
	
	int stateIdx = 0;
	while (sTableFile && (stateIdx < NUM_UNIQUE_STATES)) {
		string line;
		getline(sTableFile, line);

		if(line.length() != (NUM_STATE_BITS + NUM_OUTPUT_BITS)) {
			cout << line << endl;
			cout << line.length() << " v " << NUM_STATE_BITS << " " 
				 << NUM_OUTPUT_BITS << endl;
//			assert(0);
		}
		else {
			uStateVec[stateIdx] = line;
			stateIdx++;
		}
	}
	sTableFile.close();
	#endif

	//#define OnlySimulate
	#ifdef OnlySimulate
	vector<vecIn_t> inpVecSeq;
	strcpy(ffCkt->cktName, cktName);
	ffCkt->readVecFile(inpVecSeq);
	string initState(NUM_STATE_BITS, '0');
	ffCkt->setCktState(cktState(initState.c_str(),0));
	ffCkt->setCktOutput(string(NUM_OUTPUT_BITS, '0'));

	for (int idx = 0; idx < inpVecSeq.size(); ++idx) {
		vecIn_t vecIn = inpVecSeq[idx];
		ffCkt->simOneVector(vecIn);
		vector<int> branch_vec;
		ffCkt->getBranchCounters(branch_vec);
		string ffOut = ffCkt->getOutputs();
		string ffState = ffCkt->getCktState();
		cout << ffOut << ffState << endl;
		for(vector<int>::iterator it = branch_vec.begin(); 
				it != branch_vec.end(); ++it)
			cout << *it << " ";
		cout << endl;
	}

	#endif

	#ifndef OnlySimulate

	ifstream stateFile;
	char stateFName[256];
	sprintf(stateFName, "%s.states", cktName);
	
	cout << stateFName << endl;
	stateFile.open(stateFName, ios::in);
	
	if (stateFile == NULL) {
		cout << "Unable to open file " << stateFName << endl;
		exit(-1);
	}
	while(stateFile) {
		string line;
		getline(stateFile, line);
		
		int vecNo, faultNo;
		int ffIdx, fvIdx;
		string ffStr, fvStr; 
		string ffString, fvString; 
		string ffOut, fvOut;
		
		int ind = line.find(";");
//		cout << "L: " << ind << endl
//			 << string::npos << endl;
		if (ind == (int)string::npos)
			continue;
		
		stringstream ss;
		ss << line.substr(0,ind);
		ss >> ffIdx >> fvIdx >> faultNo >> vecNo;
	
		ffString = uStateVec[ffIdx];
		fvString = uStateVec[fvIdx];

		ffStr =	ffString.substr(NUM_OUTPUT_BITS,NUM_STATE_BITS);
		fvStr = fvString.substr(NUM_OUTPUT_BITS,NUM_STATE_BITS);
		ffOut = ffString.substr(0,NUM_OUTPUT_BITS);
		fvOut = fvString.substr(0,NUM_OUTPUT_BITS);

		cktState ffState = cktState(ffStr.c_str(), vecNo);
		cktState fvState = cktState(fvStr.c_str(), vecNo);

//		ss << line.substr(2*ind+4);
//		ss >> faultNo >> vecNo;

		cout << endl
			 << ffOut << ffStr << endl
			 << fvOut << fvStr << endl;

		int NUM_TRIES = 20;
		for (int tries = 0; tries < NUM_TRIES; ++tries) {
			
			ffCkt->setCktState(ffState);
			fvCkt->setCktState(fvState);
			ffCkt->setCktOutput(ffOut);
			fvCkt->setCktOutput(fvOut);

			vecIn_t inVecSeq;
			inVecSeq = genSequence(ffCkt, fvCkt);
			if(inVecSeq.compare("NOT FOUND")) {
				assert(inVecSeq.length() % NUM_INPUT_BITS == 0);
				cout << faultNo << " " 
					 << vecNo << endl
					 << NUM_INPUT_BITS << endl;
				for (uint idx = 0; idx < inVecSeq.length()/NUM_INPUT_BITS; ++idx)
					cout << inVecSeq.substr(idx*NUM_INPUT_BITS, NUM_INPUT_BITS) << endl;
				cout << "END" << endl;
				break;
			}
			else {
				cout << faultNo << " " << vecNo << " " << inVecSeq << endl;
			}
		}
	}	

	#endif
	return 0;
}

vecIn_t genSequence(rtLevelCkt *ffCkt, rtLevelCkt *fvCkt) {

//	#define PRINT_OUTPUTS_STATES
	// TODO: 
	// Need to first tackle faults which were propogated to a FF in the last cycle, 
	// Then target faults which were excited and propogated for some time

	int SEQ_LEN = 1000;
	vecIn_t inpVecSeq;
	RandomVecIn(inpVecSeq, NUM_INPUT_BITS * SEQ_LEN);

	int statusFlag = -1, last_index = -1;
	for (int idx = 0; idx < SEQ_LEN; ++idx) {
		vecIn_t vecIn = inpVecSeq.substr(idx*NUM_INPUT_BITS, NUM_INPUT_BITS);
		vecIn[NUM_INPUT_BITS-1] = '0';
		ffCkt->simOneVector(vecIn);
		fvCkt->simOneVector(vecIn);

		string ffOut = ffCkt->getOutputs();
		string fvOut = fvCkt->getOutputs();
	
		#ifdef PRINT_OUTPUTS_STATES
		cout << idx << ": " << vecIn << endl;
//		cout 
//			 << ffOut << endl
//			 << fvOut << endl;
		#endif

		if (ffOut.compare(fvOut) == 0) {
			string ffState = ffCkt->getCktState();
			string fvState = fvCkt->getCktState();
			#ifdef PRINT_OUTPUTS_STATES
			cout << ffOut << ffState << endl
				 << fvOut << fvState << endl;
			#endif

			if (ffState.compare(fvState) == 0) {
				#ifdef PRINT_OUTPUTS_STATES
				cout << "End of Dist Seq." << endl;
				#endif
//				cout << idx << ":" << ffOut << ffState << endl
//					 << idx << ":" << fvOut << fvState << endl;
				statusFlag = 1;
				break;
			}
		}
		else {
			cout << "Fault propogated to output" << endl;
			#ifdef PRINT_OUTPUTS_STATES
			cout << ffOut << ffCkt->getCktState() << endl
				 << fvOut << fvCkt->getCktState() << endl;
			#endif
			statusFlag = 0;
			last_index = idx+1;	// 0-idx = idx+1 vectors
			break;
		}
	}
	
	string retString;
	if (statusFlag)
		retString = string("NOT FOUND");
	else
		retString = inpVecSeq.substr(0,last_index*NUM_INPUT_BITS);

	return retString;
}
