#ifndef __B01_INT_H__ 
#define __B01_INT_H__

// Std Include 
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b01/cpp/Vtop__Syms.h"
#include "lib/b01/cpp/Vtop.h"

// User Defined Datatypes
//#include "vType.h"
#include "circuit.h" 
#include "rtLevelSim.h"

const int NUM_INPUT = 3;
const int NUM_INPUT_BITS = 3;
const int NUM_OUTPUT = 2;
const int NUM_OUTPUT_BITS = 2;
const int NUM_STATE_BITS = 3;

const int NUM_BRANCH = 26;
const int NUM_VARS = 1;

void set_input(Vtop*, const vecIn_t&);

// Functions
inline
bool IsDefaultBranch(const int i) {
    return false;
}

inline
void sim_reset_clock(Vtop *top) {        
    top->reset = 0;     
    top->clock = 0;
}

inline
void ToggleClk(Vtop *top)
{
    top->clock = !top->clock; // Toggle clock 
}

#endif	// __B01_INT_H__
