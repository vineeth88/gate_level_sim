#ifndef __B11_INT_H__
#define __B11_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "b11/Vtop__Syms.h"
#include "b11/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 2;
const int NUM_INPUT_BITS = 7;
const int NUM_OUTPUT = 2;
const int NUM_OUTPUT_BITS = 7;
const int NUM_STATE_BITS = 24;
const int NUM_CTRL_BITS = 16 + 32;

const int NUM_BRANCH = 32;
const int NUM_VARS = 4;

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


#endif	// __B11_INT_H__
