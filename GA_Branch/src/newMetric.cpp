#include "verInclude.h"
#include "rtLevelSim.h"
#include "incl.h"
#include "verilated.h"

using namespace std;

#ifdef MEM_ALLOC_DBG_ON
int state_t::mem_alloc_cnt = 0;
#endif

double computeMetric(state_pVec&, rtLevelCkt*);
double computeMetric(const state_pVec&, const int_vec&);

int main(int argc, char** argv) {
	
	if (argc < 2) {	
		cout << "Usage " << argv[0] << " <ckt> " << endl;
		exit(-1);
	}

	char vecName[256];
	sprintf(vecName, "%s", argv[1]);
	cout << "Reading " << vecName << ".vec" << endl;

	vector<vecIn_t> inputVec;
	
	Vtop *cktVar = new Vtop;
	rtLevelCkt *rtlCkt = new rtLevelCkt(cktVar); 

	sprintf(rtlCkt->cktName, "%s", vecName);
	rtlCkt->readVecFile(inputVec);
	
	state_pVec stateList (inputVec.size(), NULL);
	int_vec branch_counters(NUM_BRANCH, 0);
	int_vec branch_index;

	for (int idx = 0; (uint)idx < inputVec.size(); ++idx) {
		rtlCkt->resetCounters();
		rtlCkt->simOneVector(inputVec[idx]);
		stateList[idx] = new state_t(rtlCkt, idx);

		rtlCkt->getCoverage(branch_index);
		for (int_vec_iter it = branch_index.begin(); 
				it != branch_index.end(); ++it) {
			branch_counters[*it]++;
//			cout << *it << " ";
		}
//		cout << endl;
	}

	double newMetric = 0.0;
	newMetric = computeMetric(stateList, branch_counters);

	cout << "\nCoverage: " << newMetric << endl;

	for (uint idx = 0; idx < inputVec.size(); ++idx)
		if (stateList[idx]) {
			delete stateList[idx];
			stateList[idx] = NULL;
		}
	stateList.clear();
	delete rtlCkt;

	return 0;
}

//double computeMetric(state_pVec& stateList, rtLevelCkt* rtlCkt) {
//	
//	int_vec& startVec = rtlCkt->startIdxVec;
//	int_vec& sizeVec  = rtlCkt->sizeVec;
//	
//	typedef map<string, int> varMap_t;
//	typedef vector<varMap_t> varMapVec_t;
//	typedef varMapVec_t::iterator varMapVec_it;
//
//	varMapVec_t mapVec(NUM_VARS);
//	for (state_pVec_iter st = stateList.begin(); st != stateList.end(); ++st) {
//		
//		string stateStr = (*st)->getState();
//		for (int varIdx = 0; varIdx < NUM_VARS; ++varIdx) {	
//			string varStr = stateStr.substr(startVec[varIdx], sizeVec[varIdx]);
//			if (mapVec[varIdx].find(varStr) == mapVec[varIdx].end())
//				mapVec[varIdx].insert(make_pair(varStr, 1));
//			else
//				++(mapVec[varIdx][varStr]);
//		}
//	
//	}
//	
//	double newMetric = 0.0;
//	for (int varIdx = 0; varIdx < NUM_VARS; ++varIdx) {
//		cout << mapVec[varIdx].size() << " " << (1 << sizeVec[varIdx]) << endl;
//		newMetric += ((double) mapVec[varIdx].size()) / ((double) sizeVec[varIdx]);
//	}
//	cout << newMetric << endl;
//	return newMetric;
//}

double computeMetric(const state_pVec& stateList, const int_vec& branchHit) {

	typedef map<string, int> varMap_t;
	typedef vector<varMap_t> varMapVec_t;
	typedef varMapVec_t::iterator varMapVec_it;
	typedef vector<varMapVec_t> varMapVec2_t;

	int_vec startVec = int_vec(VAR_START_ARR, VAR_START_ARR + NUM_VARS);
	int_vec sizeVec =  int_vec(VAR_SIZE_ARR, VAR_SIZE_ARR + NUM_VARS);

	varMapVec_t mapVec(NUM_VARS);
	varMapVec2_t fullMapVec(NUM_BRANCH, mapVec);

//	#ifdef FULL_STATE_VARS
//	int stInd = 0;
//	int_vec& branchHit = paramObj->totalBranchHit;
//	#else
//	int stInd = paramObj->startIdx;
//	int_vec& branchHit = paramObj->branchHit;
//	#endif
	int stInd = 0;
	for (; stInd < stateList.size(); ++stInd) {
		state_t *st = stateList[stInd];
		string stateStr = st->getState();
		for (int varIdx = 0; varIdx < NUM_VARS; ++varIdx) {	
			string varStr = stateStr.substr(startVec[varIdx], sizeVec[varIdx]);
			for (int_vec_iter it = st->branch_index.begin(); 
					it != st->branch_index.end(); ++it) {
				if (fullMapVec[*it][varIdx].find(varStr) ==
						fullMapVec[*it][varIdx].end())
					fullMapVec[*it][varIdx].insert(make_pair(varStr, 1));
				else
					++(fullMapVec[*it][varIdx][varStr]);
			}
			if (mapVec[varIdx].find(varStr) == mapVec[varIdx].end())
				mapVec[varIdx].insert(make_pair(varStr, 1));
			else
				++(mapVec[varIdx][varStr]);
		}
	}
	
	cout << "Total:" << endl;
	for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx)
		cout << mapVec[vIdx].size() << " ";
	cout << endl;
	
	double newMetric = 0.0;
	double newMetric2 = 0.0;
	double newMetric3 = 0.0;
//	vector<double>& brMetric = paramObj->branchFactor;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if (branchHit[br]) {
			cout << "B: " << br << " " 
				// << branchHit[br] 
				 << endl;

			double val = 0.0;
			double val2 = 0.0;
			double val3 = 0.0;
			for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx) {
				val += ((double)fullMapVec[br][vIdx].size()) 
							/ mapVec[vIdx].size();
				val2 += fullMapVec[br][vIdx].size();
				val3 += fullMapVec[br][vIdx].size() / sizeVec[vIdx];
				cout << fullMapVec[br][vIdx].size() << " ";
			}
			cout << val << " " 
				// << val / branchHit[br] 
				 << endl;
//			brMetric[br] = val;
			newMetric += val;
			newMetric2 += val2;
			newMetric3 += val3;
		}
	}
	
	cout << "1: " << newMetric/NUM_VARS << endl
		 << "2: " << newMetric2/NUM_VARS << endl
		 << "3: " << newMetric3/NUM_VARS << endl;
		
	cout << "1: " << newMetric /NUM_STATE_BITS << endl
		 << "2: " << newMetric2/NUM_STATE_BITS << endl
		 << "3: " << newMetric3/NUM_STATE_BITS << endl;

	return newMetric/NUM_VARS;
}
