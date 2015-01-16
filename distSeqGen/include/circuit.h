#ifndef __CIRCUIT_H__
#define __CIRCUIT_H__

#include "incl.h"

using namespace std;

// Enum and typedefs
enum {
	ONES = 0xffffffff,
	ZERO = 0x0,
	XVAL = 40
//	DVAL = 20  // 'D' - 48
//	DBAR = -20 // 'd' - 48
};

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned long long int ullint;

typedef std::string vecIn_t;

typedef vector<char> ch_vec;
typedef vector<char>::iterator ch_vec_iter;
typedef vector<int> int_vec;
typedef vector<int>::iterator int_vec_iter;

typedef set<char> ch_set;
typedef set<char>::iterator ch_set_iter;
typedef set<int> int_set;
typedef set<int>::iterator int_set_iter;

class abstrCkt;
class rtLevelCkt;
class gateLevelCkt;
class cktState;

class abstrCkt {
	
	public:

	char cktName[80];

	int numInputs;
	int numOutputs;
	int numFFs;

	void readVecFile(vector<int_vec>&);
	void readVecFile(vector<vecIn_t>&);

	/* Simulation */
	virtual void simOneVector(const vecIn_t&) = 0;
	virtual void simOneVector(const int_vec&) = 0;
	
//	virtual simMultiVector(const vector<vecIn_t>&) = 0;
//	virtual simMultiVector(const vector<int_vec>&) = 0;

	/* Print functions */
	virtual void printInputs() = 0;
	virtual void printOutputs() = 0;
	virtual void printCurrState() = 0;
	virtual void printNextState() = 0;
	
	private:
	abstrCkt& operator=(const abstrCkt&);
//	abstrCkt(const abstrCkt&);

};

/*	-------------------------------------------------------
	class cktState
 	Contains the circuit state, modeled as a string
	-------------------------------------------------------	*/

class cktState {
	
	string stateVal;
	int stateIdx;
	
	public:
	
	cktState(const cktState&);
	cktState(const string&);
	cktState(const char[], int);
	cktState(const rtLevelCkt*, int);
	cktState(const gateLevelCkt*, int);
	
	bool operator[](int) const;
	bool operator==(const cktState&) const;
	string getState() const;
	int getIdx() const {	return stateIdx; }
	void setState(const string&);
};

#endif // __CIRCUIT_H__
