#include "b12Int.h"
#include <bitset>
#include "rtLevelSim.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
    top->start 	= input[0] - '0';
	top->k 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->k = ((top->k << 1) | ((input[1+i] - '0')& 1));
	}
	top->reset = ((input[5] - '0') & 1) ^ 1;
}

void set_input(Vtop *top, const int_vec& input) {

	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
    top->start 	= input[0] & 1;
	top->k 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->k = ((top->k << 1) | (input[1+i] & 1));
	}
	top->reset = (input[5] & 1) ^ 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->start & 1)
		 << (std::bitset<4>) (cktVar->k & 0xf) 
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (uint) (cktVar->nloss & 1)
		 << (std::bitset<4>) (cktVar->nl & 0xf)
		 << (uint) (cktVar->speaker & 1)
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
			case 1 :	cktVar->v__DOT__wr= val;
						val = 0;
						break;

			case 6 :	cktVar->v__DOT__address = val;
						val = 0;
						break;

			case 8 :	cktVar->v__DOT__data_in = val;
						val = 0;
						break;

			case 10:	cktVar->v__DOT__data_out = val;
						val = 0;
						break;

			case 12:	cktVar->v__DOT__num = val;
						val = 0;
						break;

			case 15:	cktVar->v__DOT__sound = val;
						val = 0;
						break;

			case 16:	cktVar->v__DOT__play = val;
						val = 0;
						break;

			case 17:	cktVar->v__DOT__s = val;
						val = 0;
						break;

			case 20:	cktVar->v__DOT__counter = val;
						val = 0;
						break;

			case 22:	cktVar->v__DOT__count = val;
						val = 0;
						break;
			
			case 91:	cktVar->v__DOT__gamma = val;
						val = 0;
						break;
			
			case 93:	cktVar->v__DOT__ind = val;
						val = 0;
						break;
			
			case 98:	cktVar->v__DOT__scan = val;
						val = 0;
						break;
			
			case 103:	cktVar->v__DOT__max = val;
						val = 0;
						break;
			
			case 109:	cktVar->v__DOT__timebase = val;
						val = 0;
						break;
			
			case 115:	cktVar->v__DOT__count2 = val;
						val = 0;
						break;
			
			default:	break;
		}

		if (i == 21)
			i = 85;
	}

	for(int i = 0; i < 64; ++i) {

		val = ((val << 1) | ((stateStr[22+i] - '0') & 0x1));
		
		if (i%2) {
			cktVar->v__DOT__memory[i/2] = val;
			val = 0;
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
	stateVal[0] = (cktVar->v__DOT__wr & 1) + '0';

	int i = 5;
	int val = (uint)(cktVar->v__DOT__address & 0x1f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 7;
	val = (uint)(cktVar->v__DOT__data_in & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 9;
	val = (uint)(cktVar->v__DOT__data_out & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 11;
	val = (uint)(cktVar->v__DOT__num & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 14;
	val = (uint)(cktVar->v__DOT__sound & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

//	i = 15;
	stateVal[i] = (cktVar->v__DOT__play & 0x1) + '0';

//	i = 16;
	stateVal[i] = (cktVar->v__DOT__s & 0x1) + '0';

	i = 19;
	val = (uint)(cktVar->v__DOT__counter & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 21;
	val = (uint)(cktVar->v__DOT__count & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 90;
	val = (uint)(cktVar->v__DOT__gamma & 0x1f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 92;
	val = (uint)(cktVar->v__DOT__ind & 0x3);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 97;
	val = (uint)(cktVar->v__DOT__scan & 0x1f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 102;
	val = (uint)(cktVar->v__DOT__max & 0x1f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 108;
	val = (uint)(cktVar->v__DOT__timebase & 0x3f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 114;
	val = (uint)(cktVar->v__DOT__count2 & 0x3f);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	for (int j = 0; j < 32; ++j) {
		i = 21 + j*2;
		val = (uint)(cktVar->v__DOT__memory[j] & 0x3);
		while (val) {
			stateVal[i] = (val & 0x1) + '0';
			i--;
			val = val >> 1;
		}
	}
}

//void state_t::printState (bool full_) {
//	if (full_)
//    	cout << state_val << endl;
//	else {
//		if(state_val.length() < 115) {
//			cout << state_val.length() << endl;
//			assert(false);
//		}
//		cout << state_val.substr(8,2) 	<< " "  // data_out
//			 << state_val.substr(12,3) 	<< " "  // sound
//			 << state_val.substr(13,1) 	<< " "  // play
//			 << state_val.substr(17,3) 	<< " "  // counter
//			 << state_val.substr(86,5) 	<< " "  // gamma
//			 << state_val.substr(93,5) 	<< " "  // scan
//			 << state_val.substr(98,5) 	<< " "  // max
//			 << state_val.substr(109,6)	<< endl;   // count2
//	}
//	for (vector<int>::iterator it = branch_index.begin();
//			it != branch_index.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//}
//
//// String -> Integer values
//void set_input(Vtop *top, const vecIn_t& input)
//{
//	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
//	top->start = input[0] - 48;
//	top->k = 0;
//	for (int i = 0; i < 4; ++i)
//		top->k = ((top->k << 1) | (input[1+i] - 48));
//}
//
//void setAllXState(Vtop* top) {
//
//	top->v__DOT__wr 		=	rand() & 0x01;
//	top->v__DOT__address	=	rand() & 0x1f; 
//	top->v__DOT__data_in	=	rand() & 0x03;
//	top->v__DOT__data_out	=	rand() & 0x03;
//	top->v__DOT__num		=	rand() & 0x03;
//	top->v__DOT__sound		=	rand() & 0x07;
//	top->v__DOT__play		=	rand() & 0x01;
//	top->v__DOT__s			=	rand() & 0x01;
//	top->v__DOT__counter	=	rand() & 0x07;
//	top->v__DOT__count		=	rand() & 0x03;
//
//	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
//		top->v__DOT__memory[__Vi0] = rand() & 0x03;
//    }
//
//	top->v__DOT__gamma 		=	rand() & 0x1f; 
//	top->v__DOT__ind 		=	rand() & 0x03;
//	top->v__DOT__scan		=	rand() & 0x1f;
//	top->v__DOT__max		=	rand() & 0x1f;
//	top->v__DOT__timebase 	=	rand() & 0x3f;
//	top->v__DOT__count2		=	rand() & 0x3f;
//
//}
//
//void printCktState (Vtop* top) {
//	cout
//		<< (uint) top->v__DOT__wr 		<< " "					
//		<< (uint) top->v__DOT__address	<< " "
//		<< (uint) top->v__DOT__data_in	<< " "
//		<< (uint) top->v__DOT__data_out	<< " "
//		<< (uint) top->v__DOT__num		<< " "
//		<< (uint) top->v__DOT__sound	<< " "	
//		<< (uint) top->v__DOT__play		<< " "
//		<< (uint) top->v__DOT__s		<< " "	
//		<< (uint) top->v__DOT__counter	<< " "
//		<< (uint) top->v__DOT__count	<< " "
//	// [.. - 22] + [86 - ..]
//		<< (uint) top->v__DOT__gamma 	<< " "		
//		<< (uint) top->v__DOT__ind 		<< " "
//		<< (uint) top->v__DOT__scan		<< " "
//		<< (uint) top->v__DOT__max		<< " "
//		<< (uint) top->v__DOT__timebase	<< " "
//		<< (uint) top->v__DOT__count2	<< " "
//		<< endl;
//	cout << "[ ";
//	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
//		cout << (uint) top->v__DOT__memory[__Vi0] << " ";
//    }
//	cout << "]" << endl;
//	
//}
