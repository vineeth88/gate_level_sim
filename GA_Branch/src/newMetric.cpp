#include "verInclude.h"
#include "rtLevelSim.h"
#include "incl.h"
#include "verilated.h"

using namespace std;

#ifdef MEM_ALLOC_DBG_ON
int state_t::mem_alloc_cnt = 0;
#endif

double computeMetric(state_pVec&, rtLevelCkt*);

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
	newMetric = computeMetric(stateList, rtlCkt);

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

double computeMetric(state_pVec& stateList, rtLevelCkt* rtlCkt) {
	
	int_vec& startVec = rtlCkt->startIdxVec;
	int_vec& sizeVec  = rtlCkt->sizeVec;
	
	typedef map<string, int> varMap_t;
	typedef vector<varMap_t> varMapVec_t;
	typedef varMapVec_t::iterator varMapVec_it;

	varMapVec_t mapVec(NUM_VARS);
	for (state_pVec_iter st = stateList.begin(); st != stateList.end(); ++st) {
		
		string stateStr = (*st)->getState();
		for (int varIdx = 0; varIdx < NUM_VARS; ++varIdx) {	
			string varStr = stateStr.substr(startVec[varIdx], sizeVec[varIdx]);
			if (mapVec[varIdx].find(varStr) == mapVec[varIdx].end())
				mapVec[varIdx].insert(make_pair(varStr, 1));
			else
				++(mapVec[varIdx][varStr]);
		}
	
	}
	
	double newMetric = 0.0;
	for (int varIdx = 0; varIdx < NUM_VARS; ++varIdx) {
		cout << mapVec[varIdx].size() << " " << (1 << sizeVec[varIdx]) << endl;
		newMetric += ((double) mapVec[varIdx].size()) / ((double) sizeVec[varIdx]);
	}
	cout << newMetric << endl;
	return newMetric;
}
