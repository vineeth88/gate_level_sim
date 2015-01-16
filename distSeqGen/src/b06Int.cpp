#include "b06Int.h"
#include <bitset>
#include "rtLevelSim.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->eql = input[0] - 48;
	top->cont_eql = input[1] - 48;
	top->reset = (input[2] - 48) & 1;
}

void set_input(Vtop* top, const int_vec& input) {
	assert(input.size() == (uint)NUM_INPUT_BITS);
	top->eql = input[0] & 1;
	top->cont_eql = input[1] & 1;
	top->reset = input[2] & 1;
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
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));
	}
	
	cktVar->v__DOT__curr_state = val;
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

string rtLevelCkt :: getOutputs() const {
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
