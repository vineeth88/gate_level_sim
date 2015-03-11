#include "verInclude.h"
#include "rtLevelSim.h"
#include "incl.h"
#include "verilated.h"
#include "graphLib.h"

using namespace std;

#ifdef MEM_ALLOC_DBG_ON
int state_t::mem_alloc_cnt = 0;
#endif

#define NEW_METRIC
#define ALL_BRANCH_METRIC

double computeMetric(state_pVec&, rtLevelCkt*);
double computeMetric(const state_pVec&, const int_vec&);
double computeMetricNew(const state_pVec&, const int_vec&);
double computeNormMetric(int_vec&);

#ifndef ALL_BRANCH_METRIC
int_vec IsBranchLeaf;
covGraph_t*	cktCovGraph;

int readLeafNodes(covGraph_t* graphCov) {

	int_vec leafNodes;
	int_vec topNodes;
	IsBranchLeaf = int_vec(NUM_BRANCH, 0);

	for (int i = 1; (unsigned) i <= graphCov->numNodes; ++i) {
		if ((graphCov->gNodes[i]).branch_index == -1)
			continue;

		int branch_ = (graphCov->gNodes[i]).branch_index;

		if(IsDefaultBranch(branch_))
			continue;

		if (graphCov->gNodes[i].num_child == 0) {
			IsBranchLeaf[branch_] = 1;
		}
	}
}
#endif

int main(int argc, char** argv) {
	
	if (argc < 2) {	
		cout << "Usage " << argv[0] << " <ckt> " << endl;
		exit(-1);
	}

	char vecName[256];
	sprintf(vecName, "%s", argv[1]);
	cout << "Reading " << vecName << ".vec" << endl;

	#ifndef ALL_BRANCH_METRIC
	// branch hierarchy tree (covGraph) file
	char cktDir[256];
	sprintf(cktDir, "lib/%s", benchCkt);
	covGraphObj = new covGraph_t;
	sprintf(covGraphObj->fName, "%s/%s.graph", cktDir, benchCkt);
	if(readGraph(*covGraphObj)) {
		delete covGraphObj;
		cout << "Error reading graph file" << endl;
		exit(-1);
	}
	readLeafNodes(covGraphObj);
	#endif

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

//	computeNormMetric(branch_counters);
	double newMetric = 0.0;
	newMetric = computeMetric(stateList, branch_counters);
//	newMetric = computeMetricNew(stateList, branch_counters);

	cout << "\nCoverage: " << newMetric << endl;

	for (uint idx = 0; idx < inputVec.size(); ++idx)
		if (stateList[idx]) {
			delete stateList[idx];
			stateList[idx] = NULL;
		}
	stateList.clear();

	delete rtlCkt;
	#ifndef ALL_BRANCH_METRIC
	delete covGraphObj;
	#endif

	return 0;
}

double computeMetricNew(const state_pVec& stateList, const int_vec& branchHit) {
	
//	int_vec& startVec = rtlCkt->startIdxVec;
//	int_vec& sizeVec  = rtlCkt->sizeVec;
	
	int_vec startVec = int_vec(VAR_START_ARR, VAR_START_ARR + NUM_VARS);
	int_vec sizeVec =  int_vec(VAR_SIZE_ARR, VAR_SIZE_ARR + NUM_VARS);

	typedef map<string, int> varMap_t;
	typedef vector<varMap_t> varMapVec_t;
	typedef varMapVec_t::iterator varMapVec_it;
	typedef vector<varMapVec_t> varMapVec2_t;

	varMapVec_t mapVec(NUM_VARS);
	varMapVec2_t fullMapVec(NUM_BRANCH, mapVec);

	uint stInd = 0;
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
	double negVal = 0.0;

	int num_branch = 0;
//	vector<double>& brMetric = paramObj->branchFactor;
	for (int br = 0; br < NUM_BRANCH; ++br) {

	#ifndef ALL_BRANCH_METRIC
		if (IsBranchLeaf[br] == 0)
			continue;
	#endif

		if (branchHit[br] > 0) {
			cout << "B: " << br << " " 
				 << endl;

			double val = 0.0;
			for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx) {
				val += ((double)fullMapVec[br][vIdx].size()) 
							/ mapVec[vIdx].size();
			//	val2 += fullMapVec[br][vIdx].size();
			//	val3 += fullMapVec[br][vIdx].size() / sizeVec[vIdx];
				cout << fullMapVec[br][vIdx].size() << " ";
			}
			cout << endl
				 << val << " " 
				 << endl;
			newMetric += val;
			++num_branch;
		}
		#ifdef NORM_METRIC
		else if (branchHit[br] == -1) {
			negVal += NUM_VARS;
		}
		#else
		else {
			negVal += NUM_VARS;
		}
		#endif
	}
	
	cout << "1: " << newMetric 
		 << " - " << negVal 
		 << " = " << newMetric - negVal << endl;
	return (newMetric - negVal);
}

double computeNormMetric(int_vec& branchHit) {

	int sum = 0;
	for (int_vec_iter it = branchHit.begin(); it != branchHit.end(); ++it) {
		sum += *it;
		cout << *it << " ";
	}
	
	cout << "\nAvg: " << sum / NUM_BRANCH << endl;
	sum /= NUM_BRANCH;

	for (int_vec_iter it = branchHit.begin(); it != branchHit.end(); ++it) {
		if(*it == 0)
			*it = -1;
		else if(*it > sum)
			*it = 0;
//		else if(*it < sum/100)
//			*it = 0;
	}

	return 0.0;
}

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
				#ifndef ALL_BRANCH_METRIC
				if (IsBranchLeaf[*it] == 0)
					continue;
				#endif

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

	int num_branch = 0;
//	vector<double>& brMetric = paramObj->branchFactor;
	for (int br = 0; br < NUM_BRANCH; ++br) {

	#ifndef ALL_BRANCH_METRIC
		if (IsBranchLeaf[br] == 0)
			continue;
	#endif
		if (branchHit[br]) {
			cout << "B: " << br << " " 
				 << branchHit[br] 
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
			cout << endl
				 << val << " " 
				 << val2 << " "
				 << val3 << " "
				// << val / branchHit[br] 
				 << endl;
//			brMetric[br] = val;
			newMetric += val;
			newMetric2 += val2;
			newMetric3 += val3;
			++num_branch;
		}

		else 
			newMetric -= NUM_STATE_BITS;
	}
	
	cout << "1: " << newMetric  << endl
		 << "2: " << newMetric2 << endl
		 << "3: " << newMetric3 << endl
		 << "4: " << (newMetric * num_branch)/NUM_BRANCH << endl;

	return newMetric2/NUM_BRANCH;
}
