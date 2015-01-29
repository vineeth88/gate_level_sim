#ifndef __B12_INT_H__
#define __B12_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b12/cpp/Vtop__Syms.h"
#include "lib/b12/cpp/Vtop.h"

// User Defined Datatypes
//#include "vType.h"
#include "circuit.h" 
#include "rtLevelSim.h"

const int NUM_INPUT = 3;
const int NUM_INPUT_BITS = 6;
const int NUM_OUTPUT = 3;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 115;

const int NUM_BRANCH = 105;
const int NUM_CTRL_BITS = 30;

const int NUM_VARS = 17;
const int VAR_START_ARR[] = {0, 1, 6, 8, 10, 12, 15, 16, 17, 20, 22, 86, 91, 93, 
  								98, 103, 109};
const int VAR_SIZE_ARR[] =  {1, 5, 2, 2,  2,  3,  1,  1,  3,  2, 64,  5,  2,  5,
				 				 5,   6,   6};
void set_input(Vtop*, const vecIn_t&);

inline 
bool IsDefaultBranch(const int br) {
	return ((br == 19) || (br == 103));
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

#endif	// __B12_INT_H__
