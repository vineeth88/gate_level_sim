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
	
	string init0str(NUM_STATE_BITS, '0');
	cktState initS0(init0str.c_str(), 0);

	string init0gstr(gCkt->numStateFFs, '0');
	cktState initS0g(init0gstr.c_str(), 0);

	gCkt->setCktState(initS0g);
	gCkt->printCurrState();
	vecIn_t inp (gCkt->numInputs, 'X');

	gCkt->simOneVector(inp);
	gCkt->printCurrState();

	cout << endl
		 << "RTL Simulation" << endl;
	vCkt->setCktState(initS0);
	
	vCkt->printCurrState();	
	vCkt->printInputs();
	vCkt->printNextState();
	vCkt->printOutputs();
	

	for (int cnt = 0; cnt < 10; cnt ++) {
	vecIn_t vecIn;
	RandomVecIn(vecIn, gCkt->numInputs);
	cout << endl
		 << "Vector: " << vecIn << endl;

	cout << endl
		 << "Gate Level Simulation" << endl;
	gCkt->setCktState(initS0g);
	
	gCkt->simOneVector(vecIn);
	
	gCkt->printInputs();
	gCkt->printCurrState();
	gCkt->printNextState();
	gCkt->printOutputs();
	
	cout << endl
		 << "RTL Simulation" << endl;

	vCkt->setCktState(initS0);
	vCkt->simOneVector(vecIn);

	vCkt->printInputs();
	vCkt->printCurrState();
	
	vCkt->printNextState();
	vCkt->printOutputs();

	}
	return 0;
}

