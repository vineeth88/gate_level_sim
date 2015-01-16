#include "b06Int.h"
#include <bitset>
#include "rtLevelSim.h"

using namespace std;

void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
	top->eql = input[0] - 48;
	top->cont_eql = input[1] - 48;
	top->reset = ((input[2] - 48) & 1) ^ 1;
}

void set_input(Vtop* top, const int_vec& input) {
	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
	top->eql = input[0] & 1;
	top->cont_eql = input[1] & 1;
	top->reset = (input[2] & 1) ^ 1;
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
	}
	
	cktVar->v__DOT__curr_state = val;
	
//	cout << "Set state to " << val << endl;
	if (currState)
		delete currState;

	this->currState = new cktState(state);
}

string rtLevelCkt :: getCktState() const {
	
	assert(cktVar != NULL);

	string stateVal = std::string(numFFs, '0');
	int val = (uint)cktVar->v__DOT__curr_state;
	
//	cout << endl << "CktState (rtl)"
//		 << endl << val << endl;
	int i = NUM_STATE_BITS-1;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}
	
	return stateVal;
}

//void SimMultiCycle(Vtop* top, int NUM_CYCLES) {
//
//    vecIn_t vecIn(CONST_NUM_INPUT_BITS, '0');
//    init_reset_clock(top);
//
//    bool rstFlag = false;
//    int main_time = 0;
//    int cycle_cnt = 0;
//    int rstCycle = 10;
//
//    while ((main_time < (rstCycle + 2*NUM_CYCLES))
//            && !Verilated::gotFinish()) {
//
//        if (main_time >= rstCycle && !rstFlag) {
//            rstFlag = true;
//            sim_reset_clock(top);
//        }
//
//        if (start_sim(top)) {
//            RandomVecIn(vecIn);
//            set_input(top, vecIn);
//            cycle_cnt++;
//        }
//
//        top->eval();
//
//        main_time++;
//        ToggleClk(top);
//    }
//    
//}
//
//void SimMultiCycle(Vtop* top, vecIn_t& inputVec) {
//    init_reset_clock(top);   
//    bool rstFlag = false;
//    int main_time = 0;                                                           
//    int cycle_cnt = 0;                                                                  
//    int rstCycle = 10;
//
//	int vec_length = (inputVec.length()/CONST_NUM_INPUT_BITS);
//    
//    while ((main_time < (rstCycle + 2*vec_length))
//            && !Verilated::gotFinish()) {
//
//        if (main_time >= rstCycle && !rstFlag) {
//			rstFlag = true;
//			sim_reset_clock(top);
//        }
//
//        if (start_sim(top)) {
//			set_input(top, inputVec.substr(cycle_cnt, CONST_NUM_INPUT_BITS));
//			cycle_cnt += CONST_NUM_INPUT_BITS;
//        }   
//            
//        top->eval();                                                                     
//        main_time++;
//        ToggleClk(top);
//    }   
//        
//}
//
//void SimOneCycle(Vtop* top, vecIn_t& vecIn) {
//	
//    uint main_time = 0;
//	sim_reset_clock(top);
//	//cout << "Simulating " << vecIn << endl;
//	
//	assert (vecIn.length() == (uint)CONST_NUM_INPUT_BITS);
//	#ifdef _ResetMask_
//	ModifyVecIn(vecIn, gVarClass::resetInput);
//	#endif
//    while ((main_time < 2) && !Verilated::gotFinish()) {
//    	if (start_sim(top))
//            set_input(top, vecIn);
//
//        top->eval();
//
//        main_time++;
//        ToggleClk(top);
//    }
//   	
//}
//	
//void setAllXState(Vtop* top) {
//    top->v__DOT__curr_state = rand() & 0x07;
//}
//
//void printCktState (Vtop* top) {
//    cout << (uint) top->v__DOT__curr_state << endl ;
//}
