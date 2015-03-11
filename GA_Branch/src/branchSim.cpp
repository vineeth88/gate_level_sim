#include "verInclude.h"
#include "rtLevelSim.h"
#include "incl.h"
#include "verilated.h"
#include "gaLib.h"
#include "graphLib.h"
#include <sys/time.h>
#include <sys/resource.h>

// Defines
#define RANDOM_SEED
#define GA_FIND_TOP_INDIV
//#define FITNESS_DBG_ON
#define DEBUG_PRINT_INDIV_OFF
//#define FULL_STATE_VARS
//#define B14_ROUND_DEBUG

#define PRINT_ROUND_OFF
#define ALL_BRANCH_METRIC
// ==================== Global Variables ============================
int FAULT_COVERAGE = 0;

int PARAMFILE_PRESENT = 0;
int EXCL_FILE_PRESENT = 0;
int COVGRAPH_PRESENT = 0;
int BRGRAPH_PRESENT = 0;
int CKT_WITH_MEMORY = 0;

int rnd_seed;

int_vec	exclBranchList;
int_vec IsBranchLeaf;
int_vec BranchNumTries;

#ifdef MEM_ALLOC_DBG_ON
int gaIndiv_t::mem_alloc_cnt = 0;
int state_t::mem_alloc_cnt = 0;
#endif

// ===================== Typedefs and Class definitions =====================
typedef pair<int, int> nodePair_t;
typedef map<nodePair_t, string> pathMap_t;

typedef map<string, vector<int> > stateIdxMap_t;
typedef stateIdxMap_t::iterator stateIdxMap_iter;

typedef map<string, int> varMap_t;
typedef varMap_t::iterator varMap_iter;
typedef vector<varMap_t> varMapVec_t;
typedef vector<varMapVec_t> varMapVec2_t;

typedef vector<double> dbl_vec;

class paramObj_t { 
	
	public:
		
		string 		paramName;

		// Stage 1 Parameters
		int			POP_SIZE_1, INDIV_LEN_1;
		int			NUM_GEN_1, MAX_ROUNDS_1;
		
		// Stage 2 Parameters
		int 		POP_SIZE_2, INDIV_LEN_2;
		int 		NUM_GEN_2, MAX_ROUNDS_2;
		int 		MAX_TRIES_2;

		vecIn_t		inputVec;
		state_pVec	stateList;
	stateIdxMap_t	stateTable;
		
		int 		startIdx;		// Start index of stage 2
		int_vec		resetIndices;

	varMapVec2_t 	varMapArr;
		int_vec		varWtArr;
		dbl_vec 	branchFactor;

		int_vec		branchHit;
		int_vec 	totalBranchHit;
		
		rtLevelCkt*	rtlCkt;

		// Stage 2 members
		int_vec		lastBranchHit;
		int_vec		branch_index;

		brGraph_t*	cktBrGraph;
		covGraph_t*	cktCovGraph;
		string		currPath;
	
		// Functions 
		paramObj_t() {

			NUM_GEN_1 = 4;
			POP_SIZE_1 = 256;
			INDIV_LEN_1 = 20;
			MAX_ROUNDS_1 = 10;

			NUM_GEN_2 = 4;
			POP_SIZE_2 = 256;
			INDIV_LEN_2 = 20;
			MAX_ROUNDS_2 = 100;
			MAX_TRIES_2 = 20;

			startIdx = 0;
			resetIndices = int_vec();

			varMapVec_t tmpVec(NUM_VARS);
			varMapArr = varMapVec2_t(NUM_BRANCH+1, tmpVec);
			varWtArr = int_vec(NUM_VARS, 0);
			branchFactor = dbl_vec(NUM_BRANCH, 0.0);

			branchHit = int_vec(NUM_BRANCH, 0);
			lastBranchHit = int_vec(NUM_BRANCH, 0);
			totalBranchHit = int_vec(NUM_BRANCH, 0);
			
			rtlCkt = NULL;
			cktBrGraph = NULL;
			cktCovGraph = NULL;
		}

		~paramObj_t() {
			if (rtlCkt) {
				delete rtlCkt; rtlCkt = NULL;
			}
			if (cktBrGraph) {
				delete cktBrGraph; cktBrGraph = NULL;
			}
			if (cktCovGraph) { 
				delete cktCovGraph; cktCovGraph = NULL;
			}
			for (state_pVec_iter st = stateList.begin();
					st != stateList.end(); ++st)
				if (*st) {
					delete *st;
					*st = NULL;
				}
		}
		void readParam();
};


// ===================== Function declarations ======================
double get_time();

void readExclBranchList(char[]);
int readBranchGraph(covGraph_t&, brGraph_t&);

void Stage1_GenerateVectors(paramObj_t*);
void Stage2_GenerateVectors(paramObj_t*);
void Stage2_Core(paramObj_t*, int);
string getPathBFS(brGraph_t*, int, int);

int computeBranches(int_vec&);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);
int compHammingDist(string, string);
void printCnt(int_vec&);
void printVec(int_vec&);
void printVectorSet(const vecIn_t&);

double computeMetric(paramObj_t*);
double computeMetricNew(paramObj_t*);

int main(int argc, char* argv[]) {

	double start_time = get_time();
	
	double total_stage1_time = 0.0;
	double total_stage2_time = 0.0;
	double total_metric_time = 0.0;
	double total_misc_time = 0.0;

	uint32_t seed;
	#ifdef RANDOM_SEED
	seed = time(NULL);
	cout << "Seed: " << seed << endl;
//	#if defined (__b10)
//		seed = 1424033984;
//	#endif
	srand(seed);
//	#else
//	#if defined (__b12)
//		seed = 1422414827;
//	#elif defined (__b10)
//		seed = 1422647276;
//	#else
//		seed = 1422647276;
//	#endif
	#endif

	rnd_seed = seed;

	char cktDir[256];
	sprintf(cktDir, "lib/%s", benchCkt);

	if (argc == 1) {
		cerr << "Executing GA for MAX BRANCH COVERAGE for " << benchCkt << endl
			 << "\tAdditional files will be read from " << cktDir << endl
			 << "\tType " << argv[0] << " -h for more options" << endl;
	}
	else if (argc >= 2) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'h') {
			cout << "Usage: " << argv[0] << " [-c0] [<path>]" << endl 
				 << "-c option selects FAULT COVERAGE(0) / BRANCH COVERAGE(1)" 
				 << endl
				 << "<path> is used to input location of additional files" << endl
				 << "\tDefault: " << cktDir << endl
				 << "Required files:\n"
				 << "\t" << benchCkt << ".param (Parameters)\n"
				 << "\t" << benchCkt << ".graph (Coverage Graph)\n"
				 << endl 
				 << "Optional files:\n" 
				 << "\t" << benchCkt << ".excl (Excluded Branches)\n"
				 << "\t" << benchCkt << ".bg (Branch Graph)\n"
				 << "\t" << benchCkt << ".mem (Memory details)\n" << endl;
			}
			else if (argv[1][1] == 'c') {
				if (argv[1][2] != '\0')
					FAULT_COVERAGE = argv[1][2] - '0';
				else
					FAULT_COVERAGE = 0;
					
			}
			else {
				cout << "\t" << "Unknown option!" << endl
			 		 << "\t" << "Type " << argv[0] << " -h for more options" << endl;
				exit(-1);
			}	
		}
		else {
			sprintf(cktDir, "%s", argv[1]);
			cerr << "\tDirectory changed to " << cktDir << endl;
		}

		if (argc == 3) {
			sprintf(cktDir, "%s", argv[2]);
			cerr << "\tDirectory changed to " << cktDir << endl;
		}
	}	


	if(FAULT_COVERAGE) 
		cout << "\nExecuting GA for MAX FAULT COVERAGE" << endl;
	else
		cout << "\nExecuting GA for MAX BRANCH COVERAGE" << endl;
	
	// param file
	paramObj_t* paramObj = new paramObj_t;
	char pName[256];
	sprintf(pName, "%s/%s.param", cktDir, benchCkt);

	paramObj->paramName = string(pName);
	paramObj->readParam();
	
	// excl branch file
	sprintf(pName, "%s/%s.excl", cktDir, benchCkt);
	readExclBranchList(pName);	
	
	// branch hierarchy tree (covGraph) file
	covGraph_t* covGraphObj = new covGraph_t;
	sprintf(covGraphObj->fName, "%s/%s.graph", cktDir, benchCkt);
	if(readGraph(*covGraphObj))
		COVGRAPH_PRESENT = 0;
	else
		COVGRAPH_PRESENT = 1;
	
	// branch transition graph (brGraph) file
	brGraph_t* brGraphObj = NULL;
	if (COVGRAPH_PRESENT) {
		brGraphObj = new brGraph_t;
		sprintf(brGraphObj->fName, "%s/%s.btg", cktDir, benchCkt);
		BRGRAPH_PRESENT = readBranchGraph(*covGraphObj, *brGraphObj);
	}
	else
		BRGRAPH_PRESENT = 0;

	cout << "PARAMFILE " << PARAMFILE_PRESENT << endl
		 << "EXCL_FILE " << EXCL_FILE_PRESENT << endl
		 << "COVGRAPH  " << COVGRAPH_PRESENT << endl
		 << "BRGRAPH   " << BRGRAPH_PRESENT << endl;
	
	if (COVGRAPH_PRESENT) 
		paramObj->cktCovGraph = covGraphObj;

	if (BRGRAPH_PRESENT && brGraphObj) 
		paramObj->cktBrGraph = brGraphObj;

	paramObj->totalBranchHit = int_vec(NUM_BRANCH, 0);
	paramObj->resetIndices = int_vec();
	
	double parse_time = get_time();
	total_misc_time += (parse_time - start_time);

	/* Start: Stage 1 */
	Stage1_GenerateVectors(paramObj);
	
	double stage1_time = get_time();
	total_stage1_time += (stage1_time - parse_time);
	/* Start: Stage 2 (If graph present) */
	if (BRGRAPH_PRESENT) 
		Stage2_GenerateVectors(paramObj); 
	
	double stage2_time = get_time();
	total_stage2_time += (stage2_time - stage1_time);

	/* Compute branch coverage after iteration */
	for (int br = 0; br < NUM_BRANCH; ++br)
		paramObj->totalBranchHit[br] += paramObj->branchHit[br];
	
	cout << "\nParse time: " << parse_time - start_time << endl
		 << "\nC0-S1 time: " << stage1_time - parse_time << endl
		 << "\nC0-S2 time: " << stage2_time - stage1_time << endl;

	int cnt = FAULT_COVERAGE;
	#if defined(__b14)
		if (FAULT_COVERAGE)
			cnt = 10;
		else
			cnt = 0;
	#endif
	double newMetric = 0.0;

	while(cnt) {
		start_time = get_time();

		newMetric = computeMetricNew(paramObj);
		cout << endl;
		newMetric = computeMetric(paramObj);
		cout << "\nnewMetric Coverage: " << newMetric << endl;
	
		/* Add all states from previous iteration into stateTable */
		for (int idx = 0; (uint)idx < paramObj->stateList.size(); ++idx) {
			string stateStr = paramObj->stateList[idx]->getHash();
			if (paramObj->stateTable.find(stateStr) == paramObj->stateTable.end()) {
				vector<int> tmpVec;
				tmpVec.push_back(idx);
				paramObj->stateTable.insert(make_pair(stateStr, tmpVec));
			}
			else
				paramObj->stateTable[stateStr].push_back(idx);
		}
		
		paramObj->startIdx = paramObj->stateList.size();
		paramObj->resetIndices.push_back(paramObj->startIdx);

		double metric_time = get_time();
		total_metric_time += (metric_time - start_time);

		Stage1_GenerateVectors(paramObj);
		stage1_time = get_time();
		total_stage1_time += (stage1_time - metric_time);

		if (BRGRAPH_PRESENT)
			Stage2_GenerateVectors(paramObj);
		stage2_time = get_time();
		total_stage2_time += (stage2_time - stage1_time);

		for (int br = 0; br < NUM_BRANCH; ++br)
			paramObj->totalBranchHit[br] += paramObj->branchHit[br];

		--cnt;
		cout << paramObj->inputVec.length() / NUM_INPUT_BITS << endl;
	}

	#ifdef MEM_ALLOC_DBG_ON
	cout << "gaIndiv_t:  " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t:    " << state_t::mem_alloc_cnt << endl;
	#endif
	
	double end_time = get_time();
	/* Print final branch coverage */
	cout << "\nFinal branch coverage: " << endl;
	printCnt(paramObj->totalBranchHit);
	
	/* Print vectors to file */
	printVectorSet(paramObj->inputVec);
	
	newMetric = computeMetricNew(paramObj);
	cout << endl;
	newMetric = computeMetric(paramObj);
	cout << "\nnewMetric Coverage: " << newMetric << endl;
	
	delete paramObj;

	#ifdef MEM_ALLOC_DBG_ON
	cout << "After deleting paramObj" << endl
		 << "gaIndiv_t:  " << gaIndiv_t::mem_alloc_cnt << endl
		 << "state_t:    " << state_t::mem_alloc_cnt << endl;
	#endif
	
	total_misc_time += ( get_time() - end_time);
	
	double total_exec_time = total_metric_time
							+ total_stage1_time
							+ total_stage2_time
							+ total_misc_time;

	cout << "\n Times:" << endl 
		 << "Stage1: " << total_stage1_time << endl  
		 << "Stage2: " << total_stage2_time << endl 
		 << "Metric: " << total_metric_time << endl 
		 << "Misc: " << total_misc_time << endl
		 << "Exec: " << total_exec_time << endl;

	return 0;
	
}

void paramObj_t :: readParam () {

	ifstream paramIn;
	paramIn.open(paramName.c_str(), ios::in);

	if (!paramIn) {
		cout << "Unable to read " << paramName << endl
			<< "Using default values. " << endl;
		PARAMFILE_PRESENT = 0;
	}	
	else {
		PARAMFILE_PRESENT = 1;
		while(paramIn) {
			string curr;
			getline(paramIn, curr);

			if (curr.find("NUM_GEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN_1 = " << val << endl;
					NUM_GEN_1 = val;
				}
			}
			else if (curr.find("POP_SIZE_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_1 = " << val << endl;
					POP_SIZE_1 = val;
				}
			}
			else if (curr.find("INDIV_LEN_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_1 = " << val << endl;
					INDIV_LEN_1 = val;
				}
			}
			else if (curr.find("MAX_ROUNDS_1") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_ROUNDS_1 = " << val << endl;
					MAX_ROUNDS_1 = val;
				}
			}
			else if (curr.find("NUM_GEN_2") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "NUM_GEN_2 = " << val << endl;
					NUM_GEN_2 = val;
				}
			}
			else if (curr.find("POP_SIZE_2") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "POP_SIZE_2 = " << val << endl;
					POP_SIZE_2 = val;
				}
			}
			else if (curr.find("INDIV_LEN_2") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "INDIV_LEN_2 = " << val << endl;
					INDIV_LEN_2 = val;
				}
			}
			else if (curr.find("MAX_ROUNDS_2") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_ROUNDS_2 = " << val << endl;
					MAX_ROUNDS_2 = val;
				}
			}
			else if (curr.find("MAX_TRIES_2") != string::npos) {
				size_t ind = curr.find("=");
				if (ind != string::npos) {
					stringstream ss;
					int val;
					ss << curr.substr(ind+1);
					ss >> val;
					cout << "MAX_TRIES_2 = " << val << endl;
					MAX_TRIES_2 = val;
				}
			}
		}
		cout << "Finished parsing " << paramName << endl;
		paramIn.close();
	}
}

void readExclBranchList (char exclName[]) {

	ifstream exclFile;
	exclFile.open(exclName, ios::in);
	exclBranchList = int_vec(NUM_BRANCH, 0);

	if(exclFile) {
		cout << "Reading excluded branch list from "
			<< exclName << endl;
		EXCL_FILE_PRESENT = 1;
	}
	else {
		cout << "Using default branch list from "
			<< benchCkt << "Int.h" << endl;
		for (int br = 0; br < NUM_BRANCH; ++br){ 
			if (IsDefaultBranch(br)) {
				exclBranchList[br] = -10;
				cout << br << " ";
			}
		}
		cout << endl;
		EXCL_FILE_PRESENT = 0;
		return;
	}

	while(exclFile) {	
		stringstream ss;
		string curr;
		getline(exclFile, curr);

		ss << curr;
		int br;
		ss >> br;
		while(ss) {
			cout << br << " ";
			exclBranchList[br] = -10;
			ss >> br;
		}
		cout << endl;
	}
	exclFile.close();

	return;
}

int readBranchGraph(covGraph_t& graphCov, brGraph_t& branchGraph) {

	int_vec leafNodes;
	int_vec topNodes;
	IsBranchLeaf = int_vec(NUM_BRANCH, 0);

	for (int i = 1; (unsigned) i <= graphCov.numNodes; ++i) {
		if ((graphCov.gNodes[i]).branch_index == -1)
			continue;

		int branch_ = (graphCov.gNodes[i]).branch_index;
		int parent_ = graphCov.getTopNode(branch_);

		if(IsDefaultBranch(branch_))
			continue;

		if (branch_ == parent_)
			topNodes.push_back(parent_);

		if (graphCov.gNodes[i].num_child == 0) {
			leafNodes.push_back(branch_);
			IsBranchLeaf[branch_] = 1;
		}
	}

//	cout << "Leaf nodes : "; 
//	printVec(leafNodes);
//	cout << endl;
//
//	cout << "Top nodes : "; 
//	printVec(topNodes);
//	cout << endl;

	for (int_vec_iter it = leafNodes.begin(); 
			it != leafNodes.end(); ++it)
		branchGraph.branchType[*it]++;

	for (int_vec_iter it = topNodes.begin(); 
			it != topNodes.end(); ++it)
		branchGraph.branchType[*it] += 2;

	ifstream brFile;
	brFile.open(branchGraph.fName, ios::in);
	if (brFile == NULL) {
		cout << "\nUnable to open file " << branchGraph.fName << endl;
		return false;
	}
	else	
		cout << "\nReading from file " << branchGraph.fName << endl;

	for (int lNum = 0; brFile && (uint) lNum < topNodes.size(); ++lNum) {
		string tmpStr;
		getline(brFile, tmpStr);

		stringstream ss;
		ss << tmpStr;

		int node_index;
		ss >> node_index;

		int num;
		ss >> num;

		int_vec edge_vec;
		while (ss) {
			edge_vec.push_back(num);

			int topNode = graphCov.getTopNode(num);

			branchGraph.addNode(node_index);
			branchGraph.addNode(topNode);
			branchGraph.addEdge(num, topNode, node_index);

			ss >> num;
		}

	}

	brFile.close();
//	branchGraph.printGraph();

	return true;
}

void Stage1_GenerateVectors(paramObj_t* paramObj) {

	/* Parameters */
	int NUM_GEN	= paramObj->NUM_GEN_1;
	int POP_SIZE = paramObj->POP_SIZE_1;

	int VEC_LEN	= paramObj->INDIV_LEN_1 / NUM_INPUT_BITS;
	int MAX_ROUNDS = paramObj->MAX_ROUNDS_1;
	int startIdx = paramObj->startIdx;
	stateIdxMap_t& stateTable = paramObj->stateTable;
	state_pVec& pStateList = paramObj->stateList;

//	int vec_offset = 0;

	double WT_FIT_STATE = 0.4;
	double WT_FIT_COV = 0.3;
	double WT_FIT_BRANCH = 0.7;

	int WT_COV = 500;
	int WT_NEW_BRANCH = 2000;
	int NEW_STATE_FIT = NUM_STATE_BITS;

	int lg_pop_size = 0;
	for (int pop_size = POP_SIZE; pop_size > 0;) {
		lg_pop_size++;
		pop_size >>= 1;
	}
	lg_pop_size--;

	cout << "\nGA Stage 1: " << endl
		<< POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;
	
	cout << lg_pop_size << endl;

	/* Initialize rtLevelCkt */
	Vtop *cktVar = new Vtop;
	rtLevelCkt *rtlCkt = new rtLevelCkt(cktVar);
	int_vec branch_counters(NUM_BRANCH, 0);
	
	#ifdef B14_ROUND_DEBUG
	Vtop *dbgCktVar = new Vtop;
	rtLevelCkt *dbgRtlCkt = new rtLevelCkt(dbgCktVar);
	int_vec dbg_branch_count(NUM_BRANCH, 0);
	#endif

	vecIn_t rstVec(NUM_INPUT_BITS, '0');
	rstVec[NUM_INPUT_BITS-1] = '1';
	#if defined(__b13)
	rstVec[NUM_INPUT_BITS-1] = '0';
	#endif

	string rstStateStr (NUM_STATE_BITS, '0');
	rtlCkt->setCktState(rstStateStr);
	rtlCkt->resetCounters();
	rtlCkt->simOneVector(rstVec);
	rtlCkt->getBranchCounters(branch_counters);

	state_t *rstState;
	rstState = new state_t(rtlCkt, 0);
	rstState->pIndiv = NULL;

	state_pVec startPool;
	startPool.push_back(rstState);

	#ifndef PRINT_ROUND_OFF
	cout << "Reset state: " << endl
		 << rstState->getState() << endl;
	#endif

	gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
	/*	Init population with one of the states in the pool 
		of start states and string of random vectors	*/
	stage0Pop.initPopulation(startPool);

//	stateMap_t glStateMap;
	stateSet_t currStateMap, glStateMap; 
	currStateMap.clear();
	int prevMaxCov = 0;

	gaIndiv_pVec topIndiv_vec;
	bool gaTerminate = false;

	//	int last_gen = 0;
	for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

		cout << "GEN " << gen << endl << endl;
		int_vec currBranchCov(NUM_BRANCH, 0);

		bool improv_cov = false; 
		bool new_state = false;

		int maxCov = 0, avgCov = 0;

		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];

			/* Reset masking for b12 */
			//#if defined(__b12)
			//	for (int x = 0; x < indiv->input_vec.length(); 
			//		x += NUM_INPUT_BITS) 
			//		indiv->input_vec[x] = '0';
			//#endif

			rtlCkt->resetCounters();
			indiv->simCkt(rtlCkt);

			/* Add states to stateMap	*/
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st) {
				state_t *curr = *st;
				keyVal_t hash_val_ = curr->getHash();
				retVal_t ret = currStateMap.insert(hash_val_);

				if (ret.second)
					new_state = true;
			}

			indiv->num_branch = computeBranches(indiv->branch_cov);
			rtlCkt->resetCounters();

			/* Computing max and avg branch coverage */
			if (maxCov < indiv->num_branch)
				maxCov = indiv->num_branch;

			avgCov += indiv->num_branch;

		}
	
//		avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
		avgCov = avgCov >> lg_pop_size;
		improv_cov = (maxCov >= prevMaxCov);

//		cout << "Avg Coverage: " << avgCov << endl
//			<< "Max Coverage: " << maxCov << endl;

		/* Compute Fitness	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

			/* Fitness for #branches covered */
			fitness_t fitness_cov = (avgCov - indiv->num_branch) * WT_COV;

			/* Fitness for branch coverage	*/
			fitness_t fitness_branch = 0;
			if (FAULT_COVERAGE && (startIdx > 0)) {
				for (int br = 0; br < NUM_BRANCH; ++br) {
					if (indiv->branch_cov[br] && (paramObj->totalBranchHit[br] == 0) 
							&& (exclBranchList[br] == 0)) {
						#ifndef PRINT_ROUND_OFF
						cout << ind << " reached new branch " << br << endl;
						#endif
						fitness_branch -= WT_NEW_BRANCH;
					}
				}
			}

			/* Fitness for states reached 	*/
			fitness_t fitness_state = 0;
			if (FAULT_COVERAGE && (startIdx > 0)) {
			#ifndef THREE_STATE_HAMMING_DIST
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st) {
					keyVal_t hash = (*st)->getHash();
					int avg_hamming_dist = 0;
					string currStr = (*st)->getState();
					if (stateTable.find(hash) != stateTable.end()) {
						vector<int>& indexRef = stateTable[hash];
						for (int_vec_iter vt = indexRef.begin(); 
								vt != indexRef.end(); ++vt) {;
							string prevStr = pStateList[*vt]->getState();
							/* Compute hamming distance */
							avg_hamming_dist += compHammingDist(currStr, prevStr);
						}
						if (indexRef.size())
							avg_hamming_dist /= indexRef.size();
						fitness_state -= avg_hamming_dist;
					}
					else 
						fitness_state -= NEW_STATE_FIT;
				}
			#endif

			#ifdef THREE_STATE_HAMMING_DIST
			// TODO
			#endif
			}

			/* Combining all fitness values	*/
			#ifdef FITNESS_DBG_ON
			cout << "[" << ind << "]\t" 
				 << (fitness_cov * WT_FIT_COV) << " "
				 << (fitness_branch * WT_FIT_BRANCH) << " "
				 << (fitness_state * WT_FIT_STATE) << endl;
			#endif
			indiv->fitness 
				= (fitness_cov * WT_FIT_COV) 
				+ (fitness_branch * WT_FIT_BRANCH)
				+ (fitness_state * WT_FIT_STATE);
		}	

		/* Terminate GA Condition:
		   - gen == NUM_GEN
		   - coverage[gen] <= coverage[gen-1]
		   - no new states reached	*/

		gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

		if (!gaTerminate) {

			#ifndef GA_FIND_TOP_INDIV
			std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
			
			#ifndef PRINT_ROUND_OFF
			cout << "Fitness: " << (stage0Pop.indiv_vec[0])->fitness << "\t | " << (stage0Pop.indiv_vec[0])->index << endl;
			#endif
			gaIndiv_t *tmp = new gaIndiv_t(*(stage0Pop.indiv_vec[0]));

			#else
			gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

			for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
					gt != stage0Pop.indiv_vec.end(); ++gt) {
//				if (compCoverage(*gt, indiv))
				if (compFitness(*gt, indiv))
					indiv = *gt;
			}
			#ifndef PRINT_ROUND_OFF
			cout << "Fitness: " << indiv->fitness << "\t | " << indiv->index << endl;
			#endif
			gaIndiv_t *tmp = new gaIndiv_t(*indiv);
			#endif

			topIndiv_vec.push_back(tmp);

			stage0Pop.gaEvolve();

			prevMaxCov = maxCov;
		}
		#ifndef PRINT_ROUND_OFF
		else
			cout << "Terminating after gen " << gen << endl;
		#endif

	}
	
	#ifdef MEM_ALLOC_DBG_ON
	cout << "States allocated: " << state_t::mem_alloc_cnt << endl;
	#endif

	for (gaIndiv_pVec_iter gt = topIndiv_vec.begin();
			gt != topIndiv_vec.end(); ++gt) {
		gaIndiv_t *tmp = *gt;
		tmp->state_list = state_pVec(tmp->vec_length, NULL);
		tmp->simCkt(rtlCkt);
	}

	/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
	#ifndef GA_FIND_TOP_INDIV
	std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);

	topIndiv_vec.push_back(stage0Pop.indiv_vec[0]);
	stage0Pop.indiv_vec[0] = NULL;
	#else
	gaIndiv_t *tmp = stage0Pop.indiv_vec[0];
	#ifndef PRINT_ROUND_OFF
	cout << "Fitness: " << tmp->fitness << "\t | " << tmp->index << endl;
	#endif
	int max_idx = 0, idx = 0;
	for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
			gt != stage0Pop.indiv_vec.end(); ++gt, ++idx) {
		if (compCoverage(*gt, tmp)) {
			tmp = *gt;
			max_idx = idx;
		}
	}

	topIndiv_vec.push_back(stage0Pop.indiv_vec[max_idx]);
	stage0Pop.indiv_vec[max_idx] = NULL;
	#endif

	/* Sorting to calculate the best indiv among the N generations */
	std::sort(topIndiv_vec.begin(), topIndiv_vec.end(), compFitness);
	gaIndiv_t *indiv = topIndiv_vec[0];

	gaIndiv_pVec_iter gt = topIndiv_vec.begin(); 
	gt++;
	for (;gt != topIndiv_vec.end(); ++gt) {
		delete *gt;
		*gt = NULL;
	}

	topIndiv_vec.clear();

	/* ***** ROUND 1:MAX_ROUNDS	***** */

	/*	Need to add the final state to statePool only if its unique	
		If not unique, the indiv with greater coverage should be added
		If all coverages are same, the indiv with lesser max_index should be added
	 */

	vecIn_t startVec;
	startPool.clear();

	rtlCkt->resetCounters();
	rtlCkt->setCktState(rstState);
	indiv->simCkt(rtlCkt);

	//	indiv->printIndiv(1);
//	#if defined(__b14)
//	if (indiv->max_index % 2 == 0)
//		indiv->max_index--;
//	#endif

	for (int st = 0; st <= indiv->max_index; ++st) {	
		state_t *curr = indiv->state_list[st];
		keyVal_t hash_val_ = curr->getHash();
		glStateMap.insert(hash_val_);

		for (int_vec_iter bt = curr->branch_index.begin();
				bt != curr->branch_index.end(); ++bt)
			branch_counters[*bt]++;

	}
	state_t* st = indiv->state_list[indiv->max_index];
	startVec = rstVec + indiv->input_vec.substr(0,NUM_INPUT_BITS*(indiv->max_index+1));
	startPool.push_back(st);
	#ifdef B14_ROUND_DEBUG
	cout << "Last branch indices : "<< endl;
	printVec(st->branch_index);
	cout << endl;
	#endif

//	vec_offset += 1 + indiv->max_index + 1;
//	cout << "Size: " << startVec.length() << " Off: " << vec_offset << endl;
//	assert(startVec.size() == (vec_offset * NUM_INPUT_BITS));

	cout << "After Round 0: " << endl
//		 << st->getState()
		 << " --------------- " << endl;

	#ifndef PRINT_ROUND_OFF
	printCnt(branch_counters);
	cout << " --------------- " << endl;
	#endif

//	#if defined(__b14)
//	rtlCkt->resetCounters();
//	rtlCkt->setCktState(rstStateStr);
//	rtlCkt->simMultiVector(startVec);
//	rtlCkt->getBranchCounters(branch_counters);
//	#endif

	#ifdef B14_ROUND_DEBUG
	dbgRtlCkt->resetCounters();
	dbgRtlCkt->setCktState(rstStateStr);
	dbgRtlCkt->simMultiVector(startVec);
	dbgRtlCkt->getBranchCounters(dbg_branch_count);
	
	cout << "Debug Counters: " << endl;
	printCnt(dbg_branch_count);
	cout << " --------------- " << endl;

	for(int dbg_br = 0; dbg_br < NUM_BRANCH; ++dbg_br) {
		if (dbg_branch_count[dbg_br] != branch_counters[dbg_br])
			//exit(-1);
			assert(0);
	}
	dbg_branch_count.clear();

	cout << "End State: 0" << endl
		 << dbgRtlCkt->getCktState() << endl
		 << endl;
	
	printVectorSet(startVec);
	#endif

	bool Rnd_terminate = false;

	bool all_done = true;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if (!exclBranchList[br] && (branch_counters[br] == 0))
			all_done = false;
	}
	
	for (int round = 1; !all_done && round < MAX_ROUNDS && !Rnd_terminate; ++round) {

		gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
		/*	Init population with one of the states in the pool 
			of start states and string of random vectors	*/
		stage0Pop.initPopulation(startPool);

		#ifdef B14_ROUND_DEBUG
		cout << "Start state: " << round << endl
			 << startPool[0]->getState() << endl
			 << endl;
		#endif
	
//		#if defined(__b14)
//		int_vec dupl_branch_cnt(NUM_BRANCH, 0);
//		for(int_vec_iter dupl = startPool[0]->branch_index.begin();
//				dupl != startPool[0]->branch_index.end(); ++dupl) 
//			dupl_branch_cnt[*dupl]++;
//		#endif

		stateSet_t currStateMap;
		//stateMap_t currStateMap = glStateMap;
		int prevMaxCov = 0;

		bool gaTerminate = false;
		for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

			#ifndef PRINT_ROUND_OFF
			cout << "GEN " << gen << endl << endl;
			#endif

			bool improv_cov = false; 
			bool new_state = false;

			int maxCov = 0, avgCov = 0;

			for (int ind = 0; ind < POP_SIZE; ++ind) {

				rtlCkt->resetCounters();
				gaIndiv_t* indiv = stage0Pop.indiv_vec[ind];
				indiv->simCkt(rtlCkt);

//				indiv->printIndiv(0);

				/* Add states to stateMap	*/
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st) {
					state_t *curr = *st;
					keyVal_t hash_val_ = curr->getHash();
					retVal_t ret = currStateMap.insert(hash_val_);

					if (ret.second)
						new_state = true;
				}

				indiv->num_branch = computeBranches(indiv->branch_cov);

				/* Computing max and avg branch coverage */
				if (maxCov < indiv->num_branch)
					maxCov = indiv->num_branch;

				avgCov += indiv->num_branch;

			}

//			cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
//				<< "Max Coverage: " << maxCov << endl;
//
//			avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
			avgCov = avgCov >> lg_pop_size;
			improv_cov = (maxCov >= prevMaxCov);

			/* Compute Fitness	*/
			for (int ind = 0; ind < POP_SIZE; ++ind) {

				gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

				/* Fitness for #branches covered */
				fitness_t fitness_cov = (avgCov - indiv->num_branch) * WT_COV;

				/* Fitness for branch coverage	*/
				fitness_t fitness_branch = 0;
				for (int br = 0; br < NUM_BRANCH; ++br) {
					if (indiv->branch_cov[br] && (branch_counters[br] == 0) 
							&& (exclBranchList[br] == 0))
						fitness_branch -= WT_NEW_BRANCH;
				}

				/* Fitness for states reached 	*/
				fitness_t fitness_state = 0;
				if (FAULT_COVERAGE && (startIdx > 0)) {
					for (state_pVec_iter st = indiv->state_list.begin(); 
							st != indiv->state_list.end(); ++st) {
						keyVal_t hash = (*st)->getHash();
						int avg_hamming_dist = 0;
						string currStr = (*st)->getState();
						if (stateTable.find(hash) != stateTable.end()) {
							vector<int>& indexRef = stateTable[hash];
							for (int_vec_iter vt = indexRef.begin(); 
									vt != indexRef.end(); ++vt) {
								string prevStr = pStateList[*vt]->getState();
								/* Compute hamming distance */
								avg_hamming_dist += compHammingDist(currStr, prevStr);
							}
							if (indexRef.size())
								avg_hamming_dist /= indexRef.size();
							fitness_state -= avg_hamming_dist;
						}
						else 
							fitness_state -= NEW_STATE_FIT;
					}
				}

				/* Combining all fitness values	*/
				#ifdef FITNESS_DBG_ON
				cout << "[" << ind << "]\t" 
					 << (fitness_cov * WT_FIT_COV) << " "
					 << (fitness_branch * WT_FIT_BRANCH) << " "
					 << (fitness_state * WT_FIT_STATE) << endl;
				#endif
				indiv->fitness 
					= (fitness_cov * WT_FIT_COV) 
					+ (fitness_branch * WT_FIT_BRANCH)
					+ (fitness_state * WT_FIT_STATE);
			}	


			/* Terminate GA Condition:
			   - gen == NUM_GEN
			   - coverage[gen] <= coverage[gen-1]
			   - no new states reached	*/

			gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

			if (!gaTerminate) {
				stage0Pop.gaEvolve();
				prevMaxCov = maxCov;
			}
			else {
				#ifndef PRINT_ROUND_OFF
				cout << "Terminating after gen " << gen << endl;
				#endif
				if (prevMaxCov < maxCov)
					prevMaxCov = maxCov;
			}

		}

		#ifndef GA_FIND_TOP_INDIV
		/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
		std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compFitness);
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
		#else
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
		for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
				gt != stage0Pop.indiv_vec.end(); ++gt) {
			if (compFitness(*gt, indiv))
				indiv = *gt;
		}
		#endif

		#ifndef DEBUG_PRINT_INDIV_OFF
		cout << "Fittest indiv after round " << round << endl;
		indiv->printIndiv(1);
		#endif

		int_vec curr_branch_cnt (NUM_BRANCH, 0);
		bool new_start_state = false;
//		#if defined(__b14)
//		if (indiv->max_index % 2)
//			indiv->max_index--;
//		#endif
		for (int st = 0; st <= indiv->max_index; ++st) {	
			state_t *curr = indiv->state_list[st];
			keyVal_t hash_val_ = curr->getHash();
			retVal_t ret = glStateMap.insert(hash_val_);

			#ifdef DEBUG_PRINT_INDIV_OFF
			if (ret.second)
				new_start_state = true;
			#else
			if (ret.second) {
				cout << "X";
				new_start_state = true;
			}
			else 
				cout << "-";
			#endif

			for (int_vec_iter bt = curr->branch_index.begin();
					bt != curr->branch_index.end(); ++bt)
				curr_branch_cnt[*bt]++;

		}

		cout << endl;

		bool new_branch_ = false;
		for (int br = 0; br < NUM_BRANCH; ++br) {
			if ((curr_branch_cnt[br]) && (branch_counters[br] == 0))
				new_branch_ = true;
			branch_counters[br] += curr_branch_cnt[br];
		}

		bool all_done_2 = true;
		for (int br = 0; br < NUM_BRANCH; ++br) {
			if (!exclBranchList[br] && (branch_counters[br] == 0)) {
				all_done_2 = false;
				break;
			}
		}

		Rnd_terminate = !(new_start_state || new_branch_) || (round == MAX_ROUNDS-1) || (all_done_2);

		startVec += indiv->input_vec.substr(0,NUM_INPUT_BITS*(indiv->max_index+1));
		
//		vec_offset += indiv->max_index + 1;
//		cout << "Size: " << startVec.size() << " Off: " << vec_offset << endl;
//		assert(startVec.size() == (NUM_INPUT_BITS*vec_offset));

		for (state_pVec_iter st = startPool.begin(); st != startPool.end(); ++st)
			if (*st != NULL) {
				delete *st;
				*st = NULL;
			}

		startPool.clear();
		state_t* st = indiv->state_list[indiv->max_index];
		indiv->state_list[indiv->max_index] = NULL;

		#ifdef B14_ROUND_DEBUG
		cout << "Last branch indices : "<< endl;
		printVec(st->branch_index);
		cout << endl;
		#endif

		startPool.push_back(st);

		cout << "After Round " << round << " : " << endl
		//	 << st->getState()
			 << " --------------- " << endl;

		#if defined(__b14)
		rtlCkt->resetCounters();
		rtlCkt->setCktState(rstStateStr);
		rtlCkt->simMultiVector(startVec);
		rtlCkt->getBranchCounters(branch_counters);
		#endif

		#ifndef PRINT_ROUND_OFF
		printCnt(branch_counters);
		cout << " --------------- " << endl;
		#endif

		#ifdef B14_ROUND_DEBUG
		dbgRtlCkt->resetCounters();
		dbgRtlCkt->setCktState(rstStateStr);
		dbgRtlCkt->simMultiVector(startVec);
		int_vec dbg_branch_count;
		dbgRtlCkt->getBranchCounters(dbg_branch_count);
		
		cout << "Debug Counters: " << endl;
		printCnt(dbg_branch_count);
		cout << " -------------- " << endl;

		for(int dbg_br = 0; dbg_br < NUM_BRANCH; ++dbg_br) {
			if (dbg_branch_count[dbg_br] != branch_counters[dbg_br]) {
					printVectorSet(startVec);
					assert(0);
				}
		}
		dbg_branch_count.clear();

		cout << "End State: " << round << endl
			 << dbgRtlCkt->getCktState() << endl
			 << endl;
		#endif

		if (Rnd_terminate)
			cout << "Exiting after round " << round << endl;
	}		

	rtlCkt->setCktState(rstStateStr);
	rtlCkt->simMultiVector(startVec);
	int_vec branch_cov;
	rtlCkt->getBranchCounters(branch_cov);

	cout << "Final coverage: " << endl;
	printCnt(branch_cov);
	cout << "\n#Pre: " << paramObj->inputVec.length() / NUM_INPUT_BITS << endl;
	cout << "\n#Vec: " << startVec.length() / NUM_INPUT_BITS << endl;

	gaIndiv_t *indiv_ = new gaIndiv_t(0, 
			startVec.length()/NUM_INPUT_BITS,
			rstState);
	indiv_->input_vec = startVec;
	indiv_->simCkt(rtlCkt);
	
	paramObj->branchHit = branch_cov;
	paramObj->branch_index = (indiv_->state_list.back())->branch_index;
	if (FAULT_COVERAGE && (startIdx > 0)) {
		paramObj->inputVec += startVec;
		size_t vecSize = paramObj->stateList.size() 
							+ indiv_->state_list.size();
		paramObj->stateList.reserve(vecSize);
		paramObj->stateList.insert(paramObj->stateList.end(), 
									indiv_->state_list.begin(), 
									indiv_->state_list.end());
	}
	else {
		paramObj->stateList = indiv_->state_list;
		paramObj->inputVec = startVec;
	}
	indiv_->state_list = state_pVec(indiv_->vec_length, NULL);

	cout << "\n#Post: " << paramObj->inputVec.length() / NUM_INPUT_BITS << endl;

//	cout << paramObj->stateList.size() << endl
//		 << startIdx << endl
//		 << vec_offset << endl;
//	assert (paramObj->stateList.size() == (startIdx + vec_offset));

	delete indiv_;
	delete rstState;
	delete rtlCkt;
}

void Stage2_GenerateVectors(paramObj_t* paramObj) {
	
	int POP_SIZE = paramObj->POP_SIZE_2;
	int MAX_TRIES = paramObj->MAX_TRIES_2;

	int_vec &branch_index = paramObj->branch_index;
	int_vec &branchHit = paramObj->branchHit;
	brGraph_t* branchGraph = paramObj->cktBrGraph;
	
	Vtop* cktVar = new Vtop;
	rtLevelCkt* rtlCkt = new rtLevelCkt(cktVar);
	paramObj->rtlCkt = rtlCkt;

	/* computing branch_index */
	int_vec unCovered;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if((exclBranchList[br] == 0) && 
				(branchHit[br] == 0) && 
				(IsBranchLeaf[br])) {
			bEdge_t* edge = branchGraph->getEdge(br);
			if (edge) 
				unCovered.push_back(br);
		}
	}

	int MAX_ROUNDS = paramObj->MAX_ROUNDS_2;
	BranchNumTries = int_vec(NUM_BRANCH, 0);
	pathMap_t pathDB;
	
	int exit_status = 1;
	int prev_target_node = 0;
	for (int num_round = 0; num_round < MAX_ROUNDS; ++num_round) {

		cout //<< endl 
			<< "\nROUND : " << num_round << " / " << MAX_ROUNDS
			//<< endl 
			<< endl;

		if (unCovered.size() == 0) {
			exit_status = 0;
			break;
		}

		/*	edgeVec = The list of edges traversed in the last cycle	*/
		int_vec edgeVec;
		for (int_vec_iter br = branch_index.begin(); br != branch_index.end(); ++br) {
			if (IsBranchLeaf[*br])	{
				bEdge_t* curr_edge = branchGraph->getEdge(*br);
				if (curr_edge) 
					edgeVec.push_back(*br);
			}
		}

		/* 	curr_val = Selected edge with endpoints start_val -> end_val */
		if (edgeVec.size() == 0)
			return;
		int curr_val = edgeVec.back();
		bEdge_t* curr_edge = branchGraph->getEdge(curr_val);
		int end_val = curr_edge->endTop;

		bNode_t* curr_node = branchGraph->getNode(end_val);
		assert(curr_node);

		string target_path = "Unreachable";
		int target_lvl = (2 << 20);
		int target_node = -1;
//		int target_tries = paramObj->MAX_TRIES;

		/*	- Find path from curr_node to all the start_nodes of the uncovered edges
			- Find the uncovered edge with the shortest path
			- Set the topNode(start_node) as the target_node */

		for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
			string path;

			nodePair_t curr = make_pair(end_val, *it);

			if (pathDB.find(curr) == pathDB.end()) {
				path = getPathBFS(branchGraph, end_val, *it);
				pathDB.insert(make_pair(curr, path));
			}
			else {
				path = pathDB[curr];
			}

			int lvl = path.length();

			#ifdef S2_DBG_PRINT
			cout << end_val << "-> " 
				<< *it << ": " 
				<< path << endl;
			#endif

			if ((target_lvl > lvl) && (path.compare("Unreachable"))) {
				target_lvl = lvl;
				target_node = *it;
				target_path = path;
			}
		}

		cout << "Target	: " << target_node //<< endl
			<< "\tLevel : " << target_lvl //<< endl
			<< "\tPath 	: " << target_path << endl;

		if (target_path.compare("Unreachable")) {
			if (BranchNumTries[target_node] >= MAX_TRIES) {
				exit_status = 1;
				break;
			}
			
			//BranchNumTries[target_node]++;
			int iter_val = end_val;

			bNode_t* iter_node = branchGraph->getNode(iter_val);
			assert(iter_node);

			paramObj->currPath = target_path;
			Stage2_Core(paramObj, iter_val);

			cout << "END OF PATH\n";

			for (int br = 0; (uint)br < unCovered.size(); ++br) {
				if (unCovered[br] == target_node)
					unCovered[br] = -100;
				else if (branchHit[unCovered[br]])
					unCovered[br] = -100;
			}

			int_vec newVec;
			for (int_vec_iter br = unCovered.begin(); br != unCovered.end(); 
					++br) {
				if ((*br != -100) && (BranchNumTries[*br] < MAX_TRIES))
					newVec.push_back(*br);
			}
			if ((branchHit[target_node] == 0) && 
					(BranchNumTries[target_node] < MAX_TRIES))
				newVec.push_back(target_node);

			unCovered.clear();
			unCovered = newVec;
			newVec.clear();

			#ifndef PRINT_ROUND_OFF
			cout << "unCovered: " << endl;
			printVec(unCovered);
			#endif

			prev_target_node = target_node;
		}
		else {
			cout << "#Tries : " << endl;
			printCnt(BranchNumTries);
			if (prev_target_node > 0) {
				cout << prev_target_node << "++" << endl;
				BranchNumTries[prev_target_node]++;
				prev_target_node = 0;
			}

//			for (int_vec_iter br = unCovered.begin(); br != unCovered.end();
//					++br) {
//				BranchNumTries[*br]++;
//			}

			cout << endl
			#ifndef PRINT_ROUND_OFF
				<< " * * * * * * * * * * * * * " << endl
			#endif
				<< "No reachable path. Simulating vector for next state." << endl;

			stateMap_t nxtStateMap = stateMap_t();
			map<keyVal_t, vecIn_t> vecMap;
			gaPopulation_t nxtStatePop(POP_SIZE, 1);

			state_pVec initPool;
			initPool.push_back(paramObj->stateList.back());

			#ifndef PRINT_ROUND_OFF
			cout << "Start state: " << endl
				 << initPool[0]->getState()
				 << endl;
			#endif

			nxtStatePop.initPopulation(initPool);

			rtLevelCkt *rtlCkt = paramObj->rtlCkt;
			for (int ind = 0; ind < POP_SIZE; ++ind) {

				rtlCkt->resetCounters();
				gaIndiv_t* indiv = nxtStatePop.indiv_vec[ind];
				indiv->simCkt(rtlCkt);
				//indiv->simCkt(cktVar);

				int vt = 0;
				for (state_pVec_iter st = indiv->state_list.begin(); 
						st != indiv->state_list.end(); ++st, ++vt) {
					keyVal_t hash = (*st)->getHash();
					retMap_t ret = nxtStateMap.insert(make_pair(hash, *st));
					if (ret.second == false) {
						(ret.first->second)->hit_count++;
					}
					vecMap.insert(make_pair(hash, indiv->input_vec)); 
				}
			}

			vector<vecIn_t> nxtVec;
			vector<int> nxtPaths;
			for (stateMap_iter st = nxtStateMap.begin(); 
					st != nxtStateMap.end(); ++st) {

				int_vec edgeVec;
				edgeVec.clear();
				state_t *curr = st->second;
				for (int_vec_iter br = curr->branch_index.begin(); 
						br != curr->branch_index.end(); ++br) {
					if (IsBranchLeaf[*br])	{
						bEdge_t* curr_edge = branchGraph->getEdge(*br);
						if (curr_edge) 
							edgeVec.push_back(*br);
					}
				}

				int curr_val = edgeVec.back();
				bEdge_t* curr_edge = branchGraph->getEdge(curr_val);
//				int start_val = curr_edge->startTop;
				int end_val = curr_edge->endTop;

				bNode_t* curr_node = branchGraph->getNode(end_val);
				assert(curr_node);

				string target_path = "Unreachable";
				int target_lvl = (2 << 20);
//				int target_node = -1;

				/*	- Find path from curr_node to all the start_nodes of the uncovered edges
					- Find the uncovered edge with the shortest path
					- Set the topNode(start_node) as the target_node */
				int num_paths = 0;
				for (int_vec_iter it = unCovered.begin(); it != unCovered.end(); ++it) {
					string path;
					nodePair_t curr_pair = make_pair(end_val, *it);
					if (pathDB.find(curr_pair) == pathDB.end()) {
						path = getPathBFS(branchGraph, end_val, *it);
						pathDB.insert(make_pair(curr_pair, path));
					}
					else {
						path = pathDB[curr_pair];
					}

					int lvl = path.length();

					#ifndef PRINT_ROUND_OFF
					cout << *it << ": " << path << endl;
					#endif
					if (path.compare("Unreachable")) {
						num_paths++;
						if (target_lvl > lvl) {
							target_lvl = lvl;
							target_node = *it;
							target_path = path;
						}
					}
				}

				if (target_path.compare("Unreachable")) {
					nxtVec.push_back(vecMap[st->first]);
					nxtPaths.push_back(num_paths);
				}
			}

			int max_num_paths = -1, max_path_index = -1;
//			assert(nxtPaths.size());
			if (nxtPaths.size() == 0) {
				//printVectorSet(paramObj->inputVec, true);
				//getDominator(covGraph, unCovered);
				exit_status = -1;
				break;
				/* Might need to backtrack or just exit in this case */
			}

			for (uint np = 0; np < nxtPaths.size(); ++np) {
				if (nxtPaths[np] > max_num_paths) {
					max_num_paths = nxtPaths[np];
					max_path_index = np;
				}
			}
			if (max_path_index < 0) {
				exit_status = -1;
				break;
			}
			cout << endl;

			rtlCkt->setCktState(paramObj->stateList.back());
			rtlCkt->simOneVector(nxtVec[max_path_index]);
			state_t *tmpState = new state_t(rtlCkt, paramObj->stateList.size());
			(paramObj->stateList).push_back(tmpState);
			paramObj->inputVec += nxtVec[max_path_index];

			cout << "Adding vector: " << nxtVec[max_path_index] << endl
			#ifndef PRINT_ROUND_OFF
				<< "State: \n" << tmpState->getState()
			#endif
				<< endl; 

			paramObj->lastBranchHit = int_vec(NUM_BRANCH, 0);
			for (int_vec_iter it = tmpState->branch_index.begin();
					it != tmpState->branch_index.end(); ++it)
				paramObj->lastBranchHit[*it]++;

			paramObj->branch_index = tmpState->branch_index;
			for (int ind = 0; ind < NUM_BRANCH; ++ind)
				paramObj->branchHit[ind] += paramObj->lastBranchHit[ind];
			#ifndef PRINT_ROUND_OFF
			printCnt(paramObj->lastBranchHit);
			printCnt(paramObj->branchHit);
			#endif

			cout << " * * * * * * * * * * * * * * * * * * * * " << endl << endl;
		}
	}

	if (exit_status == 0) 
		cout << "All branches reached!!" << endl;
	else if (exit_status == 1)
		cout << "Maximum Try Limit reached. Terminating Stage 2" << endl;
	else
		cout << "Assertion: No paths found. Exiting!!" << endl;

#ifdef S2_DBG_PRINT
	int i = 0;
	for (state_pVec_iter st = paramObj->stateList.begin();
			st != paramObj->stateList.end(); ++st, ++i) {
		cout << paramObj->inputVec.substr(i*NUM_INPUT_BITS, NUM_INPUT_BITS);
		cout << endl << (*st)->getState()
			 << endl << endl;
	}
#endif

	return;
}

void Stage2_Core(paramObj_t* paramObj, int start_node) {

	int NUM_GEN	= paramObj->NUM_GEN_2;
	int POP_SIZE = paramObj->POP_SIZE_2;
	int VEC_LEN = paramObj->INDIV_LEN_2/NUM_INPUT_BITS;
	
	brGraph_t* branchGraph = paramObj->cktBrGraph;
	rtLevelCkt* rtlCkt = paramObj->rtlCkt;
	int_vec &branchHit = paramObj->branchHit;
	int_vec &lastBranchHit = paramObj->lastBranchHit;
	int startIdx = paramObj->startIdx;

	stateIdxMap_t& stateTable = paramObj->stateTable;
	state_pVec& pStateList = paramObj->stateList;

	lastBranchHit = int_vec(NUM_BRANCH, 0);

	state_pVec startPool;
	state_t* start_state = paramObj->stateList.back();
	startPool.push_back(start_state);

	//int vec_offset = paramObj->stateList.size() - startIdx;

	rtlCkt->setCktState(start_state);
	rtlCkt->resetCounters();

	int TAR_EDGE_FITNESS	= -2000;
	int TAR_NODE_FITNESS	= -1000;
	int SELF_LOOP_FITNESS 	= -500;
	int OUT_EDGE_FITNESS	= 1000;
	int UNIMPT_FITNESS		= 0;
	
	int NEW_STATE_FIT = 20;
	int WT_NEW_STATE = -10;
	/*	Run GA	*/
	gaPopulation_t stage2Pop(POP_SIZE, VEC_LEN);
	stage2Pop.initPopulation(startPool);
	
	fitness_t prev_gen_fitness = (2 << 24), curr_gen_fitness = (2 << 25);
	string path = paramObj->currPath;
	//NUM_GEN = 1;
	for (int gen = 0; gen < NUM_GEN; ++gen) {
		
		cout << "GEN " << gen << endl;

		fitness_t curr_gen_max_fit = - (2 << 20), curr_gen_min_fit = (2 << 20);
		for (int ind = 0; ind < POP_SIZE; ++ind) {
			
			rtlCkt->resetCounters();
			gaIndiv_t* indiv = stage2Pop.indiv_vec[ind];

			/* Reset masking for b12 */
			#if defined(__b12)
			for (uint x = 0; x < indiv->input_vec.length(); 
					x += NUM_INPUT_BITS) 
				indiv->input_vec[x] = '0';
			#endif

			indiv->simCkt(rtlCkt);
			//indiv->simCkt(cktVar);
			
			int path_idx = 0, exit_state_loop = 0;
			int curr_node = start_node;
			//int indiv_idx = 0;
			int_vec state_fit_vec;
			fitness_t prev_fit = 0;

			fitness_t fitness_state = 0;
			uint st_idx = 0;
			for (state_pVec_iter st = indiv->state_list.begin();
					st != indiv->state_list.end(); ++st, ++st_idx) {

				if (FAULT_COVERAGE && (startIdx > 0)) {
					keyVal_t hash = (*st)->getHash();
					double avg_hamming_dist = 0;
					string currStr = (indiv->state_list[st_idx])->getState();
					if (stateTable.find(hash) != stateTable.end()) {
						vector<int>& indexRef = stateTable[hash];
						for (int_vec_iter vt = indexRef.begin(); 
								vt != indexRef.end(); ++vt) {
							string prevStr = pStateList[*vt]->getState();
							/* Compute hamming distance */
							avg_hamming_dist += compHammingDist(currStr, 
													prevStr);
						}
						if (indexRef.size())
							avg_hamming_dist /= indexRef.size();
						fitness_state -= avg_hamming_dist;
					}
					else 
						fitness_state -= NEW_STATE_FIT;
				}

				/* Compute	- target edge [nxt_edge]
							- target node [nxt_node]
							- set of branches in self loop	*/

				bNode_t* curr_node_obj = branchGraph->getNode(curr_node);
				assert(curr_node_obj);

				int nxt_node = curr_node_obj->outNodes[path[path_idx] - '0'];
				int nxt_edge = curr_node_obj->outEdges[path[path_idx] - '0'];

				set<int> self_loop_br, out_node_br;
				for (int bt = 0; (uint) bt < curr_node_obj->outEdges.size(); ++bt) { 
					int br = curr_node_obj->outEdges[bt];
					if (br == nxt_edge)
						continue;

					if (curr_node_obj->outNodes[bt] == curr_node)
						self_loop_br.insert(br);
					else
						out_node_br.insert(br);
				}
				
				int state_fit = 0;
				for (int_vec_iter br = (*st)->branch_index.begin(); 
						br != (*st)->branch_index.end(); ++br) {
					
					/* Add vector to branch	*/

					if(IsBranchLeaf[*br] == 0)
						continue;

					bEdge_t* curr_edge_obj = branchGraph->getEdge(*br); 
					if (curr_edge_obj == NULL)
						continue;
					
					/* If correct edge [nxt_edge] was taken */
					if (*br == nxt_edge) {
//						state_fit += (TAR_EDGE_FITNESS) * (path_idx + 1);
						state_fit += prev_fit + TAR_EDGE_FITNESS;
						path_idx++;
						curr_node = nxt_node;
						break;
					}
						
					/* if correct node [nxt_node] was reached */
					else if (curr_edge_obj->endTop == nxt_node) {
//						state_fit += (TAR_NODE_FITNESS) * (path_idx + 1);
						state_fit += prev_fit + TAR_NODE_FITNESS;
						path_idx++;
						curr_node = nxt_node;
						break;
					}

					/* If self loop was taken */
					else if (self_loop_br.find(*br) != self_loop_br.end()) {
						state_fit += prev_fit + SELF_LOOP_FITNESS;
						break;
					}

					/* Else if any other edge was taken */
					else if (out_node_br.find(*br) != out_node_br.end()) {
//						indiv_idx = path_idx;	
						exit_state_loop = true;
						state_fit += prev_fit + OUT_EDGE_FITNESS;
						break;
					}

					/* If the branch does not matter */
					else {
						state_fit += UNIMPT_FITNESS;
					}
				}	

				state_fit_vec.push_back(state_fit);
				prev_fit = state_fit;

				if ((exit_state_loop) || ((uint)path_idx == path.length())){
//					cout << "Indiv " << ind << " digressed @ index " 
//						 << path_idx << "after" << state_fit_vec.size()
//						 << endl;
					break;
				}

			}	// END State
			
//			cout << "Indiv " << ind << " reached " << state_fit_vec.size()
//				 << " vectors"  << endl
//				 << "Fitness: ";
//			printVec(state_fit_vec);
//			cout << endl;
//			indiv->printIndiv(0);

			indiv->fitness = WT_NEW_STATE * fitness_state;			
			int max_ind = -1, max_fit = (2 << 20);
			for (int it = 0; (uint)it < state_fit_vec.size(); ++it) {
				if (max_fit >= state_fit_vec[it]) {
					max_ind = it;
					max_fit = state_fit_vec[it];
				}
				indiv->fitness += state_fit_vec[it];
			}

			if (max_ind != -1)
				indiv->max_index = max_ind;

			if (curr_gen_fitness >= indiv->fitness)
				curr_gen_fitness = indiv->fitness;

			if (curr_gen_min_fit >= indiv->fitness)
				curr_gen_min_fit = indiv->fitness;

			if (curr_gen_max_fit <= indiv->fitness)
				curr_gen_max_fit = indiv->fitness;

#ifdef S2_DBG_PRINT
			cout << indiv->fitness << " : " 
				<< indiv->max_index + 1 << " vectors " 
				<< endl << endl;
#endif
		}	// END Indiv

//		cout << "Max: " << curr_gen_max_fit
//			<< " Min: " << curr_gen_min_fit << endl;
		bool gaTerminate = false;

		if ((curr_gen_min_fit == curr_gen_max_fit) ||
				(gen == NUM_GEN - 1) ||
				(prev_gen_fitness < curr_gen_fitness))
			gaTerminate = true;
			
		if (gaTerminate) {
			#ifndef PRINT_ROUND_OFF
			cout << "Terminating after GEN " << gen << endl;
			#endif
			break;
		}
		else {
			stage2Pop.gaEvolve();
			prev_gen_fitness = curr_gen_fitness;
			curr_gen_fitness = (2 << 25);
		}

	}	// END Generation

#ifndef GA_FIND_TOP_INDIV
	std::sort(stage2Pop.indiv_vec.begin(), stage2Pop.indiv_vec.end(), compFitness);
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
#endif

#ifdef GA_FIND_TOP_INDIV
	gaIndiv_t *indiv = stage2Pop.indiv_vec[0];
	for (gaIndiv_pVec_iter gt = stage2Pop.indiv_vec.begin(); 
			gt != stage2Pop.indiv_vec.end(); ++gt) {
		if (compFitness(*gt, indiv))
			indiv = *gt;
	}
#endif
	
//	cout << endl << "After GA(Core) : " << endl;
//	indiv->printIndiv(0);
//	cout << endl << indiv->max_index + 1 << " vectors copied" << endl;

	paramObj->inputVec += indiv->input_vec.substr(0, 
					NUM_INPUT_BITS * (indiv->max_index + 1));

	for (int ind = 0; ind <= indiv->max_index; ++ind) {
		state_t *st = indiv->state_list[ind];
		if (st == NULL) {
			cout << "GG" << endl;
			break;
		}
		for (int_vec_iter it = st->branch_index.begin();
				it != st->branch_index.end(); ++it)
			lastBranchHit[*it]++;

		paramObj->stateList.push_back(indiv->state_list[ind]);
		indiv->state_list[ind] = NULL;
	}
	
	paramObj->branch_index = paramObj->stateList.back()->branch_index;
	for (int ind = 0; ind < NUM_BRANCH; ++ind)
		branchHit[ind] += lastBranchHit[ind];
	
//	cout << "Returning from Stage2_Core" << endl;
//	printCnt(paramObj->branchHit);
	
	#ifndef PRINT_ROUND_OFF
	cout << paramObj->stateList.back()->getState() << endl;
	#endif
}

string getPathBFS(brGraph_t* brGraph, int start, int target) {

#ifdef S2_DBG_PRINT
	cout << "Finding path from " << start << " -> " << target << endl;
#endif
	map<int, string> label;
	map<int, int> level;

	typedef pair< map<int, string>::iterator, bool> retStr;

	int max_level = -1;
	string path = "";
	vector<int> bfsQueue;
	bfsQueue.push_back(start);
	level.insert(make_pair(start, 0));
	label.insert(make_pair(start, ""));

	const int THRESH_LEVEL = 20;
	const int MAX_QUEUE_SIZE = 1000;
	for (int qInd = 0; (uint) qInd < bfsQueue.size(); ++qInd) {

		int front = bfsQueue[qInd];
		bNode_t* curr = brGraph->getNode(front);
		assert(curr);

		//		cout << "F: " << front << endl;
		for (int eInd = 0; (uint) eInd < curr->outNodes.size(); ++eInd) {
			int node_ = curr->outNodes[eInd];

			retStr ret;
			char ch[2];
			ch[0] = eInd + 48; ch[1] = '\0';
			string tmpLabel = label[front] + string(ch);
			ret = label.insert(make_pair(node_, tmpLabel));

			int tmpLevel = level[front] + 1;
			if (ret.second) {
				level.insert(make_pair(node_, tmpLevel));
			}
			//			if (node_ == target) {
			//				path = tmpLabel;
			//				return tmpLevel;
			//			} 

			int edge_ = curr->outEdges[eInd];
			if (edge_ == target) {
				//				path = tmpLabel;
				//				return tmpLevel;
				return tmpLabel;
			}

			if (max_level < tmpLevel)
				max_level = tmpLevel;

			if (ret.second) {
				bfsQueue.push_back(node_);
			}

		}

		if ((max_level > THRESH_LEVEL) || (qInd > MAX_QUEUE_SIZE)) {
			cout << "Lvl: " << max_level << "/" << THRESH_LEVEL << "\t"
				<< "Q: " << qInd << "/" << MAX_QUEUE_SIZE << endl;
			path = "Unreachable";
			return path;
		}

	}

	path = "Unreachable";
	return path;
}

int computeBranches(vector<int>& branch_cov) {

	assert(branch_cov.size() == (uint)NUM_BRANCH);
	int num_branch = 0;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if (branch_cov[br] && !exclBranchList[br])
			num_branch ++;
	}
	return num_branch;
}

/*	Comparator function for sort() in Stage 1 - Round 0	*/
bool compCoverage(gaIndiv_t* A, gaIndiv_t* B) {

	if (A->num_branch != B->num_branch)
		return (A->num_branch > B->num_branch);
	else 
		return (A->max_index < B->max_index);

}

int compHammingDist(string strA, string strB) {
	assert(strA.length() == strB.length());
	int hamm_dist = 0;
	for (uint idx = 0; idx < strA.length(); ++idx) {
		hamm_dist += strA[idx] ^ strB[idx];
	}
	return hamm_dist;
}

void printCnt(int_vec& vec_) {
	int num_branch = 0;
	for (uint v = 0; v < vec_.size(); ++v) {
		cout << v << "(" << vec_[v] << ") ";
		if (vec_[v] == 0)
			num_branch++;
	}
	cout << endl << "#Branch " << NUM_BRANCH - num_branch << endl;
}

void printVec(int_vec& vec_) {
	for (int_vec_iter vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}

double get_time() {
   struct rusage ru;
   struct timeval rtime; 

   getrusage(RUSAGE_SELF, &ru);
   rtime=ru.ru_utime;
   double time = (double)rtime.tv_sec + (double)rtime.tv_usec / 1000000.0;
   rtime = ru.ru_stime;
   time += (double)rtime.tv_sec + (double)rtime.tv_usec / 1000000.0;

   return time;
}

void printVectorSet(const vecIn_t& inputVec) {

	ofstream vecOutFile;
	char vecName[100];
	
	sprintf(vecName, "%s_%d.vec", benchCkt, (rand() % 10000));

	cout << "Writing vectors into file -> " << vecName << endl; 
	vecOutFile.open(vecName, ios::out);
	vecOutFile << NUM_INPUT_BITS << endl;

	for (uint i = 0; i < inputVec.length(); i++) {
		vecOutFile << inputVec[i];
		if ((i+1) % NUM_INPUT_BITS == 0) {
			vecOutFile << endl;
		}
	}

	vecOutFile << "END" << endl;
	vecOutFile << rnd_seed << endl;

	vecOutFile.close();
}

double computeMetric(paramObj_t* paramObj) {
	
	state_pVec& stateList = paramObj->stateList;
	int_vec& branchHit = paramObj->branchHit;

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
	for (; (uint)stInd < stateList.size(); ++stInd) {
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
	
	#ifndef PRINT_ROUND_OFF
	cout << "Total:" << endl;
	for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx)
		cout << mapVec[vIdx].size() << " ";
	cout << endl;
	#endif

	double newMetric = 0.0;
	double newMetric2 = 0.0;
	double newMetric3 = 0.0;
//	vector<double>& brMetric = paramObj->branchFactor;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if (IsBranchLeaf[br] == 0)
			continue;

		if (branchHit[br]) {
			#ifndef PRINT_ROUND_OFF
			cout << "B: " << br << " " 
				 << branchHit[br] 
				 << endl;
			#endif

			double val = 0.0;
			double val2 = 0.0;
			double val3 = 0.0;
			for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx) {
				val += ((double)fullMapVec[br][vIdx].size()) 
							/ mapVec[vIdx].size();
				val2 += fullMapVec[br][vIdx].size();
				val3 += fullMapVec[br][vIdx].size() / sizeVec[vIdx];
				#ifndef PRINT_ROUND_OFF
				cout << fullMapVec[br][vIdx].size() << " ";
				#endif
			}
			#ifndef PRINT_ROUND_OFF
			cout << val << " " 
				// << val / branchHit[br] 
				 << endl;
			#endif
//			brMetric[br] = val;
			newMetric += val;
			newMetric2 += val2;
			newMetric3 += val3;
		}
		else {
			newMetric -= NUM_VARS;
			newMetric2 -= NUM_STATE_BITS;
		}
	}
	
	cout << "1: " << newMetric /NUM_BRANCH << endl
		 << "2: " << newMetric2/NUM_BRANCH << endl
		 << "3: " << newMetric3/NUM_BRANCH << endl;

//	cout << "1: " << newMetric/NUM_VARS << endl
//		 << "2: " << newMetric2/NUM_VARS << endl
//		 << "3: " << newMetric3/NUM_VARS << endl;
		
//	cout << "1: " << newMetric /NUM_STATE_BITS << endl
//		 << "2: " << newMetric2/NUM_STATE_BITS << endl
//		 << "3: " << newMetric3/NUM_STATE_BITS << endl;

	return newMetric2/NUM_BRANCH;
}

double computeMetricNew(paramObj_t* paramObj) {
	
	state_pVec& stateList = paramObj->stateList;
	int_vec& branchHit = paramObj->branchHit;

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
	
		if (IsBranchLeaf[br] == 0) 
			continue;

		if (branchHit[br] > 0) {
//			cout << "B: " << br << " " 
//				 << endl;

			double val = 0.0;
			for (int vIdx = 0; vIdx < NUM_VARS; ++vIdx) {
				val += ((double)fullMapVec[br][vIdx].size()) 
							/ mapVec[vIdx].size();
			//	val2 += fullMapVec[br][vIdx].size();
			//	val3 += fullMapVec[br][vIdx].size() / sizeVec[vIdx];
//				cout << fullMapVec[br][vIdx].size() << " ";
			}
//			cout << endl
//				 << val << " " 
//				 << endl;
			newMetric += val;
			++num_branch;
		}
		else if (branchHit[br] == 0) {
			negVal += NUM_VARS;
		}
	}
	
	cout << "1: " << newMetric 
		 << " - " << negVal 
		 << " = " << newMetric - negVal << endl;
	return (newMetric - negVal);
}
