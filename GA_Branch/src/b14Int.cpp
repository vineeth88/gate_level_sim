#include "b14Int.h"

using namespace std;

void set_input(Vtop *cktVar, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
	cktVar->datai = 0;
	for (int i = 0; i < 32; ++i) {
		cktVar->datai = ((cktVar->datai << 1) | ((input[i] - '0') & 1));
	}
	cktVar->reset = (input[32] - '0') & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (std::bitset<32>) (cktVar->datai & 0xffffffff) 
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<32>) (cktVar->addr & 0xffffffff)
		 << (std::bitset<32>) (cktVar->datao & 0xffffffff)
		 << (uint) (cktVar->rd & 1)
		 << (uint) (cktVar->wr & 1)
		 << endl;
}

inline
void rtLevelCkt :: printNextState() {}

inline
void rtLevelCkt :: printCurrState() {}

void rtLevelCkt :: setCktState(const state_t* state) {
	string stateStr = state->getState();
	setCktState(stateStr);
}

void rtLevelCkt :: setCktState(const string& stateStr) {
	assert(stateStr.length() == (uint)numFFs);

	int64_t val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {

		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));

		switch(i+1) {
			case 32 :	cktVar->v__DOT__reg0 = val;
						val = 0;
						break;

			case 64 :	cktVar->v__DOT__reg1 = val;
						val = 0;
						break;

			case 96 :	cktVar->v__DOT__reg2 = val;
						val = 0;
						break;

			case 122:	cktVar->v__DOT__reg3 = (val << 3);
						val = 0;
						break;

			case 123:	cktVar->v__DOT__B = val;
						val = 0;
						break;

			case 153:	cktVar->v__DOT__IR = (val << 1);
						val = 0;
						break;

			case 154:	cktVar->v__DOT__state = val;
						val = 0;
						break;

			case 156:	cktVar->v__DOT__d = val;
						val = 0;
						break;

			case 178:	cktVar->v__DOT__MAR = val;
						val = 0;
						break;

			case 210:	cktVar->v__DOT__MBR = val;
						val = 0;
						break;

			case 212:	cktVar->v__DOT__mf = val;
						val = 0;
						break;

			case 215:	cktVar->v__DOT__df = val;
						val = 0;
						break;

			case 216:	cktVar->v__DOT__cf = val;
						val = 0;
						break;
			
			case 220:	cktVar->v__DOT__ff = val;
						val = 0;
						break;
			
			case 252:	cktVar->v__DOT__tail = val;
						val = 0;
						break;
			
			case 284:	cktVar->v__DOT__r = val;
						val = 0;
						break;
			
			case 316:	cktVar->v__DOT__m = val;
						val = 0;
						break;
			
			case 348:	cktVar->v__DOT__t = val;
						val = 0;
						break;

			case 350:	cktVar->v__DOT__s = val;
						val = 0;
						break;

			default:	break;
		}
	}
}

void rtLevelCkt :: setCktOutput(string outStr) {
	assert(outStr.length() == (uint)NUM_OUTPUT_BITS);

	int64_t val = 0;
	for(uint i = 0; i < 32; ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));
	cktVar->addr = (val & 0xffffffff);

	val = 0;
	for(uint i = 32; i < 64; ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));
	cktVar->datao = (val & 0xffffffff);

	cktVar->rd = (outStr[64] - '0') & 1;
	cktVar->wr = (outStr[65] - '0') & 1;
	
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);
	
	string stateVal = std::string(numFFs, '0');
	
	int i = 31;
	int64_t val = (cktVar->v__DOT__reg0) & 0xffffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 63;
	val = (cktVar->v__DOT__reg1) & 0xffffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 95;
	val = (cktVar->v__DOT__reg2) & 0xffffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 121;
	val = (cktVar->v__DOT__reg3 >> 3) & 0x3ffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	stateVal[122] = (cktVar->v__DOT__B & 1);

	i = 152;
	val = (cktVar->v__DOT__IR >> 1) & 0x3fffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[153] = (cktVar->v__DOT__state & 0x1) + '0';

	i = 155;
	val = (cktVar->v__DOT__d & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	// No mapping exists for the rest 5 FFs
	i = 177;
	val = (cktVar->v__DOT__MAR & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 209;
	val = (cktVar->v__DOT__MBR & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 211;
	val = (cktVar->v__DOT__mf & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 214;
	val = (cktVar->v__DOT__df & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 215;
	val = (cktVar->v__DOT__cf & 0x1);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 219;
	val = (cktVar->v__DOT__ff & 0xf);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 251;
	val = (cktVar->v__DOT__tail & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 283;
	val = (cktVar->v__DOT__r & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 315;
	val = (cktVar->v__DOT__m & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 347;
	val = (cktVar->v__DOT__t & 0xffffffff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 349;
	val = (cktVar->v__DOT__s & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	return stateVal;
}

string rtLevelCkt :: getOutputs() const {
	assert(cktVar != NULL);
	string outStr = std::string(numOutputs, '0');

	int i = 0;
	int64_t val = (cktVar->addr & 0xffffffff);
	while (val) {
		outStr[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	i = 0;
	val = (cktVar->datao & 0xffffffff);
	while (val) {
		outStr[i+32] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}
	outStr[64] = (cktVar->rd & 1) + '0';
	outStr[65] = (cktVar->wr & 1) + '0';

	return outStr;
}

// ========================== Old Interface =========================

keyVal_t state_t :: getHash() const {
	keyVal_t hash_value = 
    	state_val.substr(122,1) + // B //(1);
    	state_val.substr(123,30) + // IR //(32);
    	state_val.substr(153,1) + // state //(1);
    	state_val.substr(154,2) + // d //(32);
    	state_val.substr(210,2) + // mf //(2);
    	state_val.substr(212,3) + // df //(3);
    	state_val.substr(215,1) + // cf //(1);
    	state_val.substr(216,4) + // ff //(4);
    	state_val.substr(252,32) + // r //(32);
    	state_val.substr(284,32) + // m //(32);
    	state_val.substr(316,32) + // t //(32);
    	state_val.substr(348,2); // s //(2);
	return hash_value;
}
