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
    eql = VL_RAND_RESET_I(1);
    cont_eql = VL_RAND_RESET_I(1);
    cc_mux = VL_RAND_RESET_I(2);
    uscite = VL_RAND_RESET_I(2);
    enable_count = VL_RAND_RESET_I(1);
    ackout = VL_RAND_RESET_I(1);
    v__DOT__curr_state = VL_RAND_RESET_I(3);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "b06.v", 55, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "b06.v", 65, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "b06.v", 71, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "b06.v", 77, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "b06.v", 86, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "b06.v", 93, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "b06.v", 84, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "b06.v", 102, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "b06.v", 109, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "b06.v", 100, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "b06.v", 118, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "b06.v", 125, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "b06.v", 116, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "b06.v", 136, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "b06.v", 143, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "b06.v", 134, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "b06.v", 152, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "b06.v", 159, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "b06.v", 150, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "b06.v", 168, 0, ".v", "v_line/b06", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "b06.v", 175, 0, ".v", "v_line/b06", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "b06.v", 166, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "b06.v", 182, 0, ".v", "v_line/b06", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "b06.v", 64, 0, ".v", "v_line/b06", "else");
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
    // ALWAYS at b06.v:53
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__curr_state = 0;
	vlTOPp->cc_mux = 0;
	vlTOPp->enable_count = 0;
	vlTOPp->ackout = 0;
	vlTOPp->uscite = 0;
    } else {
	++(vlSymsp->__Vcoverage[23]);
	if (vlTOPp->cont_eql) {
	    ++(vlSymsp->__Vcoverage[1]);
	    vlTOPp->ackout = 0;
	    vlTOPp->enable_count = 0;
	} else {
	    ++(vlSymsp->__Vcoverage[2]);
	    vlTOPp->ackout = 1;
	    vlTOPp->enable_count = 1;
	}
	if ((4 & (IData)(vlTOPp->v__DOT__curr_state))) {
	    if ((2 & (IData)(vlTOPp->v__DOT__curr_state))) {
		if ((1 & (IData)(vlTOPp->v__DOT__curr_state))) {
		    ++(vlSymsp->__Vcoverage[22]);
		    vlTOPp->v__DOT__curr_state = 0;
		} else {
		    ++(vlSymsp->__Vcoverage[21]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[19]);
			vlTOPp->v__DOT__curr_state = 6;
			vlTOPp->uscite = 3;
			vlTOPp->cc_mux = 2;
		    } else {
			++(vlSymsp->__Vcoverage[20]);
			vlTOPp->v__DOT__curr_state = 1;
			vlTOPp->uscite = 1;
			vlTOPp->cc_mux = 1;
		    }
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__curr_state))) {
		    ++(vlSymsp->__Vcoverage[9]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[7]);
			vlTOPp->v__DOT__curr_state = 4;
			vlTOPp->uscite = 0;
			vlTOPp->cc_mux = 3;
		    } else {
			++(vlSymsp->__Vcoverage[8]);
			vlTOPp->v__DOT__curr_state = 1;
			vlTOPp->uscite = 1;
			vlTOPp->cc_mux = 1;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[18]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[16]);
			vlTOPp->v__DOT__curr_state = 4;
			vlTOPp->uscite = 0;
			vlTOPp->cc_mux = 3;
		    } else {
			++(vlSymsp->__Vcoverage[17]);
			vlTOPp->v__DOT__curr_state = 6;
			vlTOPp->uscite = 3;
			vlTOPp->cc_mux = 2;
		    }
		}
	    }
	} else {
	    if ((2 & (IData)(vlTOPp->v__DOT__curr_state))) {
		if ((1 & (IData)(vlTOPp->v__DOT__curr_state))) {
		    ++(vlSymsp->__Vcoverage[15]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[13]);
			vlTOPp->v__DOT__curr_state = 3;
			vlTOPp->uscite = 1;
			vlTOPp->cc_mux = 1;
		    } else {
			++(vlSymsp->__Vcoverage[14]);
			vlTOPp->v__DOT__curr_state = 1;
			vlTOPp->uscite = 1;
			vlTOPp->cc_mux = 1;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[12]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[10]);
			vlTOPp->v__DOT__curr_state = 2;
			vlTOPp->uscite = 0;
			vlTOPp->cc_mux = 3;
		    } else {
			++(vlSymsp->__Vcoverage[11]);
			vlTOPp->v__DOT__curr_state = 3;
			vlTOPp->uscite = 1;
			vlTOPp->ackout = 1;
			vlTOPp->enable_count = 1;
			vlTOPp->cc_mux = 1;
		    }
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__curr_state))) {
		    ++(vlSymsp->__Vcoverage[6]);
		    if (vlTOPp->eql) {
			++(vlSymsp->__Vcoverage[4]);
			vlTOPp->v__DOT__curr_state = 2;
			vlTOPp->uscite = 0;
			vlTOPp->cc_mux = 3;
		    } else {
			++(vlSymsp->__Vcoverage[5]);
			vlTOPp->v__DOT__curr_state = 5;
			vlTOPp->uscite = 1;
			vlTOPp->cc_mux = 2;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[3]);
		    vlTOPp->v__DOT__curr_state = 1;
		    vlTOPp->cc_mux = 1;
		    vlTOPp->uscite = 1;
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
