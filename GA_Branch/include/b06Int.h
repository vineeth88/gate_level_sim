#ifndef __B06_INT_H__
#define __B06_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b06/cpp/Vtop__Syms.h"
#include "lib/b06/cpp/Vtop.h"

// User Defined Datatypes
#include "circuit.h"
#include "rtLevelSim.h"

const int NUM_INPUT = 3;
const int NUM_INPUT_BITS = 3;
const int NUM_OUTPUT = 4;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 3;

const int NUM_BRANCH = 24;
const int NUM_VARS = 1;
const int VAR_START_ARR[] = {0};
const int VAR_SIZE_ARR[] = {3};

void set_input(Vtop*, const vecIn_t&);

inline
bool IsDefaultBranch (const int br) {
	return (br == 22);
}

inline
void sim_reset_clock(Vtop *top) {        
    top->reset = 0;     // b circuit
    top->clock = 0;
}

inline
void ToggleClk(Vtop *top) {
    top->clock = ((top->clock & 1) ^ 1); // Toggle clock 
}

#endif	// __B06_INT_H__
