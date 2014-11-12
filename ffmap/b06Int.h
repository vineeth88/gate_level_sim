#ifndef __B06_INT_H__
#define __B06_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "b06/Vtop__Syms.h"
#include "b06/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 2;
const int NUM_INPUT_BITS = 2;
const int NUM_OUTPUT = 4;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 3;

const int NUM_BRANCH = 24;
const int NUM_VARS = 1;

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
    top->clock = ((top->clock & 1) ^ 1); // Toggle clock 
}

#endif	// __B06_INT_H__
