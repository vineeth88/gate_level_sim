#ifndef __OR1200_1_INT_H__ 
#define __OR1200_1_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/or1200_1/cpp/Vtop__Syms.h"
#include "lib/or1200_1/cpp/Vtop.h"

// User Defined Datatypes
#include "vType.h"

const int NUM_INPUT = 14;
const int NUM_INPUT_BITS = 144;
const int NUM_BRANCH = 25;
const int NUM_VARS = 11;		// TODO
const int NUM_STATE_BITS = 77;
const int NUM_CTRL_BITS = 8;	// TODO

void set_input(Vtop*, const vecIn_t&);

// Functions

inline
bool IsDefaultBranch(const int i)
{
    return (
//			(i == 23) || (i == 11) ||
			(i == 1)  //||
//			(i == 0)
			); // reset branch
}

inline
void sim_reset_clock(Vtop *top) {        
    top->rst = 0;     
	top->clk = 0;
}

inline
void ToggleClk(Vtop *top) {
    top->clk = !top->clk; // Toggle clk 
}

#endif	// OR1200_1_INT_H
