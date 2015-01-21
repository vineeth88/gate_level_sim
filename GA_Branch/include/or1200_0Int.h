#ifndef __OR1200_0_INT_H__ 
#define __OR1200_0_INT_H__

// Std Include
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "lib/or1200_0/cpp/Vtop__Syms.h"
#include "lib/or1200_0/cpp/Vtop.h"

// User Defined Datatypes
#include "vType.h"

const int NUM_INPUT = 12;
const int NUM_INPUT_BITS = 111;
const int NUM_BRANCH = 19;
const int NUM_VARS = 4;		// TODO
const int NUM_STATE_BITS = 75;
const int NUM_CTRL_BITS = 7;	// TODO

void set_input(Vtop*, const vecIn_t&);

// Functions

inline
bool IsDefaultBranch(const int i) {
    return (i == 17) || (i == 1); // reset branch
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

#endif	// OR1200_0_INT_H
