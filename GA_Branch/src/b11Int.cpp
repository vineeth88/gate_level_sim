#include "b11Int.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS);
	top->x_in = 0; 
	for(uint i = 0; i < 6; ++i) {
		top->x_in = ((top->x_in << 1) | ((input[i] - '0') & 1));
	}
	top->stbi = (input[6] - '0') & 1;
	top->reset = (input[7] - '0') & 1;	
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (std::bitset<6>) (cktVar->x_in & 0x3f)
		 << (uint) (cktVar->stbi & 1)
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<6>)(cktVar->x_out & 0x3f)
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
	
void rtLevelCkt :: setCktState(const string& state) {
	assert(stateStr.length() == (uint)numFFs);

	int val = 0;
	for(uint i = 0; i < stateStr.length(); ++i) {
		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));

		switch(i+1) {
			case 6 :	cktVar->v__DOT__r_in= val;
						val = 0;
						break;

			case 10 :	cktVar->v__DOT__stato = val;
						val = 0;
						break;

			case 16 :	cktVar->v__DOT__cont = val;
						val = 0;
						break;

			case 25 :	cktVar->v__DOT__cont1 = val;
						val = 0;
						break;

			default:	break;
		}
	}
	
}

void rtLevelCkt :: setCktOutput(string outStr) {

	int val = 0;
	for(uint i = 0; i < outStr.length(); ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));
	cktVar->x_out = val & 0x3f;

}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	
	#ifdef B11_PRINT_STATE_VAL
	int cont1 = (int) cktVar->v__DOT__cont1 & 0x1ff;
	if (cont1 > 0xff)
		cont1 -= 512;
	
   	cout << (uint) cktVar->v__DOT__r_in  << "\t"
   	 	 << (uint) cktVar->v__DOT__stato << "\t"
    	 << (uint) cktVar->v__DOT__cont  << "\t"
    	 << cont1 
		 << endl;
	#endif

	#ifdef B11_PRINT_STATE_STRING
	stringstream ss;
   	ss 	<< (bitset<6>) cktVar->v__DOT__r_in
   	 	<< (bitset<4>) cktVar->v__DOT__stato
    	<< (bitset<5>) cktVar->v__DOT__cont
    	<< (bitset<9>) cktVar->v__DOT__cont1;

	string state_val;
	ss >> state_val;
	#endif

	int i = 5;
	int val = (uint)(cktVar->v__DOT__r_in & 0x3f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 9;
	val = (uint)(cktVar->v__DOT__stato & 0xf);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 15;
	val = (uint)(cktVar->v__DOT__cont & 0x3f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 24;
	val = (uint)(cktVar->v__DOT__cont1 & 0x1ff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	#ifdef B11_PRINT_STATE_STRING
	cout << state_val << endl
		 << stateVal << endl
		 << endl;
	#endif
	return stateVal;
}

string rtLevelCkt :: getOutputs() const {
	assert(cktVar != NULL);

	string stateVal = std::string(numOutputs, '0');

	int i = 5;
	int val = (uint)(cktVar->x_out & 0x3f);
	while (val) {
		stateVal[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	return stateVal;
}

// ========================= Old interface ==========================
keyVal_t state_t :: getHash() {
    return state_val;
}

