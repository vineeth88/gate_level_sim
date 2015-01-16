#ifndef __B14_INT_H__
#define __B14_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "b14/Vtop__Syms.h"
#include "b14/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUT = 1;
const int NUM_INPUT_BITS = 32;
const int NUM_OUTPUT = 4;
const int NUM_OUTPUT_BITS = 66;
const int NUM_STATE_BITS = 398;	// TODO
const int NUM_CTRL_BITS = 270;	// TODO

const int NUM_BRANCH = 211;
const int NUM_VARS = 19;

void set_input(Vtop*, const vecIn_t&);
void set_input(Vtop*, const int_vec&);
// Functions

inline
bool IsDefaultBranch(const int i)
{
    return ((i == 80) || (i == 95) || 
			(i == 17) || (i == 21) ||
			(i == 47) || (i == 191) ||
			(i == 105) || (i == 115) || 
			(i == 125) || (i == 135) || 
			(i == 145) || (i == 155) || 
			(i == 165) || (i == 175) || 
			(i == 185) || (i == 194) || 
			(i == 199)); 
			// default branches
}

// Circuit Simulation Functions
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


#endif	// __B14_INT_H__
