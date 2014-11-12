#ifndef __B10_INT_H__
#define __B10_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "b10/Vtop__Syms.h"
#include "b10/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 8;
const int NUM_INPUT_BITS = 11;
const int NUM_OUTPUT = 8;
const int NUM_OUTPUT_BITS = 11;
const int NUM_STATE_BITS = 14;

const int NUM_BRANCH = 32;
const int NUM_VARS = 7;

void set_input(Vtop*, const vecIn_t&);
void set_input(Vtop*, const int_vec&);

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
