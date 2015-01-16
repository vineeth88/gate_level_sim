#include "b14Int.h"
#include <bitset>
#include "rtLevelSim.h"
#include <sstream>

using namespace std;

void set_input(VcktVar *cktVar, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
	cktVar->datai = 0;
	for (int i = 0; i < 32; ++i) {
		cktVar->datai = ((cktVar->datai << 1) | ((input[i] - '0') & 1));
	}
	cktVar->reset = (input[32] - '0') & 1;
}

void set_input(VcktVar *cktVar, const vecIn_t& input) {

	assert(input.length() == (uint)NUM_INPUT_BITS);
	cktVar->datai = 0;
	for (int i = 0; i < 32; ++i) {
		cktVar->datai = ((cktVar->datai << 1) | (input[i] & 1));
	}
	cktVar->reset = input[32] & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (std::bitset<32>) (cktVar->datai & 0xffffffff) 
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (std::bitset<32>) (cktVar->addr & 0xffffffff)
		 << (std::bitset<32>) (cktVar->datao & 0xffffffff)
		 << (uint) (cktVar->rd & 1)
		 << (uint) (cktVar->wr & 1)
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
			case 32 :	cktVar->v__DOT__reg0 = val;
						val = 0;
						break;

			case 64 :	cktVar->v__DOT__reg1 = val;
						val = 0;
						break;

			case 96 :	cktVar->v__DOT__reg2 = val;
						val = 0;
						break;

			case 128:	cktVar->v__DOT__reg3 = val;
						val = 0;
						break;

			case 129:	cktVar->v__DOT__B = val;
						val = 0;
						break;

			case 161:	cktVar->v__DOT__MAR = val;
						val = 0;
						break;

			case 193:	cktVar->v__DOT__MBR = val;
						val = 0;
						break;

			case 195:	cktVar->v__DOT__mf = val;
						val = 0;
						break;

			case 198:	cktVar->v__DOT__df = val;
						val = 0;
						break;

			case 199:	cktVar->v__DOT__cf = val;
						val = 0;
						break;
			
			case 203:	cktVar->v__DOT__ff = val;
						val = 0;
						break;
			
			case 235:	cktVar->v__DOT__tail = val;
						val = 0;
						break;
			
			case 267:	cktVar->v__DOT__IR = val;
						val = 0;
						break;
			
			case 268:	cktVar->v__DOT__state = val;
						val = 0;
						break;
			
			case 300:	cktVar->v__DOT__r = val;
						val = 0;
						break;
			
			case 332:	cktVar->v__DOT__m = val;
						val = 0;
						break;
			
			case 364:	cktVar->v__DOT__t = val;
						val = 0;
						break;

			case 396:	cktVar->v__DOT__d = val;
						val = 0;
						break;
			
			case 398:	cktVar->v__DOT__s = val;
						val = 0;
						break;

			default:	break;
		}

	if (currState)
		delete currState;

	this->currState = new cktState(state);
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);
	
	string stateVal = std::string(numFFs, '0');
	
	int i = 31;
	uint val = (cktVar->v__DOT__reg0);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 63;
	val = (uint)(cktVar->v__DOT__reg1);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 95;
	val = (uint)(cktVar->v__DOT__reg2);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	i = 121;
	val = (cktVar->v__DOT__reg3 >> 3) & 0x3ffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	stateVal[122] = (cktVar->v__DOT__B & 1);

	i = 152;
	val = (cktVar->v__DOT__IR >> 1) & 0x3fffffff;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[153] = (cktVar->v__DOT__state & 0x1) + '0';

	i = 155;
	val = (uint)(cktVar->v__DOT__d & 0x7);
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	// No mapping exists for the rest 5 FFs
	
	#ifdef B12_PRINT_STATE_STRING
	cout << state_val << endl
		 << stateVal << endl;
	#endif

	return stateVal;
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
//void set_input(VcktVar *cktVar, const vecIn_t& input)
//{
//	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
//	cktVar->start = input[0] - 48;
//	cktVar->k = 0;
//	for (int i = 0; i < 4; ++i)
//		cktVar->k = ((cktVar->k << 1) | (input[1+i] - 48));
//}
//
//void setAllXState(VcktVar* cktVar) {
//
//	cktVar->v__DOT__wr 		=	rand() & 0x01;
//	cktVar->v__DOT__address	=	rand() & 0x1f; 
//	cktVar->v__DOT__data_in	=	rand() & 0x03;
//	cktVar->v__DOT__data_out	=	rand() & 0x03;
//	cktVar->v__DOT__num		=	rand() & 0x03;
//	cktVar->v__DOT__sound		=	rand() & 0x07;
//	cktVar->v__DOT__play		=	rand() & 0x01;
//	cktVar->v__DOT__s			=	rand() & 0x01;
//	cktVar->v__DOT__counter	=	rand() & 0x07;
//	cktVar->v__DOT__count		=	rand() & 0x03;
//
//	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
//		cktVar->v__DOT__memory[__Vi0] = rand() & 0x03;
//    }
//
//	cktVar->v__DOT__gamma 		=	rand() & 0x1f; 
//	cktVar->v__DOT__ind 		=	rand() & 0x03;
//	cktVar->v__DOT__scan		=	rand() & 0x1f;
//	cktVar->v__DOT__max		=	rand() & 0x1f;
//	cktVar->v__DOT__timebase 	=	rand() & 0x3f;
//	cktVar->v__DOT__count2		=	rand() & 0x3f;
//
//}
//
//void printCktState (VcktVar* cktVar) {
//	cout
//		<< (uint) cktVar->v__DOT__wr 		<< " "					
//		<< (uint) cktVar->v__DOT__address	<< " "
//		<< (uint) cktVar->v__DOT__data_in	<< " "
//		<< (uint) cktVar->v__DOT__data_out	<< " "
//		<< (uint) cktVar->v__DOT__num		<< " "
//		<< (uint) cktVar->v__DOT__sound	<< " "	
//		<< (uint) cktVar->v__DOT__play		<< " "
//		<< (uint) cktVar->v__DOT__s		<< " "	
//		<< (uint) cktVar->v__DOT__counter	<< " "
//		<< (uint) cktVar->v__DOT__count	<< " "
//	cout << "[ ";
//	for (int __Vi0=0; __Vi0<32; ++__Vi0) {
//		cout << (uint) cktVar->v__DOT__memory[__Vi0] << " ";
//    }
//	cout << "] ";// << endl;
//	// [.. - 22] + [86 - ..]
//		<< (uint) cktVar->v__DOT__gamma 	<< " "		
//		<< (uint) cktVar->v__DOT__ind 		<< " "
//		<< (uint) cktVar->v__DOT__scan		<< " "
//		<< (uint) cktVar->v__DOT__max		<< " "
//		<< (uint) cktVar->v__DOT__timebase	<< " "
//		<< (uint) cktVar->v__DOT__count2	<< " "
//		<< endl;
//	
//}
keyVal_t state_t :: getHash() {
	keyVal_t hash_value = 
    	state_val.substr(128,1) + // B //(1);
    	state_val.substr(193,2) + // mf //(2);
    	state_val.substr(195,3) + // df //(3);
    	state_val.substr(198,1) + // cf //(1);
    	state_val.substr(199,4) + // ff //(4);
    	state_val.substr(235,32) + // IR //(32);
    	state_val.substr(267,1) + // state //(1);
    	state_val.substr(268,32) + // r //(32);
    	state_val.substr(300,32) + // m //(32);
    	state_val.substr(332,32) + // t //(32);
    	state_val.substr(364,32) + // d //(32);
    	state_val.substr(396,2); // s //(2);
	return hash_value;
}

state_t::state_t() {
	
	branch_index = std::vector<int>();
	state_index = -1;
	hit_count = 0;
	
	state_val = string(CONST_NUM_STATE_BITS, '0');

	mem_alloc_cnt++;
}

state_t::state_t(const state_t& copy_obj) {

	state_index = copy_obj.state_index;
	state_val = copy_obj.state_val;

	branch_index = copy_obj.branch_index;
	hit_count = copy_obj.hit_count;

	pIndiv = copy_obj.pIndiv;
	state_fitness = copy_obj.state_fitness;

	mem_alloc_cnt++;
}

state_t::state_t(const VcktVar* copy_obj, int index_) :
	state_index(index_)
{
	stringstream ss;
	ss 	<< (bitset<32>)	copy_obj->v__DOT__reg0
    	<< (bitset<32>)	copy_obj->v__DOT__reg1
    	<< (bitset<32>)	copy_obj->v__DOT__reg2
    	<< (bitset<32>)	copy_obj->v__DOT__reg3 
    	<< (bitset<1>)	copy_obj->v__DOT__B 
    	<< (bitset<32>)	copy_obj->v__DOT__MAR 
    	<< (bitset<32>)	copy_obj->v__DOT__MBR 
    	<< (bitset<2>)	copy_obj->v__DOT__mf 
    	<< (bitset<3>)	copy_obj->v__DOT__df 
    	<< (bitset<1>)	copy_obj->v__DOT__cf 
    	<< (bitset<4>)	copy_obj->v__DOT__ff 
    	<< (bitset<32>)	copy_obj->v__DOT__tail 
    	<< (bitset<32>)	copy_obj->v__DOT__IR 
    	<< (bitset<1>)	copy_obj->v__DOT__state 
    	<< (bitset<32>)	copy_obj->v__DOT__r 
    	<< (bitset<32>)	copy_obj->v__DOT__m 
    	<< (bitset<32>)	copy_obj->v__DOT__t 
    	<< (bitset<32>)	copy_obj->v__DOT__d 
    	<< (bitset<2>)	copy_obj->v__DOT__s; 
	ss >> state_val; 

	GetCoverage(copy_obj, branch_index);

	mem_alloc_cnt++;
}

state_t::~state_t() {
	state_index = -1;
	hit_count = 0;

	//state_val = "INVALID STATE";
	branch_index.clear();

	mem_alloc_cnt--;
	#ifdef _DBG_DEST_CALL_
	cout << endl << "Deleted state_t " << mem_alloc_cnt << endl;
	#endif
}

state_t& state_t::operator=(const state_t& copy_obj) {   
    if (this != &copy_obj) {

		state_index = copy_obj.state_index;
		branch_index = copy_obj.branch_index;
		hit_count = copy_obj.hit_count;
		state_val = copy_obj.state_val;
	}

	return *this;
}        

bool state_t::operator==(const state_t& lhs) {   
	return (state_val.compare(lhs.state_val) == 0);
}

string state_t::operator[] (int index_) {
	assert(index_ < CONST_NUM_VARS);
	string sstr;

	switch(index_) {
		case 0	:	sstr = state_val.substr(0,32);	// reg0 //(32);
					break;

    	case 1	:	sstr = state_val.substr(32,32);	// reg1 //(32);
					break;

    	case 2	:	sstr = state_val.substr(64,32); // reg2 //(32);
					break;

    	case 3	:	sstr = state_val.substr(96,32);	// reg3 //(32);
					break;

    	case 4	:	sstr = state_val.substr(128,1);	// B //(1);
					break;

    	case 5	:	sstr = state_val.substr(129,32);	// MAR //(32);
					break;

    	case 6	:	sstr = state_val.substr(161,32); // MBR //(32);
					break;

    	case 7	:	sstr = state_val.substr(193,2);	// mf //(2);
					break;

    	case 8	:	sstr = state_val.substr(195,3);	// df //(3);
					break;

    	case 9	:	sstr = state_val.substr(198,1);	// cf //(1);
					break;

    	case 10 :	sstr = state_val.substr(199,4);	// ff //(4);
					break;

    	case 11	:	sstr = state_val.substr(203,32);	// tail //(32);
					break;

    	case 12	:	sstr = state_val.substr(235,32);	// IR //(32);
					break;

    	case 13	:	sstr = state_val.substr(267,1);	// state //(1);
					break;

    	case 14	:	sstr = state_val.substr(268,32);	// r //(32);
					break;

    	case 15	:	sstr = state_val.substr(300,32);	// m //(32);
					break;

    	case 16	:	sstr = state_val.substr(332,32);	// t //(32);
					break;

    	case 17	:	sstr = state_val.substr(364,32);	// d //(32);
					break;

    	case 18	:	sstr = state_val.substr(396,2);	// s //(2);
					break;

		default:	break;
	}

	return sstr;
}

void state_t::printState (bool full_) {
	if (full_) {
		cout << state_val.substr(0,32) << " "// reg0 //(32);
    		 << state_val.substr(32,32) << " " // reg1 //(32);
    		 << state_val.substr(64,32) << " " // reg2 //(32);
    		 << state_val.substr(96,32) << " " // reg3 //(32);
    		 << state_val.substr(128,1) << " " // B //(1);
    		 << state_val.substr(129,32) << " " // MAR //(32);
    		 << state_val.substr(161,32) << " " // MBR //(32);
    		 << state_val.substr(193,2) << " " // mf //(2);
    		 << state_val.substr(195,3) << " " // df //(3);
    		 << state_val.substr(198,1) << " " // cf //(1);
    		 << state_val.substr(199,4) << " " // ff //(4);
    		 << state_val.substr(203,32) << " " // tail //(32);
    		 << state_val.substr(235,32) << " " // IR //(32);
    		 << state_val.substr(267,1) << " " // state //(1);
    		 << state_val.substr(268,32) << " " // r //(32);
    		 << state_val.substr(300,32) << " " // m //(32);
    		 << state_val.substr(332,32) << " " // t //(32);
    		 << state_val.substr(364,32) << " " // d //(32);
    		 << state_val.substr(396,2)	// s //(2);
			 << endl;
	}
	else {
		cout << state_val.substr(128,1) << " " // B //(1);
    		 << state_val.substr(193,2) << " " // mf //(2);
    		 << state_val.substr(195,3) << " " // df //(3);
    		 << state_val.substr(198,1) << " " // cf //(1);
    		 << state_val.substr(199,4) << " " // ff //(4);
    		 << state_val.substr(235,32) << " " // IR //(32);
    		 << state_val.substr(267,1) << " " // state //(1);
    		 << state_val.substr(268,32) << " " // r //(32);
    		 << state_val.substr(300,32) << " " // m //(32);
    		 << state_val.substr(332,32) << " " // t //(32);
    		 << state_val.substr(364,32) << " " // d //(32);
    		 << state_val.substr(396,2)	 // s //(2);
			 << endl;
	}
	for (vector<int>::iterator it = branch_index.begin();
			it != branch_index.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

void state_t::setCktState(VcktVar* cktVar) {
	int val = 0;
	for(int i = 0; i < CONST_NUM_STATE_BITS; ++i) {

		val = val << 1;
		if (state_val[i] == '1')
			val++;
		
		switch(i+1) {
			case 32 :	cktVar->v__DOT__reg0 = val;
						val = 0;
						break;

			case 64 :	cktVar->v__DOT__reg1 = val;
						val = 0;
						break;

			case 96 :	cktVar->v__DOT__reg2 = val;
						val = 0;
						break;

			case 128:	cktVar->v__DOT__reg3 = val;
						val = 0;
						break;

			case 129:	cktVar->v__DOT__B = val;
						val = 0;
						break;

			case 161:	cktVar->v__DOT__MAR = val;
						val = 0;
						break;

			case 193:	cktVar->v__DOT__MBR = val;
						val = 0;
						break;

			case 195:	cktVar->v__DOT__mf = val;
						val = 0;
						break;

			case 198:	cktVar->v__DOT__df = val;
						val = 0;
						break;

			case 199:	cktVar->v__DOT__cf = val;
						val = 0;
						break;
			
			case 203:	cktVar->v__DOT__ff = val;
						val = 0;
						break;
			
			case 235:	cktVar->v__DOT__tail = val;
						val = 0;
						break;
			
			case 267:	cktVar->v__DOT__IR = val;
						val = 0;
						break;
			
			case 268:	cktVar->v__DOT__state = val;
						val = 0;
						break;
			
			case 300:	cktVar->v__DOT__r = val;
						val = 0;
						break;
			
			case 332:	cktVar->v__DOT__m = val;
						val = 0;
						break;
			
			case 364:	cktVar->v__DOT__t = val;
						val = 0;
						break;

			case 396:	cktVar->v__DOT__d = val;
						val = 0;
						break;
			
			case 398:	cktVar->v__DOT__s = val;
						val = 0;
						break;

			default:	break;
		}
	}
}

// String -> Integer values
void set_input(VcktVar *cktVar, const vecIn_t& input)
{
	assert(input.length() == (uint)CONST_NUM_INPUT_BITS);
	cktVar->datai = 0;
	for (int i = 0; i < CONST_NUM_INPUT_BITS; ++i)
		cktVar->datai = ((cktVar->datai << 1) | (input[i] - 48));
}

void RandomVecIn(vecIn_t& vecIn)
{
	vecIn = string(CONST_NUM_INPUT_BITS, '0');
    for (uint i = 0; i < vecIn.length(); i++) 
		vecIn[i] = (rand() & 0x01) + 48;
	//cout << "RandomVecIn: " << vecIn.length() << endl;
    assert(vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
}

void SimMultiCycle(VcktVar* cktVar, int NUM_CYCLES) {

    vecIn_t vecIn(CONST_NUM_INPUT_BITS,'0');
    init_reset_clock(cktVar);

    bool rstFlag = false;
    int main_time = 0;
    int cycle_cnt = 0;
    int rstCycle = 10;

    while ((main_time < (rstCycle + 2*NUM_CYCLES))
            && !Verilated::gotFinish()) {

        if (main_time >= rstCycle && !rstFlag) {
            rstFlag = true;
            sim_reset_clock(cktVar);
        }

        if (start_sim(cktVar)) {
            RandomVecIn(vecIn);
            set_input(cktVar, vecIn);
            cycle_cnt++;
        }

        cktVar->eval();

        main_time++;
        ToggleClk(cktVar);
    }
    
}

void SimMultiCycle(VcktVar* cktVar, vecIn_t& inputVec) {
    init_reset_clock(cktVar);   
    bool rstFlag = false;
    int main_time = 0;                                                           
    int cycle_cnt = 0;                                                                  
    int rstCycle = 10;

	int vec_length = (inputVec.length()/CONST_NUM_INPUT_BITS);
	cout << "SimMultiCycle (" << vec_length << ")" << endl;    

    while ((main_time < (rstCycle + 2*vec_length))
            && !Verilated::gotFinish()) {

        if (main_time >= rstCycle && !rstFlag) {
			rstFlag = true;
			sim_reset_clock(cktVar);
        }

        if (start_sim(cktVar)) {
			set_input(cktVar, inputVec.substr(cycle_cnt, CONST_NUM_INPUT_BITS));
			cycle_cnt += CONST_NUM_INPUT_BITS;
        }   
            
        cktVar->eval();                                                                     
        main_time++;
        ToggleClk(cktVar);
    }   
        
}

void SimOneCycle(VcktVar* cktVar, vecIn_t& vecIn) {
	
    uint main_time = 0;
	sim_reset_clock(cktVar);
	//cout << "Simulating " << vecIn << endl;
	
	assert (vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
	#ifdef _ResetMask_
	ModifyVecIn(vecIn, gVarClass::resetInput);
	#endif
    while ((main_time < 2) && !Verilated::gotFinish()) {
    	if (start_sim(cktVar))
            set_input(cktVar, vecIn);

        cktVar->eval();

        main_time++;
        ToggleClk(cktVar);
    }
   	
}
	

int GetCoverage(const VcktVar* cktVar, bool printCnt) {
    uint count = 0;
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
        if (cktVar->__VlSymsp->__Vcoverage[ind]) {
			if (printCnt)
        		cout << ind << "(" << cktVar->__VlSymsp->__Vcoverage[ind] << ") ";
            count++;
		}
    }
	if (printCnt)
    	cout << endl;

    return count;
}

int GetCoverage(const VcktVar* cktVar, int index) {
    assert((index >= 0) && (index < CONST_NUM_BRANCH));
    return cktVar->__VlSymsp->__Vcoverage[index];
}

void GetCoverage(const VcktVar* cktVar, vector<int>& indVec) {
	indVec.clear();
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
        if (cktVar->__VlSymsp->__Vcoverage[ind]) {
			indVec.push_back(ind);
		}
    }

}

int GetBranchCounters(const VcktVar* cktVar, vector<int>& branchHit) {
	uint numBranchHit = 0;
	if (branchHit.size() != (uint)CONST_NUM_BRANCH)
		branchHit = vector<int>(CONST_NUM_BRANCH);

    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind) {
		branchHit[ind] = cktVar->__VlSymsp->__Vcoverage[ind];
        	if (branchHit[ind])	
				numBranchHit++;
    }

	return numBranchHit;
}

void GetResetBranch(VcktVar* cktVar, vector<int>& resetBranch) {
    SimMultiCycle(cktVar, 0);
	GetBranchCounters(cktVar, resetBranch);
}

void ResetCounters(VcktVar* cktVar) {
    for (int ind = 0; ind < CONST_NUM_BRANCH; ++ind)
        cktVar->__VlSymsp->__Vcoverage[ind] = 0;
}

void setAllXState(VcktVar* cktVar) {

	cktVar->v__DOT__reg0	=	rand() & 0xffffffff; 
	cktVar->v__DOT__reg1	=	rand() & 0xffffffff;
	cktVar->v__DOT__reg2	=	rand() & 0xffffffff;
	cktVar->v__DOT__reg3	=	rand() & 0xffffffff;
	cktVar->v__DOT__B	=	rand() & 0x01;
	cktVar->v__DOT__MAR	=	rand() & 0xffffffff;
	cktVar->v__DOT__MBR	=	rand() & 0xffffffff;
	cktVar->v__DOT__mf	=	rand() & 0x03;
	cktVar->v__DOT__df	=	rand() & 0x07;
	cktVar->v__DOT__cf	=	rand() & 0x01;
	cktVar->v__DOT__ff	=	rand() & 0x0f;
	cktVar->v__DOT__tail	=	rand() & 0xffffffff;
	cktVar->v__DOT__IR	=	rand() & 0xffffffff;
	cktVar->v__DOT__state=	rand() & 0x01;
	cktVar->v__DOT__r	=	rand() & 0xffffffff;
	cktVar->v__DOT__m	=	rand() & 0xffffffff;
	cktVar->v__DOT__t	=	rand() & 0xffffffff;
	cktVar->v__DOT__d	=	rand() & 0xffffffff;
	cktVar->v__DOT__s	=	rand() & 0x03;
}

void printCktState (VcktVar* cktVar) {
	cout 		
 		 		<< (bitset<8>) (cktVar->v__DOT__reg0 & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg0 & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg0 & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg0 & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__reg1 & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg1 & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg1 & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg1 & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__reg2 & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg2 & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg2 & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg2 & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__reg3 & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg3 & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg3 & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__reg3 & 0x000000ff)
    	 << " " << (bitset<1>) cktVar->v__DOT__B
    	 << " " << (bitset<8>) (cktVar->v__DOT__MAR & 0xff000000)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MAR & 0x00ff0000)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MAR & 0x0000ff00)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MAR & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MBR & 0xff000000)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MBR & 0x00ff0000)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MBR & 0x0000ff00)
    	 << " " << (bitset<8>) (cktVar->v__DOT__MBR & 0x000000ff)
    	 << " " << (bitset<2>) cktVar->v__DOT__mf
    	 << " " << (bitset<3>) cktVar->v__DOT__df
    	 << " " << (bitset<1>) cktVar->v__DOT__cf
    	 << " " << (bitset<4>) cktVar->v__DOT__ff
    	 << " " << (bitset<8>) (cktVar->v__DOT__tail & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__tail & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__tail & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__tail & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__IR & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__IR & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__IR & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__IR & 0x000000ff)
    	 << " " << (bitset<1>) cktVar->v__DOT__state
    	 << " " << (bitset<8>) (cktVar->v__DOT__r & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__r & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__r & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__r & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__m & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__m & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__m & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__m & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__t & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__t & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__t & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__t & 0x000000ff)
    	 << " " << (bitset<8>) (cktVar->v__DOT__d & 0xff000000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__d & 0x00ff0000)
    	 << "." << (bitset<8>) (cktVar->v__DOT__d & 0x0000ff00)
    	 << "." << (bitset<8>) (cktVar->v__DOT__d & 0x000000ff)
    	 << " " << (bitset<2>) cktVar->v__DOT__s
		 << endl;
	
}

void ModifyVecIn(vecIn_t& vecIn, const rstIn_t& resetInput) {
	assert(resetInput.length() == (uint) CONST_NUM_INPUT_BITS);
	assert(vecIn.length() == (uint) CONST_NUM_INPUT_BITS);

	for (uint it = 0; it < vecIn.length(); ++it) {	
		if(resetInput[it] == '1')
			vecIn[it] = '0';
		else if(resetInput[it] == '0')
			vecIn[it] = '1';
	}
}
