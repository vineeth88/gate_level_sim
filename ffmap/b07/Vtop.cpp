// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop.h"              // For This
#include "Vtop__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vtop) {
    Vtop__Syms* __restrict vlSymsp = __VlSymsp = new Vtop__Syms(this, name());
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    clock = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    start = VL_RAND_RESET_I(1);
    punti_retta = VL_RAND_RESET_I(8);
    v__DOT__stato = VL_RAND_RESET_I(3);
    v__DOT__cont = VL_RAND_RESET_I(8);
    v__DOT__mar = VL_RAND_RESET_I(8);
    v__DOT__x = VL_RAND_RESET_I(8);
    v__DOT__y = VL_RAND_RESET_I(8);
    v__DOT__t = VL_RAND_RESET_I(8);
    { int __Vi0=0; for (; __Vi0<16; ++__Vi0) {
	    v__DOT__mem[__Vi0] = VL_RAND_RESET_I(8);
    }}
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "b07.v", 56, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "b07.v", 91, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "b07.v", 97, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "b07.v", 104, 0, ".v", "v_line/b07", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "b07.v", 95, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "b07.v", 109, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "b07.v", 114, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "b07.v", 120, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "b07.v", 126, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "b07.v", 135, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "b07.v", 142, 0, ".v", "v_line/b07", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "b07.v", 133, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "b07.v", 151, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "b07.v", 157, 0, ".v", "v_line/b07", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "b07.v", 149, 0, ".v", "v_line/b07", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "b07.v", 163, 0, ".v", "v_line/b07", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "b07.v", 148, 0, ".v", "v_line/b07", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "b07.v", 131, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "b07.v", 168, 0, ".v", "v_line/b07", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "b07.v", 89, 0, ".v", "v_line/b07", "else");
}

Vtop::~Vtop() {
    delete __VlSymsp; __VlSymsp=NULL;
}

// Coverage
void Vtop::__vlCoverInsert(uint32_t* countp, bool enable, const char* filenamep, int lineno, int column,
    const char* hierp, const char* pagep, const char* commentp) {
    static uint32_t fake_zero_count = 0;
    if (!enable) countp = &fake_zero_count;
    *countp = 0;
    SP_COVER_INSERT(countp,  "filename",filenamep,  "lineno",lineno,  "column",column,
	"hier",string(name())+hierp,  "page",pagep,  "comment",commentp);
}

//--------------------


void Vtop::eval() {
    Vtop__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vtop::eval\n"); );
    int __VclockLoop = 0;
    IData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	VL_DEBUG_IF(VL_PRINTF(" Clock loop\n"););
	vlSymsp->__Vm_activity = true;
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't converge");
    }
}

void Vtop::_eval_initial_loop(Vtop__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    int __VclockLoop = 0;
    IData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	_eval_settle(vlSymsp);
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't DC converge");
    }
}

//--------------------
// Internal Methods

void Vtop::_sequent__TOP__1(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_sequent__TOP__1\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at b07.v:54
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__cont = 0;
	vlTOPp->v__DOT__mar = 0;
	vlTOPp->v__DOT__x = 0;
	vlTOPp->v__DOT__y = 0;
	vlTOPp->v__DOT__t = 0;
	vlTOPp->v__DOT__mem[0] = 1;
	vlTOPp->v__DOT__stato = 0;
	vlTOPp->punti_retta = 0;
	vlTOPp->v__DOT__mem[1] = 0xff;
	vlTOPp->v__DOT__mem[2] = 0;
	vlTOPp->v__DOT__mem[3] = 0;
	vlTOPp->v__DOT__mem[4] = 0;
	vlTOPp->v__DOT__mem[5] = 2;
	vlTOPp->v__DOT__mem[6] = 0;
	vlTOPp->v__DOT__mem[7] = 0;
	vlTOPp->v__DOT__mem[8] = 0;
	vlTOPp->v__DOT__mem[9] = 2;
	vlTOPp->v__DOT__mem[0xa] = 0xff;
	vlTOPp->v__DOT__mem[0xb] = 5;
	vlTOPp->v__DOT__mem[0xc] = 0;
	vlTOPp->v__DOT__mem[0xd] = 2;
	vlTOPp->v__DOT__mem[0xe] = 0;
	vlTOPp->v__DOT__mem[0xf] = 2;
    } else {
	++(vlSymsp->__Vcoverage[19]);
	if ((4 & (IData)(vlTOPp->v__DOT__stato))) {
	    if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
		    ++(vlSymsp->__Vcoverage[18]);
		    vlTOPp->v__DOT__stato = 1;
		} else {
		    ++(vlSymsp->__Vcoverage[17]);
		    if ((0xf != (IData)(vlTOPp->v__DOT__mar))) {
			++(vlSymsp->__Vcoverage[11]);
			if ((2 == (IData)(vlTOPp->v__DOT__x))) {
			    vlTOPp->v__DOT__cont = 
				(0xff & ((IData)(1) 
					 + (IData)(vlTOPp->v__DOT__cont)));
			    vlTOPp->v__DOT__mar = (0xff 
						   & ((IData)(1) 
						      + (IData)(vlTOPp->v__DOT__mar)));
			    ++(vlSymsp->__Vcoverage[9]);
			    vlTOPp->v__DOT__stato = 2;
			} else {
			    vlTOPp->v__DOT__mar = (0xff 
						   & ((IData)(1) 
						      + (IData)(vlTOPp->v__DOT__mar)));
			    ++(vlSymsp->__Vcoverage[10]);
			    vlTOPp->v__DOT__stato = 2;
			}
		    } else {
			++(vlSymsp->__Vcoverage[16]);
			if (vlTOPp->start) {
			    ++(vlSymsp->__Vcoverage[15]);
			    vlTOPp->v__DOT__stato = 6;
			} else {
			    ++(vlSymsp->__Vcoverage[14]);
			    if ((2 == (IData)(vlTOPp->v__DOT__x))) {
				++(vlSymsp->__Vcoverage[12]);
				vlTOPp->v__DOT__stato = 1;
				vlTOPp->punti_retta 
				    = (0xff & ((IData)(1) 
					       + (IData)(vlTOPp->v__DOT__cont)));
			    } else {
				++(vlSymsp->__Vcoverage[13]);
				vlTOPp->v__DOT__stato = 1;
				vlTOPp->punti_retta 
				    = vlTOPp->v__DOT__cont;
			    }
			}
		    }
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
		    vlTOPp->v__DOT__x = (0xff & ((IData)(vlTOPp->v__DOT__x) 
						 + (IData)(vlTOPp->v__DOT__y)));
		    ++(vlSymsp->__Vcoverage[8]);
		    vlTOPp->v__DOT__stato = 6;
		} else {
		    vlTOPp->v__DOT__x = (0xff & ((IData)(vlTOPp->v__DOT__x) 
						 + (IData)(vlTOPp->v__DOT__t)));
		    ++(vlSymsp->__Vcoverage[7]);
		    vlTOPp->v__DOT__y = (IData)(vlTOPp->v__DOT__mem)
			[(0xf & (IData)(vlTOPp->v__DOT__mar))];
		    vlTOPp->v__DOT__stato = 5;
		}
	    }
	} else {
	    if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
		    vlTOPp->v__DOT__mar = (0xff & ((IData)(1) 
						   + (IData)(vlTOPp->v__DOT__mar)));
		    ++(vlSymsp->__Vcoverage[6]);
		    vlTOPp->v__DOT__t = (0xff & ((IData)(vlTOPp->v__DOT__x) 
						 + (IData)(vlTOPp->v__DOT__x)));
		    vlTOPp->v__DOT__stato = 4;
		} else {
		    ++(vlSymsp->__Vcoverage[5]);
		    vlTOPp->v__DOT__x = (IData)(vlTOPp->v__DOT__mem)
			[(0xf & (IData)(vlTOPp->v__DOT__mar))];
		    vlTOPp->v__DOT__stato = 3;
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
		    ++(vlSymsp->__Vcoverage[4]);
		    if (vlTOPp->start) {
			++(vlSymsp->__Vcoverage[2]);
			vlTOPp->v__DOT__cont = 0;
			vlTOPp->v__DOT__mar = 0;
			vlTOPp->v__DOT__stato = 2;
		    } else {
			++(vlSymsp->__Vcoverage[3]);
			vlTOPp->v__DOT__stato = 1;
			vlTOPp->punti_retta = 0;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[1]);
		    vlTOPp->v__DOT__stato = 1;
		}
	    }
	}
    }
}

void Vtop::_eval(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->clock) & (~ (IData)(vlTOPp->__Vclklast__TOP__clock)))) {
	vlTOPp->_sequent__TOP__1(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clock = vlTOPp->clock;
}

void Vtop::_eval_initial(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval_initial\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vtop::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::final\n"); );
    // Variables
    Vtop__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vtop::_eval_settle(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_eval_settle\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

IData Vtop::_change_request(Vtop__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vtop::_change_request\n"); );
    Vtop* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    IData __req = false;  // Logically a bool
    return __req;
}
