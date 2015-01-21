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
    r_button = VL_RAND_RESET_I(1);
    g_button = VL_RAND_RESET_I(1);
    key = VL_RAND_RESET_I(1);
    start = VL_RAND_RESET_I(1);
    reset = VL_RAND_RESET_I(1);
    test = VL_RAND_RESET_I(1);
    rts = VL_RAND_RESET_I(1);
    rtr = VL_RAND_RESET_I(1);
    clock = VL_RAND_RESET_I(1);
    v_in = VL_RAND_RESET_I(4);
    cts = VL_RAND_RESET_I(1);
    ctr = VL_RAND_RESET_I(1);
    v_out = VL_RAND_RESET_I(4);
    v__DOT__stato = VL_RAND_RESET_I(4);
    v__DOT__voto0 = VL_RAND_RESET_I(1);
    v__DOT__voto1 = VL_RAND_RESET_I(1);
    v__DOT__voto2 = VL_RAND_RESET_I(1);
    v__DOT__voto3 = VL_RAND_RESET_I(1);
    v__DOT__sign = VL_RAND_RESET_I(4);
    v__DOT__last_g = VL_RAND_RESET_I(1);
    v__DOT__last_r = VL_RAND_RESET_I(1);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "b10.v", 87, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "b10.v", 112, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "b10.v", 118, 0, ".v", "v_line/b10", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "b10.v", 104, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "b10.v", 128, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "b10.v", 136, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "b10.v", 138, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "b10.v", 126, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "b10.v", 143, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "b10.v", 150, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "b10.v", 154, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "b10.v", 147, 0, ".v", "v_line/b10", "elsif");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "b10.v", 162, 0, ".v", "v_line/b10", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "b10.v", 141, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "b10.v", 169, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "b10.v", 184, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "b10.v", 189, 0, ".v", "v_line/b10", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "b10.v", 177, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "b10.v", 175, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "b10.v", 196, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "b10.v", 194, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "b10.v", 204, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "b10.v", 202, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "b10.v", 216, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[24]), first, "b10.v", 214, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[25]), first, "b10.v", 224, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[26]), first, "b10.v", 222, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[27]), first, "b10.v", 237, 0, ".v", "v_line/b10", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[28]), first, "b10.v", 230, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[29]), first, "b10.v", 242, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[30]), first, "b10.v", 251, 0, ".v", "v_line/b10", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[31]), first, "b10.v", 102, 0, ".v", "v_line/b10", "else");
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
    // ALWAYS at b10.v:85
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__stato = 0;
	vlTOPp->v__DOT__voto0 = 0;
	vlTOPp->v__DOT__voto1 = 0;
	vlTOPp->v__DOT__voto2 = 0;
	vlTOPp->v__DOT__voto3 = 0;
	vlTOPp->v__DOT__sign = 0;
	vlTOPp->v__DOT__last_g = 0;
	vlTOPp->v__DOT__last_r = 0;
	vlTOPp->cts = 0;
	vlTOPp->ctr = 0;
	vlTOPp->v_out = 0;
    } else {
	++(vlSymsp->__Vcoverage[31]);
	if ((8 & (IData)(vlTOPp->v__DOT__stato))) {
	    if ((4 & (IData)(vlTOPp->v__DOT__stato))) {
		++(vlSymsp->__Vcoverage[30]);
		vlTOPp->v__DOT__stato = 0;
	    } else {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[30]);
			vlTOPp->v__DOT__stato = 0;
		    } else {
			++(vlSymsp->__Vcoverage[29]);
			vlTOPp->v__DOT__voto0 = (1 
						 & (~ 
						    ((IData)(vlTOPp->v__DOT__sign) 
						     >> 3)));
			vlTOPp->v__DOT__stato = 4;
		    }
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[28]);
			vlTOPp->v__DOT__voto0 = (1 
						 & (IData)(vlTOPp->v_in));
			vlTOPp->v__DOT__sign = 8;
			vlTOPp->v__DOT__voto1 = (1 
						 & ((IData)(vlTOPp->v_in) 
						    >> 1));
			vlTOPp->v__DOT__voto2 = (1 
						 & ((IData)(vlTOPp->v_in) 
						    >> 2));
			vlTOPp->v__DOT__voto3 = (1 
						 & ((IData)(vlTOPp->v_in) 
						    >> 3));
			if (((((IData)(vlTOPp->v__DOT__voto0) 
			       & (IData)(vlTOPp->v__DOT__voto1)) 
			      & (IData)(vlTOPp->v__DOT__voto2)) 
			     & (IData)(vlTOPp->v__DOT__voto3))) {
			    ++(vlSymsp->__Vcoverage[27]);
			    vlTOPp->v__DOT__stato = 0xa;
			}
		    } else {
			++(vlSymsp->__Vcoverage[26]);
			if ((1 & (~ (IData)(vlTOPp->rtr)))) {
			    ++(vlSymsp->__Vcoverage[25]);
			    vlTOPp->v__DOT__stato = 1;
			    vlTOPp->cts = 0;
			}
		    }
		}
	    }
	} else {
	    if ((4 & (IData)(vlTOPp->v__DOT__stato))) {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[24]);
			if ((1 & (~ (IData)(vlTOPp->rtr)))) {
			    ++(vlSymsp->__Vcoverage[23]);
			    vlTOPp->v__DOT__stato = 4;
			    vlTOPp->cts = 0;
			}
		    } else {
			++(vlSymsp->__Vcoverage[22]);
			if (vlTOPp->rts) {
			    ++(vlSymsp->__Vcoverage[21]);
			    vlTOPp->v__DOT__voto0 = 
				(1 & (IData)(vlTOPp->v_in));
			    vlTOPp->v__DOT__stato = 7;
			    vlTOPp->v__DOT__voto1 = 
				(1 & ((IData)(vlTOPp->v_in) 
				      >> 1));
			    vlTOPp->v__DOT__voto2 = 
				(1 & ((IData)(vlTOPp->v_in) 
				      >> 2));
			    vlTOPp->v__DOT__voto3 = 
				(1 & ((IData)(vlTOPp->v_in) 
				      >> 3));
			    vlTOPp->ctr = 0;
			}
		    }
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[20]);
			if ((1 & (~ (IData)(vlTOPp->rts)))) {
			    ++(vlSymsp->__Vcoverage[19]);
			    vlTOPp->v__DOT__stato = 6;
			    vlTOPp->ctr = 1;
			}
		    } else {
			++(vlSymsp->__Vcoverage[18]);
			if (vlTOPp->rtr) {
			    ++(vlSymsp->__Vcoverage[17]);
			    vlTOPp->v_out = ((0xe & (IData)(vlTOPp->v_out)) 
					     | (IData)(vlTOPp->v__DOT__voto0));
			    vlTOPp->v_out = ((0xd & (IData)(vlTOPp->v_out)) 
					     | ((IData)(vlTOPp->v__DOT__voto1) 
						<< 1));
			    vlTOPp->v_out = ((0xb & (IData)(vlTOPp->v_out)) 
					     | ((IData)(vlTOPp->v__DOT__voto2) 
						<< 2));
			    vlTOPp->v_out = ((7 & (IData)(vlTOPp->v_out)) 
					     | ((IData)(vlTOPp->v__DOT__voto3) 
						<< 3));
			    vlTOPp->cts = 1;
			    if (((((~ (IData)(vlTOPp->v__DOT__voto0)) 
				   & (IData)(vlTOPp->v__DOT__voto1)) 
				  & (IData)(vlTOPp->v__DOT__voto2)) 
				 & (~ (IData)(vlTOPp->v__DOT__voto3)))) {
				++(vlSymsp->__Vcoverage[15]);
				vlTOPp->v__DOT__stato = 8;
			    } else {
				++(vlSymsp->__Vcoverage[16]);
				vlTOPp->v__DOT__stato = 5;
			    }
			}
		    }
		}
	    } else {
		if ((2 & (IData)(vlTOPp->v__DOT__stato))) {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[14]);
			vlTOPp->v__DOT__voto3 = ((IData)(vlTOPp->v__DOT__voto0) 
						 ^ 
						 ((IData)(vlTOPp->v__DOT__voto1) 
						  ^ (IData)(vlTOPp->v__DOT__voto2)));
			vlTOPp->v__DOT__stato = 4;
			vlTOPp->v__DOT__voto0 = 0;
		    } else {
			++(vlSymsp->__Vcoverage[13]);
			if (vlTOPp->start) {
			    if (vlTOPp->key) {
				++(vlSymsp->__Vcoverage[11]);
				vlTOPp->v__DOT__voto0 
				    = vlTOPp->key;
				if ((((IData)(vlTOPp->g_button) 
				      ^ (IData)(vlTOPp->v__DOT__last_g)) 
				     & (IData)(vlTOPp->g_button))) {
				    vlTOPp->v__DOT__voto1 
					= (1 & (~ (IData)(vlTOPp->v__DOT__voto1)));
				    ++(vlSymsp->__Vcoverage[9]);
				}
				if ((((IData)(vlTOPp->r_button) 
				      ^ (IData)(vlTOPp->v__DOT__last_r)) 
				     & (IData)(vlTOPp->r_button))) {
				    vlTOPp->v__DOT__voto2 
					= (1 & (~ (IData)(vlTOPp->v__DOT__voto2)));
				    ++(vlSymsp->__Vcoverage[10]);
				}
				vlTOPp->v__DOT__last_g 
				    = vlTOPp->g_button;
				vlTOPp->v__DOT__last_r 
				    = vlTOPp->r_button;
			    } else {
				++(vlSymsp->__Vcoverage[12]);
				vlTOPp->v__DOT__voto0 = 0;
				vlTOPp->v__DOT__voto1 = 0;
				vlTOPp->v__DOT__voto2 = 0;
				vlTOPp->v__DOT__voto3 = 0;
			    }
			} else {
			    ++(vlSymsp->__Vcoverage[8]);
			    vlTOPp->v__DOT__stato = 3;
			}
		    }
		} else {
		    if ((1 & (IData)(vlTOPp->v__DOT__stato))) {
			++(vlSymsp->__Vcoverage[7]);
			if (vlTOPp->start) {
			    ++(vlSymsp->__Vcoverage[4]);
			    vlTOPp->v__DOT__voto0 = 0;
			    vlTOPp->v__DOT__voto1 = 0;
			    vlTOPp->v__DOT__voto2 = 0;
			    vlTOPp->v__DOT__voto3 = 0;
			    vlTOPp->v__DOT__stato = 2;
			}
			if (vlTOPp->rtr) {
			    ++(vlSymsp->__Vcoverage[5]);
			    vlTOPp->cts = 1;
			}
			if ((1 & (~ (IData)(vlTOPp->rtr)))) {
			    ++(vlSymsp->__Vcoverage[6]);
			    vlTOPp->cts = 0;
			}
		    } else {
			++(vlSymsp->__Vcoverage[3]);
			vlTOPp->v__DOT__voto0 = 0;
			vlTOPp->v__DOT__voto1 = 0;
			vlTOPp->v__DOT__voto2 = 0;
			vlTOPp->v__DOT__voto3 = 0;
			vlTOPp->cts = 0;
			vlTOPp->ctr = 0;
			if (vlTOPp->test) {
			    ++(vlSymsp->__Vcoverage[2]);
			    vlTOPp->v__DOT__voto0 = 0;
			    vlTOPp->v__DOT__voto1 = 0;
			    vlTOPp->v__DOT__voto2 = 0;
			    vlTOPp->v__DOT__voto3 = 0;
			    vlTOPp->v__DOT__stato = 1;
			} else {
			    ++(vlSymsp->__Vcoverage[1]);
			    vlTOPp->v__DOT__sign = 0;
			    vlTOPp->v__DOT__stato = 9;
			}
		    }
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
