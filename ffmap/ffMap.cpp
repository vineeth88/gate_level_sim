#include "incl.h"
#include "gateLevelSim.h"
#include "rtLevelSim.h"

void RandomVecIn(vecIn_t& vecIn, int NUM_INPUT_BITS) {
	vecIn = string(NUM_INPUT_BITS, '0');
    for (uint i = 0; i < vecIn.size(); i++) 
		vecIn[i] = (rand() & 0x01) + 48;

    assert(vecIn.length() == (uint)NUM_INPUT_BITS);
}

void RandomVecIn(int_vec& vecIn, int NUM_INPUT_BITS) {
	vecIn = int_vec(NUM_INPUT_BITS, XVAL);
    for (uint i = 0; i < vecIn.size(); i++) 
		vecIn[i] = (rand() & 0x01);

    assert(vecIn.size() == (uint)NUM_INPUT_BITS);
}

int main(int argc, char** argv) {
	
	char gCktName[80];
	sprintf(gCktName, "%srst", benchCkt);
	gateLevelCkt* gCkt = new gateLevelCkt(gCktName);
	gCkt->printCurrState();

	Vtop* ckt = new Vtop;
	rtLevelCkt* vCkt = new rtLevelCkt(ckt);
	vCkt->printCurrState();
	
	cktState initS0("00000", 0);

	gCkt->setCktState(initS0);
	gCkt->printCurrState();

	vCkt->setCktState(initS0);
	
	for (int cnt = 0; cnt < 10; cnt ++) {
	vecIn_t vecIn;
	RandomVecIn(vecIn, gCkt->numInputs);
	cout << endl
		 << "Vector: " << vecIn << endl;

	cout << endl
		 << "Gate Level Simulation" << endl;
//	gCkt->setCktState(initS0);
	
	gCkt->simOneVector(vecIn);
	
	gCkt->printInputs();
	gCkt->printCurrState();
	gCkt->printNextState();
	gCkt->printOutputs();
	
//	cout << endl
//		 << "RTL Simulation" << endl;
//
////	vCkt->setCktState(initS0);
//	vCkt->printCurrState();
//	
//	vCkt->simOneVector(vecIn);
//
//	vCkt->printInputs();
//	vCkt->printNextState();
//	vCkt->printOutputs();

	}
	return 0;
}

