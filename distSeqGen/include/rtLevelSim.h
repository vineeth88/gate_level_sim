#ifndef __RTLEVEL_SIM_H__
#define __RTLEVEL_SIM_H__

#include "incl.h"
#include "circuit.h"
#include "verInclude.h"

class rtLevelCkt : public abstrCkt {
	
	Vtop* cktVar;

	public:

	void printInputs();
	void printOutputs();
	void printCurrState();
	void printNextState();
	
	rtLevelCkt(Vtop*);
	~rtLevelCkt() {
		if (cktVar) {
			delete cktVar;
			cktVar = NULL;
		}
	}

	void simOneVector(const vecIn_t&);
	void simOneVector(const int_vec&);

	string getCktState() const;
	string getOutputs() const;
	void setCktState(const cktState&);
	void setCktOutput(string);
	void setVeriObj(Vtop*);
 	const Vtop* getVeriObj() const;

};

#endif	// __RTLEVEL_SIM_H__
