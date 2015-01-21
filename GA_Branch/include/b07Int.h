#ifndef __B07_INT_H__
#define __B07_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/b07/cpp/Vtop__Syms.h"
#include "lib/b07/cpp/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 2;
const int NUM_INPUT_BITS = 2;
const int NUM_OUTPUT = 1;
const int NUM_OUTPUT_BITS = 8;
const int NUM_STATE_BITS = 42; //43;// + 128;

const int NUM_BRANCH = 20;
const int NUM_VARS = 6;

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
    top->clock = ((top->clock & 1) ^ 1); // Toggle clock 
}

#endif	// __B07_INT_H__
