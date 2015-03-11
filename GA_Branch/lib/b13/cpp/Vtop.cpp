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
    eoc = VL_RAND_RESET_I(1);
    data_in = VL_RAND_RESET_I(8);
    dsr = VL_RAND_RESET_I(1);
    soc = VL_RAND_RESET_I(1);
    load_dato = VL_RAND_RESET_I(1);
    add_mpx2 = VL_RAND_RESET_I(1);
    canale = VL_RAND_RESET_I(4);
    mux_en = VL_RAND_RESET_I(1);
    error = VL_RAND_RESET_I(1);
    data_out = VL_RAND_RESET_I(1);
    v__DOT__S1 = VL_RAND_RESET_I(3);
    v__DOT__S2 = VL_RAND_RESET_I(2);
    v__DOT__mpx = VL_RAND_RESET_I(1);
    v__DOT__rdy = VL_RAND_RESET_I(1);
    v__DOT__send_data = VL_RAND_RESET_I(1);
    v__DOT__confirm = VL_RAND_RESET_I(1);
    v__DOT__shot = VL_RAND_RESET_I(1);
    v__DOT__send_en = VL_RAND_RESET_I(1);
    v__DOT__tre = VL_RAND_RESET_I(1);
    v__DOT__out_reg = VL_RAND_RESET_I(8);
    v__DOT__next_bit = VL_RAND_RESET_I(4);
    v__DOT__tx_end = VL_RAND_RESET_I(1);
    v__DOT__itfc_state = VL_RAND_RESET_I(2);
    v__DOT__send = VL_RAND_RESET_I(1);
    v__DOT__load = VL_RAND_RESET_I(1);
    v__DOT__tx_conta = VL_RAND_RESET_I(10);
    v__DOT__conta_tmp = VL_RAND_RESET_I(4);
    __Vclklast__TOP__clock = VL_RAND_RESET_I(1);
}

void Vtop::__Vconfigure(Vtop__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    // Coverage Declarations
    __vlCoverInsert(&(vlSymsp->__Vcoverage[0]), first, "b13.v", 72, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[1]), first, "b13.v", 85, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[2]), first, "b13.v", 90, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[3]), first, "b13.v", 94, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[4]), first, "b13.v", 101, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[5]), first, "b13.v", 104, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[6]), first, "b13.v", 99, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[7]), first, "b13.v", 115, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[8]), first, "b13.v", 110, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[9]), first, "b13.v", 120, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[10]), first, "b13.v", 125, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[11]), first, "b13.v", 131, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[12]), first, "b13.v", 134, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[13]), first, "b13.v", 129, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[14]), first, "b13.v", 139, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[15]), first, "b13.v", 83, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[16]), first, "b13.v", 147, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[17]), first, "b13.v", 160, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[18]), first, "b13.v", 166, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[19]), first, "b13.v", 158, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[20]), first, "b13.v", 168, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[21]), first, "b13.v", 175, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[22]), first, "b13.v", 182, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[23]), first, "b13.v", 189, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[24]), first, "b13.v", 181, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[25]), first, "b13.v", 173, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[26]), first, "b13.v", 196, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[27]), first, "b13.v", 200, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[28]), first, "b13.v", 156, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[29]), first, "b13.v", 208, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[30]), first, "b13.v", 220, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[31]), first, "b13.v", 227, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[32]), first, "b13.v", 218, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[33]), first, "b13.v", 232, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[34]), first, "b13.v", 238, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[35]), first, "b13.v", 245, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[36]), first, "b13.v", 243, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[37]), first, "b13.v", 251, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[38]), first, "b13.v", 216, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[39]), first, "b13.v", 259, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[40]), first, "b13.v", 268, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[41]), first, "b13.v", 275, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[42]), first, "b13.v", 282, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[43]), first, "b13.v", 273, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[44]), first, "b13.v", 287, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[45]), first, "b13.v", 290, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[46]), first, "b13.v", 285, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[47]), first, "b13.v", 267, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[48]), first, "b13.v", 300, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[49]), first, "b13.v", 316, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[50]), first, "b13.v", 321, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[51]), first, "b13.v", 326, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[52]), first, "b13.v", 331, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[53]), first, "b13.v", 336, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[54]), first, "b13.v", 341, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[55]), first, "b13.v", 346, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[56]), first, "b13.v", 351, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[57]), first, "b13.v", 356, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[58]), first, "b13.v", 361, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[59]), first, "b13.v", 367, 0, ".v", "v_line/b13", "case");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[60]), first, "b13.v", 313, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[61]), first, "b13.v", 372, 0, ".v", "v_line/b13", "else");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[62]), first, "b13.v", 311, 0, ".v", "v_line/b13", "if");
    __vlCoverInsert(&(vlSymsp->__Vcoverage[63]), first, "b13.v", 308, 0, ".v", "v_line/b13", "else");
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
    // Variables
    VL_SIG8(__Vdly__v__DOT__rdy,0,0);
    VL_SIG8(__Vdly__v__DOT__confirm,0,0);
    VL_SIG8(__Vdly__v__DOT__tx_end,0,0);
    //char	__VpadToAlign7[1];
    // Body
    __Vdly__v__DOT__rdy = vlTOPp->v__DOT__rdy;
    __Vdly__v__DOT__confirm = vlTOPp->v__DOT__confirm;
    __Vdly__v__DOT__tx_end = vlTOPp->v__DOT__tx_end;
    // ALWAYS at b13.v:298
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[48]);
	vlTOPp->v__DOT__next_bit = 0;
	vlTOPp->v__DOT__tx_conta = 0;
	__Vdly__v__DOT__tx_end = 0;
	vlTOPp->data_out = 0;
    } else {
	++(vlSymsp->__Vcoverage[63]);
	__Vdly__v__DOT__tx_end = 0;
	vlTOPp->data_out = 1;
	if (vlTOPp->v__DOT__send_en) {
	    ++(vlSymsp->__Vcoverage[62]);
	    if ((0x68 < (IData)(vlTOPp->v__DOT__tx_conta))) {
		++(vlSymsp->__Vcoverage[60]);
		vlTOPp->v__DOT__tx_conta = 0;
		if (((((((((0 == (IData)(vlTOPp->v__DOT__next_bit)) 
			   | (2 == (IData)(vlTOPp->v__DOT__next_bit))) 
			  | (3 == (IData)(vlTOPp->v__DOT__next_bit))) 
			 | (4 == (IData)(vlTOPp->v__DOT__next_bit))) 
			| (5 == (IData)(vlTOPp->v__DOT__next_bit))) 
		       | (6 == (IData)(vlTOPp->v__DOT__next_bit))) 
		      | (7 == (IData)(vlTOPp->v__DOT__next_bit))) 
		     | (8 == (IData)(vlTOPp->v__DOT__next_bit)))) {
		    if ((0 == (IData)(vlTOPp->v__DOT__next_bit))) {
			++(vlSymsp->__Vcoverage[49]);
			vlTOPp->v__DOT__next_bit = 2;
			vlTOPp->data_out = 0;
		    } else {
			if ((2 == (IData)(vlTOPp->v__DOT__next_bit))) {
			    ++(vlSymsp->__Vcoverage[50]);
			    vlTOPp->v__DOT__next_bit = 3;
			    vlTOPp->data_out = (1 & 
						((IData)(vlTOPp->v__DOT__out_reg) 
						 >> 7));
			} else {
			    if ((3 == (IData)(vlTOPp->v__DOT__next_bit))) {
				++(vlSymsp->__Vcoverage[51]);
				vlTOPp->v__DOT__next_bit = 4;
				vlTOPp->data_out = 
				    (1 & ((IData)(vlTOPp->v__DOT__out_reg) 
					  >> 6));
			    } else {
				if ((4 == (IData)(vlTOPp->v__DOT__next_bit))) {
				    ++(vlSymsp->__Vcoverage[52]);
				    vlTOPp->v__DOT__next_bit = 5;
				    vlTOPp->data_out 
					= (1 & ((IData)(vlTOPp->v__DOT__out_reg) 
						>> 5));
				} else {
				    if ((5 == (IData)(vlTOPp->v__DOT__next_bit))) {
					++(vlSymsp->__Vcoverage[53]);
					vlTOPp->v__DOT__next_bit = 6;
					vlTOPp->data_out 
					    = (1 & 
					       ((IData)(vlTOPp->v__DOT__out_reg) 
						>> 4));
				    } else {
					if ((6 == (IData)(vlTOPp->v__DOT__next_bit))) {
					    ++(vlSymsp->__Vcoverage[54]);
					    vlTOPp->v__DOT__next_bit = 7;
					    vlTOPp->data_out 
						= (1 
						   & ((IData)(vlTOPp->v__DOT__out_reg) 
						      >> 3));
					} else {
					    if ((7 
						 == (IData)(vlTOPp->v__DOT__next_bit))) {
						++(vlSymsp->__Vcoverage[55]);
						vlTOPp->v__DOT__next_bit = 8;
						vlTOPp->data_out 
						    = 
						    (1 
						     & ((IData)(vlTOPp->v__DOT__out_reg) 
							>> 2));
					    } else {
						++(vlSymsp->__Vcoverage[56]);
						vlTOPp->v__DOT__next_bit = 9;
						vlTOPp->data_out 
						    = 
						    (1 
						     & ((IData)(vlTOPp->v__DOT__out_reg) 
							>> 1));
					    }
					}
				    }
				}
			    }
			}
		    }
		} else {
		    if ((9 == (IData)(vlTOPp->v__DOT__next_bit))) {
			++(vlSymsp->__Vcoverage[57]);
			vlTOPp->v__DOT__next_bit = 1;
			vlTOPp->data_out = (1 & (IData)(vlTOPp->v__DOT__out_reg));
		    } else {
			if ((1 == (IData)(vlTOPp->v__DOT__next_bit))) {
			    ++(vlSymsp->__Vcoverage[58]);
			    vlTOPp->v__DOT__next_bit = 0;
			    vlTOPp->data_out = 1;
			    __Vdly__v__DOT__tx_end = 1;
			} else {
			    ++(vlSymsp->__Vcoverage[59]);
			}
		    }
		}
	    } else {
		vlTOPp->v__DOT__tx_conta = (0x3ff & 
					    ((IData)(1) 
					     + (IData)(vlTOPp->v__DOT__tx_conta)));
		++(vlSymsp->__Vcoverage[61]);
	    }
	}
    }
    // ALWAYS at b13.v:257
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[39]);
	vlTOPp->v__DOT__tre = 0;
	vlTOPp->v__DOT__send_en = 0;
	vlTOPp->v__DOT__out_reg = 0;
	vlTOPp->error = 0;
    } else {
	++(vlSymsp->__Vcoverage[47]);
	if (vlTOPp->v__DOT__tx_end) {
	    ++(vlSymsp->__Vcoverage[40]);
	    vlTOPp->v__DOT__tre = 1;
	    vlTOPp->v__DOT__send_en = 0;
	}
	if (vlTOPp->v__DOT__load) {
	    ++(vlSymsp->__Vcoverage[43]);
	    if (vlTOPp->v__DOT__tre) {
		++(vlSymsp->__Vcoverage[42]);
		vlTOPp->error = 1;
	    } else {
		++(vlSymsp->__Vcoverage[41]);
		vlTOPp->v__DOT__tre = 1;
		vlTOPp->v__DOT__out_reg = vlTOPp->data_in;
		vlTOPp->error = 0;
	    }
	}
	if (vlTOPp->v__DOT__send) {
	    ++(vlSymsp->__Vcoverage[46]);
	    if ((1 & ((~ (IData)(vlTOPp->v__DOT__tre)) 
		      | (~ (IData)(vlTOPp->dsr))))) {
		++(vlSymsp->__Vcoverage[44]);
		vlTOPp->error = 1;
	    } else {
		++(vlSymsp->__Vcoverage[45]);
		vlTOPp->error = 0;
		vlTOPp->v__DOT__send_en = 1;
	    }
	}
    }
    // ALWAYS at b13.v:206
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[29]);
	vlTOPp->v__DOT__itfc_state = 0;
	vlTOPp->v__DOT__load = 0;
	vlTOPp->v__DOT__send = 0;
	__Vdly__v__DOT__confirm = 0;
    } else {
	++(vlSymsp->__Vcoverage[38]);
	if ((0 == (IData)(vlTOPp->v__DOT__itfc_state))) {
	    ++(vlSymsp->__Vcoverage[32]);
	    if (vlTOPp->v__DOT__shot) {
		++(vlSymsp->__Vcoverage[30]);
		vlTOPp->v__DOT__itfc_state = 1;
		vlTOPp->v__DOT__load = 1;
		__Vdly__v__DOT__confirm = 0;
	    } else {
		++(vlSymsp->__Vcoverage[31]);
		vlTOPp->v__DOT__itfc_state = 0;
		__Vdly__v__DOT__confirm = 0;
	    }
	} else {
	    if ((1 == (IData)(vlTOPp->v__DOT__itfc_state))) {
		++(vlSymsp->__Vcoverage[33]);
		vlTOPp->v__DOT__itfc_state = 2;
		vlTOPp->v__DOT__load = 0;
		vlTOPp->v__DOT__send = 1;
	    } else {
		if ((2 == (IData)(vlTOPp->v__DOT__itfc_state))) {
		    ++(vlSymsp->__Vcoverage[34]);
		    vlTOPp->v__DOT__itfc_state = 3;
		    vlTOPp->v__DOT__send = 0;
		} else {
		    if ((3 == (IData)(vlTOPp->v__DOT__itfc_state))) {
			++(vlSymsp->__Vcoverage[36]);
			if (vlTOPp->v__DOT__tx_end) {
			    ++(vlSymsp->__Vcoverage[35]);
			    vlTOPp->v__DOT__itfc_state = 0;
			    __Vdly__v__DOT__confirm = 1;
			}
		    } else {
			++(vlSymsp->__Vcoverage[37]);
		    }
		}
	    }
	}
    }
    vlTOPp->v__DOT__tx_end = __Vdly__v__DOT__tx_end;
    // ALWAYS at b13.v:145
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[16]);
	vlTOPp->v__DOT__S2 = 0;
	vlTOPp->v__DOT__mpx = 0;
	__Vdly__v__DOT__rdy = 0;
	vlTOPp->add_mpx2 = 0;
	vlTOPp->v__DOT__shot = 0;
    } else {
	++(vlSymsp->__Vcoverage[28]);
	if ((0 == (IData)(vlTOPp->v__DOT__S2))) {
	    ++(vlSymsp->__Vcoverage[19]);
	    if (vlTOPp->v__DOT__send_data) {
		++(vlSymsp->__Vcoverage[17]);
		vlTOPp->v__DOT__S2 = 1;
		__Vdly__v__DOT__rdy = 1;
	    } else {
		++(vlSymsp->__Vcoverage[18]);
		vlTOPp->v__DOT__S2 = 0;
	    }
	} else {
	    if ((1 == (IData)(vlTOPp->v__DOT__S2))) {
		++(vlSymsp->__Vcoverage[20]);
		vlTOPp->v__DOT__S2 = 2;
		vlTOPp->v__DOT__shot = 1;
	    } else {
		if ((2 == (IData)(vlTOPp->v__DOT__S2))) {
		    ++(vlSymsp->__Vcoverage[25]);
		    if (vlTOPp->v__DOT__confirm) {
			++(vlSymsp->__Vcoverage[24]);
			if (vlTOPp->v__DOT__mpx) {
			    ++(vlSymsp->__Vcoverage[23]);
			    vlTOPp->v__DOT__mpx = 0;
			    vlTOPp->v__DOT__S2 = 3;
			    __Vdly__v__DOT__rdy = 0;
			} else {
			    ++(vlSymsp->__Vcoverage[22]);
			    vlTOPp->v__DOT__mpx = 1;
			    vlTOPp->v__DOT__S2 = 1;
			    vlTOPp->add_mpx2 = 1;
			}
		    } else {
			++(vlSymsp->__Vcoverage[21]);
			vlTOPp->v__DOT__S2 = 2;
			vlTOPp->v__DOT__shot = 0;
		    }
		} else {
		    if ((3 == (IData)(vlTOPp->v__DOT__S2))) {
			++(vlSymsp->__Vcoverage[26]);
			vlTOPp->v__DOT__S2 = 0;
		    } else {
			++(vlSymsp->__Vcoverage[27]);
		    }
		}
	    }
	}
    }
    vlTOPp->v__DOT__confirm = __Vdly__v__DOT__confirm;
    // ALWAYS at b13.v:70
    if (vlTOPp->reset) {
	++(vlSymsp->__Vcoverage[0]);
	vlTOPp->v__DOT__S1 = 0;
	vlTOPp->v__DOT__conta_tmp = 0;
	vlTOPp->soc = 0;
	vlTOPp->v__DOT__send_data = 0;
	vlTOPp->load_dato = 0;
	vlTOPp->mux_en = 0;
	vlTOPp->canale = 0;
    } else {
	++(vlSymsp->__Vcoverage[15]);
	if ((4 & (IData)(vlTOPp->v__DOT__S1))) {
	    if ((2 & (IData)(vlTOPp->v__DOT__S1))) {
		if ((1 & (IData)(vlTOPp->v__DOT__S1))) {
		    ++(vlSymsp->__Vcoverage[9]);
		    vlTOPp->v__DOT__S1 = 4;
		    vlTOPp->v__DOT__send_data = 1;
		} else {
		    ++(vlSymsp->__Vcoverage[8]);
		    vlTOPp->v__DOT__S1 = 7;
		    vlTOPp->load_dato = 0;
		    vlTOPp->soc = 0;
		    vlTOPp->v__DOT__conta_tmp = (0xf 
						 & ((IData)(1) 
						    + (IData)(vlTOPp->v__DOT__conta_tmp)));
		    if ((8 == (IData)(vlTOPp->v__DOT__conta_tmp))) {
			++(vlSymsp->__Vcoverage[7]);
			vlTOPp->v__DOT__conta_tmp = 0;
		    }
		    vlTOPp->canale = vlTOPp->v__DOT__conta_tmp;
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__S1))) {
		    ++(vlSymsp->__Vcoverage[6]);
		    if (vlTOPp->eoc) {
			++(vlSymsp->__Vcoverage[4]);
			vlTOPp->v__DOT__S1 = 5;
		    } else {
			++(vlSymsp->__Vcoverage[5]);
			vlTOPp->v__DOT__S1 = 6;
			vlTOPp->load_dato = 1;
			vlTOPp->mux_en = 0;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[10]);
		    vlTOPp->v__DOT__S1 = 3;
		}
	    }
	} else {
	    if ((2 & (IData)(vlTOPp->v__DOT__S1))) {
		if ((1 & (IData)(vlTOPp->v__DOT__S1))) {
		    ++(vlSymsp->__Vcoverage[13]);
		    if (vlTOPp->v__DOT__rdy) {
			++(vlSymsp->__Vcoverage[12]);
			vlTOPp->v__DOT__S1 = 0;
			vlTOPp->v__DOT__send_data = 0;
		    } else {
			++(vlSymsp->__Vcoverage[11]);
			vlTOPp->v__DOT__S1 = 3;
		    }
		} else {
		    ++(vlSymsp->__Vcoverage[3]);
		    vlTOPp->v__DOT__S1 = 5;
		    vlTOPp->soc = 1;
		}
	    } else {
		if ((1 & (IData)(vlTOPp->v__DOT__S1))) {
		    ++(vlSymsp->__Vcoverage[2]);
		    vlTOPp->v__DOT__S1 = 2;
		} else {
		    ++(vlSymsp->__Vcoverage[1]);
		    vlTOPp->v__DOT__S1 = 1;
		    vlTOPp->mux_en = 1;
		}
	    }
	}
    }
    vlTOPp->v__DOT__rdy = __Vdly__v__DOT__rdy;
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
