#include "b01Int.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->line1 = (input[0] - '0') & 1;
	top->line2 = (input[1] - '0') & 1;
	top->reset = (input[2] - '0') & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->line1 & 1)
		 << (uint) (cktVar->line2 & 1)
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (uint)(cktVar->outp & 1)
		 << (uint)(cktVar->overflw & 1)
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
	string stateStr = state.getState();
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));
	}
	
	cktVar->v__DOT__stato = val;
}

void rtLevelCkt :: setCktOutput(const string& outStr) {
	
	cktVar->outp = (outStr[0] - '0') & 1;
	cktVar->overflw = (outStr[1] - '0') & 1;

}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	int val = (uint)cktVar->v__DOT__stato;
	
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

	string outVal = std::string(numOutputs, '0');

	outVal[0] = (cktVar->outp & 1) + '0';
	outVal[1] = (cktVar->overflw & 1) + '0';

	return outVal;
}

// ========================== Old Interface =========================
keyVal_t state_t :: getHash() const {
    return state_val;
}
