#ifndef __RTLEVEL_SIM_H__
#define __RTLEVEL_SIM_H__

#include "incl.h"
#include "circuit.h"
#include "verInclude.h"

class rtLevelCkt : public abstrCkt {
	
	Vtop* cktVar;
	cktState* prevState;
	cktState* currState;

	public:

	void printInputs();
	void printOutputs();
	void printCurrState();
	void printNextState();
	
	rtLevelCkt(Vtop*);
	~rtLevelCkt() {
		delete cktVar;
		delete prevState;
		delete currState;
		cktVar = NULL;
		prevState = currState = NULL;
	}

	void simOneVector(const vecIn_t&);
	void simOneVector(const int_vec&);

	string getCktState() const;
	void setCktState(const cktState&);
	void setVeriObj(Vtop*);
 	const Vtop* getVeriObj() const;

};

#endif	// __RTLEVEL_SIM_H__
