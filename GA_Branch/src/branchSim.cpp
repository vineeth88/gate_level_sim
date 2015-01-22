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

// ==================== Global Variables ============================
int BRANCH_COV_ONLY = 1;

int PARAMFILE_PRESENT = 0;
int EXCL_FILE_PRESENT = 0;
int COVGRAPH_PRESENT = 0;
int BRGRAPH_PRESENT = 0;
int CKT_WITH_MEMORY = 0;

int_vec	exclBranchList;
int_vec IsBranchLeaf;

// ===================== Class definitions ==========================
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
		int 		startIdx;		// Start index of stage 2

		int_vec		branchHit;
		int_vec		lastBranchHit;

		rtLevelCkt*	rtlCkt;
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

			branchHit = int_vec(NUM_BRANCH, 0);
			lastBranchHit = int_vec(NUM_BRANCH, 0);

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
void readExclBranchList(char[]);
int readBranchGraph(covGraph_t&, brGraph_t&);

void Stage1_GenerateVectors(paramObj_t*);

int computeBranches(vector<int>&);
bool compCoverage(gaIndiv_t*, gaIndiv_t*);

int main(int argc, char* argv[]) {

	#ifdef RANDOM_SEED
	uint32_t seed = time(NULL);
	cout << "Seed: " << seed << endl;
	srand(seed);
	#endif
	
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
					BRANCH_COV_ONLY = argv[1][2] - '0';
				else
					BRANCH_COV_ONLY = 1;
					
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

	if(BRANCH_COV_ONLY) 
		cout << "\nExecuting GA for MAX BRANCH COVERAGE" << endl;
	else
		cout << "\nExecuting GA for MAX FAULT COVERAGE" << endl;
	
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
	brGraph_t* brGraphObj;
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

	if (BRGRAPH_PRESENT) 
		paramObj->cktBrGraph = brGraphObj;

	/* Start: Stage 1 */
	Stage1_GenerateVectors(paramObj);
	
	delete paramObj;
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

	double WT_FIT_STATE = 0.0;
	double WT_FIT_COV = 0.3;
	double WT_FIT_BRANCH = 0.7;

	int WT_COV = 500;
	int WT_NEW_BRANCH = 2000;

	cout << "\nGA Stage 1: " << endl
		<< POP_SIZE << " individuals / " << VEC_LEN << " vectors" << endl;

	/* Initialize rtLevelCkt */
	Vtop *cktVar = new Vtop;
	rtLevelCkt *rtlCkt = new rtLevelCkt(cktVar);
	int_vec branch_counters(NUM_BRANCH, 0);

	vecIn_t rstVec(NUM_INPUT_BITS, '0');
	rstVec[NUM_INPUT_BITS-1] = '1';
	string rstStateStr (NUM_STATE_BITS, '0');
	rtlCkt->setCktState(rstStateStr);
	
	rtlCkt->simOneVector(rstVec);

	state_t *rstState;
	rstState = new state_t(rtlCkt, 0);
	rstState->pIndiv = NULL;

	state_pVec startPool;
	startPool.push_back(rstState);

	cout << "Reset state: " << endl
		 << rstState->getState() << endl;

	gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
	/*	Init population with one of the states in the pool 
		of start states and string of random vectors	*/
	stage0Pop.initPopulation(startPool);

	stateMap_t currStateMap, glStateMap;
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
			//	for (int x = 0; x < indiv->input_vec.length(); x += 5) 
			//		indiv->input_vec[x] = '0';
			//#endif

			rtlCkt->resetCounters();
			indiv->simCkt(rtlCkt);

//			indiv->printIndiv(0);

			/* Add states to stateMap	*/
			for (state_pVec_iter st = indiv->state_list.begin(); 
					st != indiv->state_list.end(); ++st) {
				state_t *curr = *st;
				keyVal_t hash_val_ = curr->getHash();
				retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));

				curr->hit_count = 1;
				/*	If the key for the state is present	=>	New state was not found
					If the value for the key = NULL	
					=> 	The state was found in the previous cycle
					Therefore, set the new found state as the value for that key.
					Else => The state was found in this cycle
					Therefore, just increment the count of that state		*/
				if (ret.second == false) {
					if (ret.first->second)
						(ret.first->second)->hit_count++;
					else 
						(ret.first->second) = curr;
				}
				else {
					new_state = true;
				}
			}

			indiv->num_branch = computeBranches(indiv->branch_cov);
			rtlCkt->resetCounters();

			/* Computing max and avg branch coverage */
			if (maxCov < indiv->num_branch)
				maxCov = indiv->num_branch;

			avgCov += indiv->num_branch;

		}

		cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
			<< "Max Coverage: " << maxCov << endl;

		avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
		improv_cov = (maxCov >= prevMaxCov);

		/* Compute Fitness	*/
		for (int ind = 0; ind < POP_SIZE; ++ind) {

			gaIndiv_t *indiv = stage0Pop.indiv_vec[ind];

			/* Fitness for #branches covered */
			fitness_t fitness_cov = (avgCov - indiv->num_branch) * WT_COV;

			/* Fitness for branch coverage	*/
			fitness_t fitness_branch = 0;

			/* Fitness for states reached 	*/
			fitness_t fitness_state = 0;

			/* Combining all fitness values	*/
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

			for (stateMap_iter st = currStateMap.begin(); 
					st != currStateMap.end(); ++st) 
				currStateMap[st->first] = NULL;

#ifndef GA_FIND_TOP_INDIV
			//			for (int ind = 0; ind < POP_SIZE; ++ind) {
			//				(stage0Pop.indiv_vec[ind])->printIndiv(1);
			//			}
			std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compCoverage);
			cout << "Fitness: " << (stage0Pop.indiv_vec[0])->fitness << "\t | " << (stage0Pop.indiv_vec[0])->index << endl;
			gaIndiv_t *tmp = new gaIndiv_t(*(stage0Pop.indiv_vec[0]));
#endif

#ifdef GA_FIND_TOP_INDIV
			gaIndiv_t *indiv = stage0Pop.indiv_vec[0];

			for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
					gt != stage0Pop.indiv_vec.end(); ++gt) {
				if (compCoverage(*gt, indiv))
					indiv = *gt;
			}
			cout << "Fitness: " << indiv->fitness << "\t | " << indiv->index << endl;
			gaIndiv_t *tmp = new gaIndiv_t(*indiv);
#endif

			topIndiv_vec.push_back(tmp);

			stage0Pop.gaEvolve();

			prevMaxCov = maxCov;
		}
		else
			cout << "Terminating after gen " << gen << endl;

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
#endif

#ifdef GA_FIND_TOP_INDIV
	gaIndiv_t *tmp = stage0Pop.indiv_vec[0];
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
	std::sort(topIndiv_vec.begin(), topIndiv_vec.end(), compCoverage);
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
	for (int st = 0; st <= indiv->max_index; ++st) {	
		state_t *curr = indiv->state_list[st];
		keyVal_t hash_val_ = curr->getHash();
		glStateMap.insert(make_pair(hash_val_, curr));

		for (int_vec_iter bt = curr->branch_index.begin();
				bt != curr->branch_index.end(); ++bt)
			branch_counters[*bt]++;

	}
	state_t* st = indiv->state_list[indiv->max_index];
	startVec = indiv->input_vec.substr(0,NUM_INPUT_BITS*(indiv->max_index+1));
	startPool.push_back(st);

	cout << "After Round 0: " << endl
		 << st->getState()
		 << " --------------- " << endl;

	printCnt(branch_counters);
	cout << " --------------- " << endl;

	bool Rnd_terminate = false;

	bool all_done = true;
	for (int br = 0; br < NUM_BRANCH; ++br) {
		if (!IsDefaultBranch(br) && (branch_counters[br] == 0))
			all_done = false;
	}

	for (int round = 1; !all_done && round < MAX_ROUNDS && !Rnd_terminate; ++round) {

		gaPopulation_t stage0Pop(POP_SIZE, VEC_LEN);
		/*	Init population with one of the states in the pool 
			of start states and string of random vectors	*/
		stage0Pop.initPopulation(startPool);

		stateMap_t currStateMap;
		//stateMap_t currStateMap = glStateMap;
		int prevMaxCov = 0;

		bool gaTerminate = false;
		for (int gen = 0; !gaTerminate && (gen < NUM_GEN); ++gen) {

			cout << "GEN " << gen << endl << endl;

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
					retVal_t ret = currStateMap.insert(make_pair(hash_val_, curr));

					curr->hit_count = 1;
					/*	If the key for the state is present	=>	New state was not found

						If the value for the key = NULL	
						=> The state was found in the previous cycle. 
						Therefore, set the new found state as the value for that key.

						Else => The state was found in this cycle
						Therefore, just increment the count of that state		*/

					if (ret.second == false) {
						if (ret.first->second)
							(ret.first->second)->hit_count++;
						else 
							(ret.first->second) = curr;
					}
					else {
						new_state = true;
					}
				}

				indiv->num_branch = computeBranches(indiv->branch_cov);

				/* Computing max and avg branch coverage */
				if (maxCov < indiv->num_branch)
					maxCov = indiv->num_branch;

				avgCov += indiv->num_branch;

			}

			cout << "Avg Coverage: " << (double)avgCov / (double)POP_SIZE << endl
				<< "Max Coverage: " << maxCov << endl;

			avgCov = (fitness_t)((double)avgCov / (double)POP_SIZE + 0.5);
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

				/* Combining all fitness values	*/
				indiv->fitness 
					= (fitness_cov * WT_FIT_COV) 
					+ (fitness_branch * WT_FIT_BRANCH)
					+ (fitness_state * WT_FIT_STATE);

				//				cout << "Fitness (" << ind << ") " 
				//					 << indiv->fitness 
				//					 << " [" << fitness_cov 
				//					 << ", " << fitness_branch 
				//					 << "]" << endl << endl;
			}	


			/* Terminate GA Condition:
			   - gen == NUM_GEN
			   - coverage[gen] <= coverage[gen-1]
			   - no new states reached	*/

			gaTerminate = (gen == NUM_GEN - 1) | (!improv_cov) | (!new_state);

			if (!gaTerminate) {

				for (stateMap_iter st = currStateMap.begin(); 
						st != currStateMap.end(); ++st) 
					currStateMap[st->first] = NULL;

				stage0Pop.gaEvolve();

				prevMaxCov = maxCov;
			}
			else {
				cout << "Terminating after gen " << gen << endl;
				if (prevMaxCov < maxCov)
					prevMaxCov = maxCov;
			}

		}

#ifndef GA_FIND_TOP_INDIV
		/* N.logN for sort instead of N.TOP_INDIV for finding TOP_INDIV top indivs*/
		std::sort(stage0Pop.indiv_vec.begin(), stage0Pop.indiv_vec.end(), compFitness);
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
#endif

#ifdef GA_FIND_TOP_INDIV
		gaIndiv_t *indiv = stage0Pop.indiv_vec[0];
		for (gaIndiv_pVec_iter gt = stage0Pop.indiv_vec.begin(); 
				gt != stage0Pop.indiv_vec.end(); ++gt) {
			if (compFitness(*gt, indiv))
				indiv = *gt;
		}
#endif

		cout << "Fittest indiv after round " << round << endl;
		indiv->printIndiv(1);

		int_vec curr_branch_cnt (NUM_BRANCH, 0);
		bool new_start_state = false;
		for (int st = 0; st <= indiv->max_index; ++st) {	
			state_t *curr = indiv->state_list[st];
			keyVal_t hash_val_ = curr->getHash();
			retVal_t ret = glStateMap.insert(make_pair(hash_val_, curr));

			if (ret.second == true) {
				cout << "X";
				new_start_state = true;
			}
			else {
				cout << "-";
			}
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
			if (!IsDefaultBranch(br) && (branch_counters[br] == 0)) {
				all_done_2 = false;
				break;
			}
		}

		Rnd_terminate = !(new_start_state || new_branch_) || (round == MAX_ROUNDS-1) || (all_done_2);

		startVec += indiv->input_vec.substr(0,NUM_INPUT_BITS*(indiv->max_index+1));

		for (state_pVec_iter st = startPool.begin(); st != startPool.end(); ++st)
			if (*st == NULL) {
				delete *st;
				*st = NULL;
			}

		startPool.clear();
		state_t* st = indiv->state_list[indiv->max_index];
		indiv->state_list[indiv->max_index] = NULL;

		startPool.push_back(st);

		cout << "After Round " << round << " : " << endl
			 << st->getState()
			 << " --------------- " << endl;

		printCnt(branch_counters);
		cout << " --------------- " << endl;

		if (Rnd_terminate)
			cout << "Exiting after round " << round << endl;
	}		

	rtlCkt->setCktState(rstState);
	rtlCkt->simMultiVector(startVec);
	int_vec branch_cov;
	rtlCkt->getBranchCounters(branch_cov);

	cout << "Final coverage: " << endl;
	printCnt(branch_cov);

	gaIndiv_t *indiv_ = new gaIndiv_t(0, 
			startVec.length()/NUM_INPUT_BITS,
			rstState);
	indiv_->input_vec = startVec;
	indiv_->simCkt(rtlCkt);

	paramObj->inputVec = startVec;
	paramObj->branchHit = branch_cov;
	paramObj->stateList = indiv_->state_list;

	//	cout << "Branches covered after Stage 1: " << endl;
	//	printCnt(paramObj->branchHit);
	
	delete rstState;
	delete rtlCkt;
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
