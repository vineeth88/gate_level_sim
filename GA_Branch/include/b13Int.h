#ifndef __B13_INT_H__
#define __B13_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b13/cpp/Vtop__Syms.h"
#include "lib/b13/cpp/Vtop.h"

// User Defined Datatypes
//#include "vType.h"
#include "circuit.h" 
#include "rtLevelSim.h"

const int NUM_INPUT = 4;
const int NUM_INPUT_BITS = 11;
const int NUM_OUTPUT = 7;
const int NUM_OUTPUT_BITS = 10;
const int NUM_STATE_BITS = 43;

const int NUM_BRANCH = 64;
const int NUM_CTRL_BITS = 30;

const int NUM_VARS = 17;
const int VAR_START_ARR[] = {0, 3, 5, 6, 7, 8, 9, 10, 11, 12, 20, 24, 25, 27, 28, 29, 39};
const int VAR_SIZE_ARR[] =  {3, 2, 1, 1, 1, 1, 1,  1,  1,  8,  4,  1,  2,  1,  1, 10,  4};
void set_input(Vtop*, const vecIn_t&);

inline 
bool IsDefaultBranch(const int br) {
	return ((br == 14) || (br == 27) 
		 || (br == 37) || (br == 59));
}

inline
void sim_reset_clock(Vtop *top)
{        
    top->reset = 0;     // b circuit
    top->clock = 0;
}

inline
void ToggleClk(Vtop *top)
{
    top->clock = ((top->clock & 1) ^ 1); // Toggle clock 
}

#endif	// __B13_INT_H__
