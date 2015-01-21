#include "b06Int.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->eql = (input[0] - '0') & 1;
	top->cont_eql = (input[1] - '0') & 1;
	top->reset = (input[2] - '0') & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->eql & 1)
		 << (uint) (cktVar->cont_eql & 1)
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<2>)(cktVar->cc_mux & 3)
		 << (std::bitset<2>)(cktVar->uscite & 3)
		 << (std::bitset<1>)(cktVar->enable_count & 1)
		 << (std::bitset<1>)(cktVar->ackout & 1)
		 << endl;
}

inline
void rtLevelCkt :: printNextState() {}

inline
void rtLevelCkt :: printCurrState() {}

void rtLevelCkt :: setCktState(const cktState& state) {
	string stateStr = state.getState();
	setCktState(stateStr);
}

void rtLevelCkt :: setCktState(const string& stateStr) {
	string stateStr = state.getState();
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));
	}
	
	cktVar->v__DOT__curr_state = val;
}

void rtLevelCkt :: setCktOutput(const string& outStr) {
	
	int32_t val = 0;
	for(uint i = 0; i < 2; ++i) {
		val = ((val << 1) | ((outStr[i] - '0') & 1));
	}
	cktVar->cc_mux = val & 3;
	
	val = 0;
	for(uint i = 2; i < 4; ++i) {
		val = ((val << 1) | ((outStr[i] - '0') & 1));
	}
	cktVar->uscite = val & 3;

	cktVar->enable_count = (outStr[4] - '0') & 1;
	cktVar->ackout = (outStr[5] - '0') & 1;

}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	int val = (uint)cktVar->v__DOT__curr_state;
	
	int i = NUM_STATE_BITS-1;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	return stateVal;
}

string rtLevelCkt :: getCktOutput() const {
	assert(cktVar != NULL);

	string stateVal = std::string(numOutputs, '0');

	int i = 1;
	int val = (uint)(cktVar->cc_mux & 3);
	while (val) {
		stateVal[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	i = 3;
	val = (uint)(cktVar->uscite & 3);
	while (val) {
		stateVal[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}
	
	stateVal[4] = (cktVar->enable_count & 1) + '0';
	stateVal[5] = (cktVar->ackout & 1) + '0';

	return stateVal;
}


// ========================== Old interface =========================

keyVal_t state_t :: getHash() {
    return state_val;
}
