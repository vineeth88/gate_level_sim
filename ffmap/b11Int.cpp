#include "b11Int.h"
#include <bitset>
#include "rtLevelSim.h"
#include <sstream>

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
	top->x_in = 0; 
	for(uint i = 0; i < 6; ++i) {
		top->x_in = ((top->x_in << 1) | ((input[i] - '0') & 1));
	}
	top->stbi = input[6] - '0';
	top->reset = ((input[7] - '0') & 1) ^ 1;	// !input[7]
}

void set_input(Vtop* top, const int_vec& input) {
	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
	top->x_in = 0; 
	for(uint i = 0; i < 6; ++i) {
		top->x_in = ((top->x_in << 1) | (input[i] & 1)) ;
	}
	top->stbi = input[6] & 1;
	top->reset = (input[7] & 1) ^ 1;	// !input[7]
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
void rtLevelCkt :: printNextState() {
	cout << "N state : ";
//	cout << (int) cktVar->v__DOT__process_1_stato << endl;

	if (this->currState == NULL) {
		cout << "-N/A-" << endl;
		return;
	}
	cout << this->currState->getState()
		 << endl;

	#ifdef B11_PRINT_STATE_STRING
	stringstream ss;
   	ss 	<< (bitset<6>) cktVar->v__DOT__r_in
   	 	<< (bitset<4>) cktVar->v__DOT__stato
    	<< (bitset<5>) cktVar->v__DOT__cont
    	<< (bitset<9>) cktVar->v__DOT__cont1;

	string state_val;
	ss >> state_val;

	cout << state_val << endl;
	#endif

}

inline
void rtLevelCkt :: printCurrState() {
	cout << "P state : ";
	if (this->prevState == NULL) {
		cout << "-N/A-" << endl;
		return;
	}
	cout << this->prevState->getState() 
		 << endl;
}

void rtLevelCkt :: setCktState(const cktState& state) {
	string stateStr = state.getState();
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

			case 15 :	cktVar->v__DOT__cont = val;
						val = 0;
						break;

			case 24 :	cktVar->v__DOT__cont1 = val;
						val = 0;
						break;

			default:	break;
		}
	}
	
	if (currState)
		delete currState;

	this->currState = new cktState(state);
}

#define B11_PRINT_STATE_VAL 0
#define B11_PRINT_STATE_STRING 0

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	
//	cout << endl << "CktState (rtl)"
//		 << endl << val << endl;

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

	i = 14;
	val = (uint)(cktVar->v__DOT__cont & 0x1f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 23;
	val = (int)(cktVar->v__DOT__cont1);
//	if (val > 0xff)
//		val -= 512;

	while (i>14) {
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

//void setAllXState(Vtop* top) {
//
//   	top->v__DOT__r_in	= rand() & 0x3f; 	
//   	top->v__DOT__stato	= rand() & 0x0f; 
//    top->v__DOT__cont1	= rand() & 0x3f; 
//    top->v__DOT__cont1	= rand() & 0xffffffff; 
//
//}
//
//void printCktState (Vtop* top) {
//   	cout << (int) top->v__DOT__r_in	<< " "
//    	 << (int) top->v__DOT__stato	<< " "
//   		 << (int) top->v__DOT__cont	<< " "
//   		 << (int) top->v__DOT__cont1	<< " "
//		 << endl;
//}
//