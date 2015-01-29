#ifndef __RTLEVEL_SIM_H__
#define __RTLEVEL_SIM_H__

#include "incl.h"
#include "circuit.h"
#include "verInclude.h"

class rtLevelCkt : public abstrCkt {
	
	Vtop 		*cktVar;

	void 		printCurrState();
	void 		printNextState();

	public:

	int_vec 	startIdxVec;
	int_vec		sizeVec;

	rtLevelCkt(Vtop*);
	void		readSizeVars();
	void		setVeriObj(Vtop*);
 	const Vtop*	getVeriObj() const;
	void 		resetCounters();
	void 		getCoverage(int_vec&) const;
	int  		getBranchCounters(int_vec&) const;

	void 		simOneVector(const vecIn_t&);
	void		simMultiVector(const vecIn_t&);
	void		simNRandomVec(int);

	/* {ckt}Int.cpp */
	void 		printInputs();
	void 		printOutputs();
	
	string 		getCktState() const;
	string 		getOutputs() const;
	void 		setCktState(const state_t*);
	void 		setCktState(const string&);
	void 		setCktOutput(string);

	~rtLevelCkt() {
		if (cktVar) {
			delete cktVar;
			cktVar = NULL;
		}
	}
};

void RandomVecIn(vecIn_t&, int);
	
#endif	// __RTLEVEL_SIM_H__
