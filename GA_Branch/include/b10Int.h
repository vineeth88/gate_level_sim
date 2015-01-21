#ifndef __B10_INT_H__
#define __B10_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b10/cpp/Vtop__Syms.h"
#include "lib/b10/cpp/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 9;
const int NUM_INPUT_BITS = 12;
const int NUM_OUTPUT = 3;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 11;
//const int NUM_STATE_BITS = 14;	// Changed to match gate level 

const int NUM_BRANCH = 32;
const int NUM_VARS = 7;

void set_input(Vtop*, const vecIn_t&);

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


#endif	// __B10_INT_H__
