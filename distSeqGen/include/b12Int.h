#ifndef __B12_INT_H__
#define __B12_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b12/cpp/Vtop__Syms.h"
#include "lib/b12/cpp/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 3;
const int NUM_INPUT_BITS = 6;
const int NUM_OUTPUT = 3;
const int NUM_OUTPUT_BITS = 6;
const int NUM_STATE_BITS = 115;
const int NUM_CTRL_BITS = 30;

const int NUM_BRANCH = 105;
const int NUM_VARS = 8;

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

#endif	// __B12_INT_H__
