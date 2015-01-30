#include "b10Int.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
    top->r_button = input[0] - '0';
    top->g_button = input[1] - '0';
    top->key 	= input[2] - '0';
    top->start 	= input[3] - '0';
    top->test 	= input[4] - '0';
    top->rts 	= input[5] - '0';
    top->rtr 	= input[6] - '0';
	top->v_in 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->v_in = ((top->v_in << 1) | ((input[7+i] - '0')& 1));
	}
	top->reset = (input[11] - '0') & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->r_button & 1)
    	 << (uint) (cktVar->g_button & 1)
    	 << (uint) (cktVar->key & 1)
    	 << (uint) (cktVar->start & 1)
    	 << (uint) (cktVar->test & 1)
    	 << (uint) (cktVar->rts & 1)
    	 << (uint) (cktVar->rtr & 1)
		 << (std::bitset<4>) (cktVar->v_in & 0xf) 
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (uint) (cktVar->cts & 1)
		 << (uint) (cktVar->ctr & 1)
		 << (std::bitset<4>) (cktVar->v_out & 0xf)
		 << endl;
}

inline
void rtLevelCkt :: printCurrState() {}

inline
void rtLevelCkt :: printNextState() {}

void rtLevelCkt :: readSizeVars() {
	
	this->startIdxVec = int_vec(VAR_START_ARR, VAR_START_ARR + NUM_VARS);
	this->sizeVec = int_vec(VAR_SIZE_ARR, VAR_SIZE_ARR + NUM_VARS);
	
	assert(startIdxVec.size() == sizeVec.size());
}

void rtLevelCkt :: setCktState(const state_t* state) {
	string stateStr = state->getState();
	setCktState(stateStr);
}

void rtLevelCkt :: setCktState(const string& stateStr) {
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));

		switch(i+1) {
			case 4 :	cktVar->v__DOT__stato = val;
						val = 0;
						break;

			case 5 :	cktVar->v__DOT__voto0 = val;
						val = 0;
						break;

			case 6 :	cktVar->v__DOT__voto1 = val;
						val = 0;
						break;

			case 7:		cktVar->v__DOT__voto2 = val;
						val = 0;
						break;

			case 8:		cktVar->v__DOT__voto3 = val;
						val = 0;
						break;

			case 9:		cktVar->v__DOT__sign = val >> 3;
						val = 0;
						break;

			case 10:	cktVar->v__DOT__last_g = val;
						val = 0;
						break;

			case 11:	cktVar->v__DOT__last_r = val;
						val = 0;
						break;

			default:	break;
		}
	}
}

void rtLevelCkt :: setCktOutput(string outStr) {
	assert(outStr.length() == NUM_OUTPUT_BITS);
	
	cktVar->cts = (outStr[0] - '0') & 1;
	cktVar->ctr = (outStr[1] - '0') & 1;

	int val = 0;
	for(uint i = 2; i < outStr.length(); ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));

	cktVar->v_out = val & 0xf;
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	
	int i = 3;
	int val = (uint)(cktVar->v__DOT__stato & 0xf);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	stateVal[4] = (cktVar->v__DOT__voto0 & 0x1) + '0';
	stateVal[5] = (cktVar->v__DOT__voto1 & 0x1) + '0';
	stateVal[6] = (cktVar->v__DOT__voto2 & 0x1) + '0';
	stateVal[7] = (cktVar->v__DOT__voto3 & 0x1) + '0';

	// Change to match gate level
	stateVal[8] = ((cktVar->v__DOT__sign & 0x8) >> 3) + '0';
	stateVal[9] = (cktVar->v__DOT__last_g & 0x1) + '0';
	stateVal[10] = (cktVar->v__DOT__last_r & 0x1) + '0';
	
//	i = 11; 
//	val = (uint)(cktVar->v__DOT__sign & 0xf);
//	while (val) {
//		stateVal[i] = (val & 1) + '0';
//		i--;
//		val = val >> 1;
//	}
//	stateVal[12] = (cktVar->v__DOT__last_g & 0x1) + '0';
//	stateVal[13] = (cktVar->v__DOT__last_r & 0x1) + '0';

	return stateVal;
}

string rtLevelCkt :: getOutputs() const {
	assert(cktVar != NULL);

	string outStr = std::string(numOutputs, '0');

	outStr[0] = (cktVar->cts & 1) + '0';
	outStr[1] = (cktVar->ctr & 1) + '0';
	int i = 5;
	int val = (uint)(cktVar->v_out & 0xf);
	while (val) {
		outStr[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	return outStr;
}

// ========================== Old Interface =========================

keyVal_t state_t :: getHash() const {
    return state_val;
}
