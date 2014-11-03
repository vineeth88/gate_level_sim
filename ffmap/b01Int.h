#ifndef __B01_INT_H__
#define __B01_INT_H__

// Std Include 
#include "incl.h"

// Verilator Include
#include "verilated.h"
#include "b01/Vtop__Syms.h"
#include "b01/Vtop.h"

// User Defined Datatypes
#include "circuit.h"

const int NUM_INPUTS = 2;
const int NUM_INPUT_BITS = 2;
const int NUM_OUTPUTS = 2;
const int NUM_OUTPUT_BITS = 2;
const int NUM_STATE_BITS = 3;

const int NUM_BRANCH = 26;
const int NUM_VARS = 1;

void set_input(Vtop*, const vecIn_t&);
void set_input(Vtop*, const int_vec&);

//void rtLevelCkt :: printInputs();
//void rtLevelCkt :: printOutputs();
//void rtLevelCkt :: printNextState();
//void rtLevelCkt :: printCurrState();
//void rtLevelCkt :: setCktState(const cktState&);

//cktState :: cktState(rtLevelCkt*, int);

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


#endif	// __B01_INT_H__
