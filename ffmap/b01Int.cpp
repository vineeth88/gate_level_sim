#include "b01Int.h"
#include "circuit.h"
#include "rtLevelSim.h"

using namespace std;

//bool INIT = false;
//char INIT_VAL = '0';
//bool PRINT_INPUT = false;
//bool PRINT_OUTPUT = false;
//bool PRINT_CURR_STATE = false;
//bool PRINT_NEXT_STATE = false;

// String -> Integer values
void set_input(Vtop *top, const vecIn_t& input) {
	assert(input.length() == (uint)NUM_INPUT_BITS + 1);
	top->line1 = input[0] - 48;
	top->line2 = input[1] - 48;
	top->reset = input[2] - 48;
}

void set_input(Vtop* top, const int_vec& input) {
	assert(input.size() == (uint)NUM_INPUT_BITS + 1);
	top->line1 = input[0] & 1;
	top->line2 = input[1] & 1;
	top->reset = input[2] & 1;
}

inline
void rtLevelCkt :: printInputs() {
	cout << "Input : ";
	cout << (uint) (cktVar->line1 & 1)
		 << (uint) (cktVar->line2 & 1)
		 << (uint) (cktVar->reset & 1)
		 << endl;
}

inline
void rtLevelCkt :: printOutputs() {
	cout << "Output : ";
	cout << (uint) (cktVar->outp & 1)
		 << (uint) (cktVar->overflw & 1)
		 << endl;
}

inline
void rtLevelCkt :: printNextState() {
	cout << "N state : ";
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
			val++;
	}
	
	cktVar->v__DOT__process_1_stato = val;

//	cktVar->outp = stateStr[3] - '0';
//	cktVar->overflw = stateStr[4] - '0';
}

cktState :: cktState(const rtLevelCkt* ckt, int idx) {
	
	assert(ckt != NULL);

	const Vtop* cktVar = ckt->getVeriObj();
	assert(cktVar != NULL);

	stateIdx = idx;
	stateVal = std::string(ckt->numFFs, '0');
	int val = (uint)cktVar->v__DOT__process_1_stato;

	int i = NUM_STATE_BITS-1;
	while (val) {
		stateVal[i] = (val & 0x1) + '0';
		i--;
		val = val >> 1;
	}

	stateVal[3] = (char) cktVar->outp + '0';
	stateVal[4] = (char) cktVar->overflw + '0';

}

//void SimMultiCycle(Vtop* top, int NUM_CYCLES) {
//
//    vecIn_t vecIn(CONST_NUM_INPUT_BITS,'0');
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
//void setAllXState(Vtop* top) {
//    top->v__DOT__process_1_stato = rand() & 0x07;
//}
//
//void printCktState (Vtop* top) {
//	cout << (uint) top->v__DOT__process_1_stato << endl;
//}
