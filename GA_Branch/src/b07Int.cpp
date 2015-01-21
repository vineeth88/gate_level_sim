#include "b07Int.h"
#include <bitset>

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->start = (input[0] -'0') & 1;
	top->reset = (input[1] - '0') & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (int) (cktVar->start & 1)
		 << (int) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<8>)(cktVar->punti_retta & 0xff)
		 << endl;
}

inline
void rtLevelCkt :: printNextState() {}

inline
void rtLevelCkt :: printCurrState() {}

void rtLevelCkt :: setCktOutput(string outStr) {
	assert(outStr.length() == NUM_OUTPUT_BITS);
	
	int val = 0;
	for(uint i = 0; i < outStr.length(); ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 1));

	cktVar->punti_retta = val & 0xff;
}

void rtLevelCkt :: setCktState(const cktState& state) {
	string stateStr = state.getState();
	setCktState(stateStr);
}
	
void rtLevelCkt :: setCktState(const string& state) {
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < 43; ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));

		switch(i+1) {
			case 3 :	cktVar->v__DOT__stato= val;
						val = 0;
						break;

			case 11 :	cktVar->v__DOT__cont = val;
						val = 0;
						break;

			case 19 :	cktVar->v__DOT__mar = val;
						val = 0;
						break;

			case 27:	cktVar->v__DOT__x = val;
						val = 0;
						break;

			case 35:	cktVar->v__DOT__y = val;
						val = 0;
						break;

			case 42:	cktVar->v__DOT__t = val << 1;
						val = 0;
						break;

			default:	break;
		}
	}

	cktVar->v__DOT__mem[0] = 1;
	cktVar->v__DOT__mem[1] = 0xff;
	cktVar->v__DOT__mem[2] = 0;
	cktVar->v__DOT__mem[3] = 0;
	cktVar->v__DOT__mem[4] = 0;
	cktVar->v__DOT__mem[5] = 2;
	cktVar->v__DOT__mem[6] = 0;
	cktVar->v__DOT__mem[7] = 0;
	cktVar->v__DOT__mem[8] = 0;
	cktVar->v__DOT__mem[9] = 2;
	cktVar->v__DOT__mem[0xa] = 0xff;
	cktVar->v__DOT__mem[0xb] = 5;
	cktVar->v__DOT__mem[0xc] = 0;
	cktVar->v__DOT__mem[0xd] = 2;
	cktVar->v__DOT__mem[0xe] = 0;
	cktVar->v__DOT__mem[0xf] = 2;

}

string rtLevelCkt :: getCktState() const {

	assert(cktVar != NULL);
	
	string stateVal = std::string(numFFs, '0');
	
	int i = 2;
	int val = (uint)(cktVar->v__DOT__stato & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	i = 10;
	val = (uint)(cktVar->v__DOT__cont & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 18;
	val = (uint)(cktVar->v__DOT__mar & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 26;
	val = (uint)(cktVar->v__DOT__x & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	i = 34;
	val = (uint)(cktVar->v__DOT__y & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	// i = 42;	t[0] is not synthesized in gate-level
	i = 41;
	val = (uint)(cktVar->v__DOT__t & 0xff);
	val = val >> 1;
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

	int i = 7;
	int val = (uint)(cktVar->punti_retta & 0xff);
	while (val) {
		stateVal[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	return stateVal;
}

// ========================== Old interface =========================
keyVal_t state_t :: getHash() {
	// Memory is not considered in the control variables
    return state_val.substr(0,43);
}

