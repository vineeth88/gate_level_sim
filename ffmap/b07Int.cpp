#include "b07Int.h"
#include <bitset>
#include "rtLevelSim.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
	top->start = input[0] - 48;
	top->reset = ((input[1] - 48) & 1) ^ 1;
}

void set_input(Vtop *top, const int_vec& input) {
	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
	top->start = input[0] & 1;
	top->reset = (input[1] & 1) ^ 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->start & 1)
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<8>)(cktVar->punti_retta & 0xff)
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

			case 43:	cktVar->v__DOT__t = val;
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

//	for (int i = 0; i < 128; i++) {
//
//		val = ((val << 1) | ((stateStr[i] - '0') & 0x1));
//		
//		if (i && (i % 8 == 7))
//			cktVar->v__DOT__mem[i/8] = val;
//	}
	
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

	i = 42;
	val = (uint)(cktVar->v__DOT__t & 0xff);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

//	for (int j = 0; j < 15; ++j) {
//		i = 50 + j*8;
//		val = (uint)cktVar->v__DOT__mem[j];
//		while (val) {
//			stateVal[i] = (val & 0x1) + '0';
//			i--;
//			val = val >> 1;
//		}
//	}

}

//void state_t::setCktState(Vtop* top) {
//	int val = 0;
//	for(int i = 0; i < 43; ++i) {
//
//		val = val << 1;
//		if (state_val[i] == '1')
//			val++;
//
//		switch(i+1) {
//			case 3 :	top->v__DOT__stato= val;
//						val = 0;
//						break;
//
//			case 11 :	top->v__DOT__cont = val;
//						val = 0;
//						break;
//
//			case 19 :	top->v__DOT__mar = val;
//						val = 0;
//						break;
//
//			case 27:	top->v__DOT__x = val;
//						val = 0;
//						break;
//
//			case 35:	top->v__DOT__y = val;
//						val = 0;
//						break;
//
//			case 43:	top->v__DOT__t = val;
//						val = 0;
//						break;
//
//			default:	break;
//		}
//	}
//
//	for (int i = 0; i < 128; i++) {
//
//		val = val << 1;
//		if (state_val[43+i] == '1')
//			val++;
//		
//		if (i && (i % 8 == 7))
//			top->v__DOT__mem[i/8] = val;
//	}
//
//}
//
//void setAllXState(Vtop* top) {
//	top->v__DOT__stato	= rand() & 0x07;
//	top->v__DOT__cont 	= rand() & 0xff;
//	top->v__DOT__mar 	= rand() & 0xff;
//	top->v__DOT__x 		= rand() & 0xff;
//	top->v__DOT__y 		= rand() & 0xff;
//	top->v__DOT__t 		= rand() & 0xff;
//
//	for (int i = 0; i < 16; i++)
//		top->v__DOT__mem[i] = rand() & 0xff;
//
//}
//
//void printCktState (Vtop* top) {
//    cout << (uint) top->v__DOT__stato	<< " "
//		 << (uint) top->v__DOT__cont	<< " "
//		 << (uint) top->v__DOT__mar  	<< " "
//		 << (uint) top->v__DOT__x 	<< " "
//		 << (uint) top->v__DOT__y 	<< " "
//		 << (uint) top->v__DOT__t 	<< endl;
//}
