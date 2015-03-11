#include "b13Int.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->data_in = 0; 
	for(uint i = 0; i < 8; ++i) {
		top->data_in = ((top->data_in << 1) | ((input[i] - '0') & 1));
	}
    top->eoc = (input[8] - '0') & 1;
	top->dsr = (input[9] - '0') & 1;
	top->reset = (input[10] - '0') ^ 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (std::bitset<8>) (cktVar->data_in & 0xff) 
		 << (uint) (cktVar->eoc & 1)
		 << (uint) (cktVar->dsr & 1)
		 << (uint) (cktVar->reset ^ 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<4>) (cktVar->canale & 0xf)
		 << (uint) (cktVar->soc & 1)
		 << (uint) (cktVar->load_dato & 1)
		 << (uint) (cktVar->add_mpx2 & 1)
		 << (uint) (cktVar->mux_en & 1)
		 << (uint) (cktVar->error & 1)
		 << (uint) (cktVar->data_out & 1)
		 << endl;
}

inline
void rtLevelCkt :: printNextState() {}

inline
void rtLevelCkt :: printCurrState() {}

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
			case 3 :	cktVar->v__DOT__S1= val;
						val = 0;
						break;

			case 5 :	cktVar->v__DOT__S2 = val;
						val = 0;
						break;

			case 6 :	cktVar->v__DOT__mpx = val;
						val = 0;
						break;

			case 7 :	cktVar->v__DOT__rdy = val;
						val = 0;
						break;

			case 8 :	cktVar->v__DOT__send_data = val;
						val = 0;
						break;

			case 9 :	cktVar->v__DOT__confirm = val;
						val = 0;
						break;

			case 10:	cktVar->v__DOT__shot = val;
						val = 0;
						break;

			case 11:	cktVar->v__DOT__send_en = val;
						val = 0;
						break;

			case 12:	cktVar->v__DOT__tre = val;
						val = 0;
						break;

			case 20:	cktVar->v__DOT__out_reg = val;
						val = 0;
						break;
			
			case 24:	cktVar->v__DOT__next_bit = val;
						val = 0;
						break;
			
			case 25:	cktVar->v__DOT__tx_end = val;
						val = 0;
						break;
			
			case 27:	cktVar->v__DOT__itfc_state = val;
						val = 0;
						break;
			
			case 28:	cktVar->v__DOT__send = val;
						val = 0;
						break;
			
			case 29:	cktVar->v__DOT__load = val;
						val = 0;
						break;
			
			case 39:	cktVar->v__DOT__tx_conta = val;
						val = 0;
						break;

			case 43:	cktVar->v__DOT__conta_tmp = val;
						val = 0;
						break;
			
			default:	break;
		}

	}

}

void rtLevelCkt :: setCktOutput(string outStr) {
	assert(outStr.length() == (uint)NUM_OUTPUT_BITS);
	int val = 0;
	for(uint i = 0; i < 4; ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));
	cktVar->canale = val & 0xf;

	cktVar->soc = (outStr[4] - '0') & 1;
	cktVar->load_dato = (outStr[5] - '0') & 1;
	cktVar->add_mpx2 = (outStr[6] - '0') & 1;
	cktVar->mux_en= (outStr[7] - '0') & 1;
	cktVar->error = (outStr[8] - '0') & 1;
	cktVar->data_out = (outStr[9] - '0') & 1;
	
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');

	int i = 2;
	int val = (uint)(cktVar->v__DOT__S1 & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 4;
	val = (uint)(cktVar->v__DOT__S2 & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[5] = (cktVar->v__DOT__mpx & 0x1) + '0';
	stateVal[6] = (cktVar->v__DOT__rdy & 0x1) + '0';
	stateVal[7] = (cktVar->v__DOT__send_data & 0x1) + '0';
	stateVal[8] = (cktVar->v__DOT__confirm & 0x1) + '0';
	stateVal[9] = (cktVar->v__DOT__shot & 0x1) + '0';
	stateVal[10] = (cktVar->v__DOT__send_en & 0x1) + '0';
	stateVal[11] = (cktVar->v__DOT__tre & 0x1) + '0';

	i = 19;
	val = (uint)(cktVar->v__DOT__out_reg & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 23;
	val = (uint)(cktVar->v__DOT__next_bit & 0xf);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[24] = (cktVar->v__DOT__tx_end & 0x1) + '0';

	i = 26;
	val = (uint)(cktVar->v__DOT__itfc_state & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[27] = (cktVar->v__DOT__send & 0x1) + '0';
	stateVal[28] = (cktVar->v__DOT__load & 0x1) + '0';

	i = 38;
	val = (uint)(cktVar->v__DOT__tx_conta & 0x3ff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 42;
	val = (uint)(cktVar->v__DOT__conta_tmp & 0xf);
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

	int i = 3;
	int val = (uint)(cktVar->canale & 0xf);
	while (val) {
		outStr[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}
	outStr[4] = (cktVar->soc & 1) + '0';
	outStr[5] = (cktVar->load_dato & 1) + '0';
	outStr[6] = (cktVar->add_mpx2 & 1) + '0';
	outStr[7] = (cktVar->mux_en & 1) + '0';
	outStr[8] = (cktVar->error & 1) + '0';
	outStr[9] = (cktVar->data_out & 1) + '0';

	return outStr;
}

// ========================= Old interface ==========================

keyVal_t state_t :: getHash() const {
	keyVal_t hash_value = state_val;
	return hash_value;
}

// ===================== newMetric computation ======================

