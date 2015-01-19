#include "rtLevelSim.h"

rtLevelCkt :: rtLevelCkt(Vtop* cktParam) {
	
	sprintf(cktName, "%s", benchCkt);
	this->cktVar = cktParam;
	
	this->numInputs = NUM_INPUT_BITS;	// Inputs + Reset
	this->numOutputs = NUM_OUTPUT_BITS;
	this->numFFs = NUM_STATE_BITS;	// Only FFs, no Outputs 
	
//	this->prevState = NULL;
//	this->currState = NULL;
}

void rtLevelCkt :: simOneVector(const vecIn_t& vecIn) {
	
    int main_time = 0;
	sim_reset_clock(cktVar);
	
	assert (vecIn.length() == (uint)numInputs);
	assert (cktVar != NULL);

	clearBranchCounters();

    while ((main_time < 2) && !Verilated::gotFinish()) {
    	if (cktVar->clock == 0)
            set_input(cktVar, vecIn);

        cktVar->eval();

        main_time++;
        ToggleClk(cktVar);
    }
}

void rtLevelCkt :: simOneVector(const int_vec& vecIn) {
	
    int main_time = 0;
	sim_reset_clock(cktVar);
	
	assert (vecIn.size() == (uint)numInputs);
	assert (cktVar != NULL);

    while ((main_time < 2) && !Verilated::gotFinish()) {
    	if (cktVar->clock == 0)
            set_input(cktVar, vecIn);

        cktVar->eval();

        main_time++;
        ToggleClk(cktVar);
    }
   	
}

void rtLevelCkt :: setVeriObj(Vtop* ckt) {
	if (cktVar)
		delete cktVar;
	cktVar = ckt;
}

const Vtop* rtLevelCkt :: getVeriObj() const {
	return cktVar;
}

cktState :: cktState(const rtLevelCkt* ckt, int idx) {
	assert(ckt != NULL);

	stateIdx = idx;
	stateVal = ckt->getCktState(); 

}

void rtLevelCkt :: clearBranchCounters() {
	assert(cktVar != NULL);

	for(int br = 0; br < NUM_BRANCH; ++br) {
		(cktVar->__VlSymsp)->__Vcoverage[br] = 0;
	}
}

void rtLevelCkt :: getBranchCounters(vector<int>& branch_vec) {
	assert(cktVar != NULL);

	branch_vec.clear();
	for(int br = 0; br < NUM_BRANCH; ++br) {
		if ((cktVar->__VlSymsp)->__Vcoverage[br])
			branch_vec.push_back(br);
	}
}
