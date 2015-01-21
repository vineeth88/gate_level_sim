#ifndef __GATE_LEVEL_SIM_H__
#define __GATE_LEVEL_SIM_H__ 

#include "incl.h"
#include "circuit.h"

using namespace std;

class gateLevelCkt : public abstrCkt{

	int maxLevels;

	vector<uchar> gtype;
	vector<short> fanin;
	vector<short> fanout;
	int_vec levelNum;
	vector<int_vec> inList;
	vector<int_vec> outList;
	
	/* Can use 2 bit logic (ZERO: 00 | XVAL: 01 | ONES: 11) */
 	//vector<int> value1, value2;
	vector<char> value;		

	int_vec dffList;	// stores gateIds of all FFs
	int_vec stateFFList;

	gateLevelCkt();
	public:
	
	int numGates;
	int numStateFFs;

	gateLevelCkt(char*);
	int eval(int);	
	void simOneVector(const int_vec&);	
	void simOneVector(const vecIn_t&);
//	void simulateVectors();

	string getCktState() const;
	void setCktState(const cktState&);

	void printValues();
	void printInputs();
	void printOutputs();
	void printCurrState();
	void printNextState();
	
	bool checkFanoutCone(int);
};

#endif 	// __GATE_LEVEL_SIM_H__
