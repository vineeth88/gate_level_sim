#include "rtLevelSim.h"

rtLevelCkt :: rtLevelCkt(Vtop* cktParam) {
	this->cktVar = cktParam;
	
	this->numInputs = NUM_INPUT_BITS + 1;	// Inputs + Reset
	this->numOutputs = NUM_OUTPUT_BITS;
	this->numFFs = NUM_STATE_BITS + NUM_OUTPUT_BITS;	// FFs + Outputs 
	
	this->prevState = NULL;
	this->currState = NULL;
}

void rtLevelCkt :: simOneVector(const vecIn_t& vecIn) {
	
    int main_time = 0;
	sim_reset_clock(cktVar);
	
	assert (vecIn.length() == (uint)numInputs);
	assert (cktVar != NULL);

    while ((main_time < 2) && !Verilated::gotFinish()) {
    	if (cktVar->clock == 0)
            set_input(cktVar, vecIn);

        cktVar->eval();

        main_time++;
        ToggleClk(cktVar);
    }
   
   	int stateIdx = -1;
	if (prevState) {
		stateIdx = prevState->getIdx() + 1;
		delete prevState;
	}

	prevState = currState;
	currState = new cktState(this, stateIdx + 1);
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
   	
   	int stateIdx = -1;
	if (prevState) {
		stateIdx = prevState->getIdx() + 1;
		delete prevState;
	}

	prevState = currState;
	currState = new cktState(this, stateIdx + 1);
}

void rtLevelCkt :: setVeriObj(Vtop* ckt) {
	if (cktVar)
		delete cktVar;
	cktVar = ckt;
}

const Vtop* rtLevelCkt :: getVeriObj() const {
	return cktVar;
}
