#ifndef __B11_INT_H__
#define __B11_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b11/cpp/Vtop__Syms.h"
#include "lib/b11/cpp/Vtop.h"

// User Defined Datatypes
#include "circuit.h"
#include "rtLevelSim.h"

const int NUM_INPUT = 3;
const int NUM_INPUT_BITS = 8;
const int NUM_OUTPUT = 1;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 25;

const int NUM_BRANCH = 32;
const int NUM_VARS = 4;

void set_input(Vtop*, const vecIn_t&);

inline
bool IsDefaultBranch(const int br) {
	return (//(br == 20) ||
			(br == 31));
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
    top->clock = !top->clock; // Toggle clock 
}


#endif	// __B11_INT_H__
