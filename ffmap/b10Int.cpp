#include "b10Int.h"
#include <bitset>
#include "rtLevelSim.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
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
	top->reset = ((input[2] - 48) & 1) ^ 1;
}

void set_input(Vtop *top, const int_vec& input) {

	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
    top->r_button = input[0] & 1;
    top->g_button = input[1] & 1;
    top->key 	= input[2] & 1;
    top->start 	= input[3] & 1;
    top->test 	= input[4] & 1;
    top->rts 	= input[5] & 1;
    top->rtr 	= input[6] & 1;
	top->v_in 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->v_in = ((top->v_in << 1) | (input[7+i] & 1));
	}
	top->reset = (input[2] & 1) ^ 1;
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
void rtLevelCkt :: printNextState() {
	cout << "N state : ";
//	cout << (int) cktVar->v__DOT__process_1_stato << endl;

	if (this->currState == NULL) {
		cout << "-N/A-" << endl;
		return;
	}
	cout << this->currState->getState()
		 << endl;
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
			case 4 :	cktVar->v__DOT__stato= val;
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

			case 12:	cktVar->v__DOT__sign = val;
						val = 0;
						break;

			case 13:	cktVar->v__DOT__last_g = val;
						val = 0;
						break;

			case 14:	cktVar->v__DOT__last_r = val;
						val = 0;
						break;

			default:	break;
		}
	}

	if (currState)
		delete currState;

	this->currState = new cktState(state);
}

cktState :: cktState(const rtLevelCkt* ckt, int idx) {
	
	assert(ckt != NULL);

	const Vtop* cktVar = ckt->getVeriObj();
	assert(cktVar != NULL);

	stateIdx = idx;
	stateVal = std::string(ckt->numFFs, '0');
	
//	cout << endl << "CktState (rtl)"
//		 << endl << val << endl;
	
	int i = 3;
	
	int val = (uint)cktVar->v__DOT__stato;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	stateVal[4] = (cktVar->v__DOT__voto0 & 0x1) + '0';
	stateVal[5] = (cktVar->v__DOT__voto1 & 0x1) + '0';
	stateVal[6] = (cktVar->v__DOT__voto2 & 0x1) + '0';
	stateVal[7] = (cktVar->v__DOT__voto3 & 0x1) + '0';
	
	i = 11; 
	val = (uint)(cktVar->v__DOT__sign & 0xf);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	stateVal[12] = (cktVar->v__DOT__last_g & 0x1) + '0';
	stateVal[13] = (cktVar->v__DOT__last_r & 0x1) + '0';

}

//void setAllXState(Vtop* top) {
//
//   	top->v__DOT__stato	= rand() & 0x0f; 	
//    top->v__DOT__voto0	= rand() & 0x01;
//   	top->v__DOT__voto1	= rand() & 0x01;
//   	top->v__DOT__voto2	= rand() & 0x01;
//   	top->v__DOT__voto3	= rand() & 0x01;
//   	top->v__DOT__sign	= rand() & 0x0f;
//   	top->v__DOT__last_g	= rand() & 0x01;
//   	top->v__DOT__last_r = rand() & 0x01;
//
//}
//
//void printCktState (Vtop* top) {
//   	cout << (uint) top->v__DOT__stato	<< " "
//    	 << (uint) top->v__DOT__voto0	<< " "
//   		 << (uint) top->v__DOT__voto1	<< " "
//   		 << (uint) top->v__DOT__voto2	<< " "
//   		 << (uint) top->v__DOT__voto3	<< " "
//   		 << (uint) top->v__DOT__sign	<< " "
//   		 << (uint) top->v__DOT__last_g	<< " "
//   		 << (uint) top->v__DOT__last_r 	<< endl;
//}
//
