#include "rtLevelSim.h"

rtLevelCkt :: rtLevelCkt(Vtop* cktParam) {
	
	sprintf(cktName, "%s", benchCkt);
	this->cktVar = cktParam;
	
	this->numInputs = NUM_INPUT_BITS;	// Inputs + Reset
	this->numOutputs = NUM_OUTPUT_BITS;
	this->numFFs = NUM_STATE_BITS;
	
}

void rtLevelCkt :: setVeriObj(Vtop* ckt) {
	if (cktVar)
		delete cktVar;
	cktVar = ckt;
}

const Vtop* rtLevelCkt :: getVeriObj() const {
	return cktVar;
}

void rtLevelCkt :: resetCounters() {
	assert(cktVar != NULL);

	for(int br = 0; br < NUM_BRANCH; ++br) {
		(cktVar->__VlSymsp)->__Vcoverage[br] = 0;
	}
}

void rtLevelCkt :: getCoverage(int_vec& branch_vec) const {
	assert(cktVar != NULL);

	branch_vec.clear();
	for(int br = 0; br < NUM_BRANCH; ++br) {
		if ((cktVar->__VlSymsp)->__Vcoverage[br])
			branch_vec.push_back(br);
	}
}

int rtLevelCkt :: getBranchCounters(int_vec& branchHit) const {
	uint numBranchHit = 0;
	branchHit = vector<int>(NUM_BRANCH, 0);

    for (int ind = 0; ind < NUM_BRANCH; ++ind) {
		branchHit[ind] = cktVar->__VlSymsp->__Vcoverage[ind];
        	if (branchHit[ind])	
				numBranchHit++;
    }

	return numBranchHit;
}

void rtLevelCkt :: simOneVector(const vecIn_t& vecIn) {
	
	sim_reset_clock(cktVar);
	
	assert (vecIn.length() == (uint)numInputs);
	assert (cktVar != NULL);

    int time_idx = 0;
    while ((time_idx < 2) && !Verilated::gotFinish()) {
    	if (cktVar->clock == 0)
            set_input(cktVar, vecIn);

        cktVar->eval();

        time_idx++;
        ToggleClk(cktVar);
    }
}

void rtLevelCkt :: simMultiVector(const vecIn_t& inputVec) {

    int time_idx = 0;                                                           
    int cycle_cnt = 0;
	int vec_len = (inputVec.length()/NUM_INPUT_BITS);

    while ((time_idx < (2*vec_len)) && !Verilated::gotFinish()) {
        if (cktVar->clock == 0) {
			set_input(cktVar, inputVec.substr(cycle_cnt, NUM_INPUT_BITS));
			cycle_cnt += NUM_INPUT_BITS;
        }   
            
        cktVar->eval();                                                                     
        time_idx++;
        ToggleClk(cktVar);
    }   
}

void rtLevelCkt :: simNRandomVec(int N) {
    int time_idx = 0;                                                           

	vecIn_t vecIn;
    while ((time_idx < (2*N)) && !Verilated::gotFinish()) {
        if (cktVar->clock == 0) {
			RandomVecIn(vecIn, NUM_INPUT_BITS);
			set_input(cktVar, vecIn);
        }

        cktVar->eval();

        time_idx++;
        ToggleClk(cktVar);
    }   
}

void RandomVecIn(vecIn_t& inpVec, int size) {
	assert(size % NUM_INPUT_BITS == 0);
	inpVec = string(size, '0');
	for (uint idx = 0; idx < size; ++idx) {
		if ((idx+1) % NUM_INPUT_BITS == 0)
			inpVec[idx] = '0';
		else
			inpVec[idx] = (rand() & 1) + '0';
	}
}

// ======================== state_t ==================================

state_t::state_t() {
	
	branch_index = std::vector<int>();
	state_index = -1;
	hit_count = 0;
	pIndiv = NULL;	
	state_val = string(NUM_STATE_BITS, '0');

	#ifdef MEM_ALLOC_DBG_ON
	mem_alloc_cnt++;
	#endif
}

state_t::state_t(const state_t& copy_obj) {

	state_index = copy_obj.state_index;
	state_val = copy_obj.state_val;

	branch_index = copy_obj.branch_index;
	hit_count = copy_obj.hit_count;

	pIndiv = copy_obj.pIndiv;
	state_fitness = copy_obj.state_fitness;

	#ifdef MEM_ALLOC_DBG_ON
	mem_alloc_cnt++;
	#endif
}

state_t::state_t(const rtLevelCkt* rtlObj, int state_index_) :
	state_index(state_index_)
{
	state_val = rtlObj->getCktState();
	rtlObj->getCoverage(branch_index);

	hit_count = 0;
	pIndiv = NULL;	

	#ifdef MEM_ALLOC_DBG_ON
	mem_alloc_cnt++;
	#endif
}

state_t::~state_t() {
	state_index = -1;
	hit_count = 0;

	state_val = "INVALID STATE";
	branch_index.clear();

	#ifdef MEM_ALLOC_DBG_ON
	mem_alloc_cnt--;
	#ifdef _DBG_DEST_CALL_
	cout << endl << "Deleted state_t " << mem_alloc_cnt << endl;
	#endif
	#endif
}

state_t& state_t::operator=(const state_t& copy_obj) {   
    if (this != &copy_obj) {
		state_index = copy_obj.state_index;
		branch_index = copy_obj.branch_index;
		hit_count = copy_obj.hit_count;
		state_val = copy_obj.state_val;
	}

	return *this;
}        

bool state_t::operator==(const state_t& lhs) {   
	return (state_val.compare(lhs.state_val) == 0);
}

bool state_t :: operator[] (int idx) const {
	assert((uint)idx < state_val.length());
	return state_val[idx] - '0';
}

string state_t :: getState () const {
	return state_val;
}
// ======================== cktState =================================
//cktState :: cktState(const rtLevelCkt* ckt, int idx) {
//	assert(ckt != NULL);
//
//	stateIdx = idx;
//	stateVal = ckt->getCktState(); 
//
//}

