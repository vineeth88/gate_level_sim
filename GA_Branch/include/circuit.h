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

class abstrCkt;
class rtLevelCkt;
class gateLevelCkt;
// class cktState;

class state_t;
class gaIndiv_t;
class gaPopulation_t;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long int ulint;
typedef unsigned long long int ullint;

typedef std::string vecIn_t;
typedef std::string keyVal_t;

typedef vector<char> ch_vec;
typedef vector<char>::iterator ch_vec_iter;
typedef vector<int> int_vec;
typedef vector<int>::iterator int_vec_iter;

typedef set<char> ch_set;
typedef set<char>::iterator ch_set_iter;
typedef set<int> int_set;
typedef set<int>::iterator int_set_iter;

typedef map<keyVal_t, state_t*> stateMap_t;
typedef stateMap_t::iterator stateMap_iter;
typedef pair<stateMap_iter, bool> retVal_t;

typedef vector<state_t*> state_pVec;
typedef state_pVec::iterator state_pVec_iter;
typedef vector<gaIndiv_t*> gaIndiv_pVec;
typedef vector<gaIndiv_t*>::iterator gaIndiv_pVec_iter;

typedef int fitness_t;

typedef vector<int> int_vec;
typedef vector<int>::iterator int_vec_iter;

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
//	virtual void simOneVector(const int_vec&) = 0;
	
	/* Print functions */
	virtual void printInputs() = 0;
	virtual void printOutputs() = 0;
	virtual void printCurrState() = 0;
	virtual void printNextState() = 0;
	
	private:
	abstrCkt& operator=(const abstrCkt&);

};

// ==================================================================
//	class cktState
// 	Contains the circuit state, modeled as a string
// ==================================================================

//class cktState {
//	
//	string 		stateVal;
//	int 		stateIdx;
//	int 		hitCount;
//	fitness_t 	stateFitness;
//	int_vec 	branchIndices;
//
//	public:
//	
//	cktState(const cktState&);
//	cktState(const string&);
//	cktState(const char[], int);
//	cktState(const rtLevelCkt*, int);
//	cktState(const gateLevelCkt*, int);
//	
//	bool operator[](int) const;
//	bool operator==(const cktState&) const;
//
//	string getState() const;
//	int getIdx() const { 
//		return stateIdx; 
//	}
//	int getHitCount() const { 
//		return hitCount;
//	}
//	fitness_t getFitness() const {
//		return stateFitness;
//	}
//	void getBranchIndex(int_vec& branch_idx) {
//		branch_idx = branchIndices;
//	}
//
//	void setState(const string&);
//	void setFitness(fitness_t fitness_val) {
//		stateFitness = fitness_val;
//	}
//
//	keyVal_t getHash();
//};

class state_t {

	string state_val;
	public:

	int state_index;
	gaIndiv_t* pIndiv;

	int hit_count;
	
	vector<int> branch_index;
	
	fitness_t state_fitness;

	// Constructors
	state_t();
	state_t(const string&);
    state_t(const state_t&);
	state_t(const rtLevelCkt* rtlObj, int index_ = -1);
	state_t(const gateLevelCkt*, int index_ = -1);

	~state_t();

    state_t& operator = (const state_t& copy_obj);
	bool operator==(const state_t& lhs);
	bool operator[](int) const;

	// Member functions
	keyVal_t 	getHash() const;
	string 		getState() const;

	static int mem_alloc_cnt;
};
#endif // __CIRCUIT_H__
