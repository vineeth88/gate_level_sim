#include "b12Int.h"
#include <bitset>
#include "rtLevelSim.h"
#include <sstream>

using namespace std;

//#define B12_PRINT_STATE_VAL

void set_input(Vtop *top, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
    top->start 	= input[0] - '0';
	top->k 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->k = ((top->k << 1) | ((input[1+i] - '0')& 1));
	}
	top->reset = (input[5] - '0') & 1;
}

void set_input(Vtop *top, const int_vec& input) {

	assert(input.size() == (uint)NUM_INPUT_BITS);
    top->start 	= input[0] & 1;
	top->k 	= 0; 
	for(uint i = 0; i < 4; ++i) {
		top->k = ((top->k << 1) | (input[1+i] & 1));
	}
	top->reset = input[5] & 1;
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
void rtLevelCkt :: printNextState() {}

inline
void rtLevelCkt :: printCurrState() {}

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
			cktVar->v__DOT__memory[31-i/2] = val;
			val = 0;
		}
	}

}

void rtLevelCkt :: setCktOutput(string outStr) {
	assert(outStr.length() == NUM_OUTPUT_BITS);
	cktVar->nloss = (outStr[0] - '0') & 1;
	int val = 0;
	for(uint i = 1; i < 5; ++i)
		val = ((val << 1) | ((outStr[i] - '0') & 0x1));
	cktVar->nl = val & 0xf;
	cktVar->speaker = (outStr[5] - '0') & 1;
	
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);
	
	#ifdef B12_PRINT_STATE_VAL
	cout << endl
		<< (uint) cktVar->v__DOT__wr 		<< " "					
		<< (uint) cktVar->v__DOT__address	<< " "
		<< (uint) cktVar->v__DOT__data_in	<< " "
		<< (uint) cktVar->v__DOT__data_out	<< " "
		<< (uint) cktVar->v__DOT__num		<< " "
		<< (uint) cktVar->v__DOT__sound	<< " "	
		<< (uint) cktVar->v__DOT__play		<< " "
		<< (uint) cktVar->v__DOT__s		<< " "	
		<< (uint) cktVar->v__DOT__counter	<< " "
		<< (uint) cktVar->v__DOT__count	<< " ";
	cout << "[ ";
	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
		cout << (uint) cktVar->v__DOT__memory[__Vi0] << " ";
    }
	cout << "] ";// << endl;
	// [.. - 22] + [86 - ..]
	cout 
		<< (uint) cktVar->v__DOT__gamma 	<< " "		
		<< (uint) cktVar->v__DOT__ind 		<< " "
		<< (uint) cktVar->v__DOT__scan		<< " "
		<< (uint) cktVar->v__DOT__max		<< " "
		<< (uint) cktVar->v__DOT__timebase	<< " "
		<< (uint) cktVar->v__DOT__count2	<< " "
		<< endl;
	#endif

	#ifdef B12_PRINT_STATE_STRING
	stringstream ss;
	ss
		<< (bitset<1>)	cktVar->v__DOT__wr 		
		<< (bitset<5>)	cktVar->v__DOT__address	
		<< (bitset<2>)	cktVar->v__DOT__data_in	
		<< (bitset<2>)	cktVar->v__DOT__data_out	
		<< (bitset<2>)	cktVar->v__DOT__num		
		<< (bitset<3>)	cktVar->v__DOT__sound		
		<< (bitset<1>)	cktVar->v__DOT__play		
		<< (bitset<1>)	cktVar->v__DOT__s			
		<< (bitset<3>)	cktVar->v__DOT__counter	
		<< (bitset<2>)	cktVar->v__DOT__count;
	
//	ss << " ";

	// 22	 
	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
		ss << (bitset<2>) cktVar->v__DOT__memory[__Vi0];
    }
	// 86
	ss //<< " "
		<< (bitset<5>)	cktVar->v__DOT__gamma 	
		<< (bitset<2>)	cktVar->v__DOT__ind 		
		<< (bitset<5>)	cktVar->v__DOT__scan		
		<< (bitset<5>)	cktVar->v__DOT__max		
		<< (bitset<6>)	cktVar->v__DOT__timebase 
		<< (bitset<6>)	cktVar->v__DOT__count2;
	
	string state_val;
	ss >> state_val;
	#endif

	string stateVal = std::string(numFFs, '0');
	
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
	stateVal[15] = (cktVar->v__DOT__play & 0x1) + '0';

//	i = 16;
	stateVal[16] = (cktVar->v__DOT__s & 0x1) + '0';

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
		i = 23 + j*2;
		val = (uint)(cktVar->v__DOT__memory[31-j] & 0x3);
		while (val) {
			stateVal[i] = (val & 0x1) + '0';
			i--;
			val = val >> 1;
		}
	}
	
	#ifdef B12_PRINT_STATE_STRING
	cout << state_val << endl
		 << stateVal << endl;
	#endif

	return stateVal;
}

string rtLevelCkt :: getOutputs() const {
	assert(cktVar != NULL);

	string stateVal = std::string(numOutputs, '0');

	stateVal[0] = (cktVar->nloss & 1) + '0';

	int i = 4;
	int val = (uint)(cktVar->nl & 0xf);
	while (val) {
		stateVal[i] = (val & 1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[5] = (cktVar->speaker & 1) + '0';

	return stateVal;
}
