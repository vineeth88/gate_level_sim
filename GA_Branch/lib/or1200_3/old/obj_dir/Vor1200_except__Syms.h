// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header

#ifndef _Vor1200_except__Syms_H_
#define _Vor1200_except__Syms_H_

#include "verilated.h"

// INCLUDE MODULE CLASSES
#include "Vor1200_except.h"

// SYMS CLASS
class Vor1200_except__Syms : public VerilatedSyms {
  public:
    
    // LOCAL STATE
    const char* __Vm_namep;
    bool	__Vm_activity;		///< Used by trace routines to determine change occurred
    bool	__Vm_didInit;
    //char	__VpadToAlign10[6];
    
    // SUBCELL STATE
    Vor1200_except*                TOPp;
    
    // COVERAGE
    uint32_t	__Vcoverage[47];
    
    // SCOPE NAMES
    
    // CREATORS
    Vor1200_except__Syms(Vor1200_except* topp, const char* namep);
    ~Vor1200_except__Syms() {};
    
    // METHODS
    inline const char* name() { return __Vm_namep; }
    inline bool getClearActivity() { bool r=__Vm_activity; __Vm_activity=false; return r;}
    
} VL_ATTR_ALIGNED(64);

#endif  /*guard*/
