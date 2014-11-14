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

void printVec(int_vec& inVec) {
	for (int_vec_iter it = inVec.begin(); it != inVec.end(); ++it)
		cout << *it << " ";
	cout << endl;
}

int main(int argc, char** argv) {
	
	char gCktName[80];
	sprintf(gCktName, "%srst", benchCkt);
	gateLevelCkt* gCkt = new gateLevelCkt(gCktName);
	gCkt->printCurrState();

	Vtop* ckt = new Vtop;
	rtLevelCkt* vCkt = new rtLevelCkt(ckt);
	vCkt->printCurrState();

	vector<vecIn_t> vecFromFile;
	gCkt->readVecFile(vecFromFile);

	int numFFs = gCkt->numStateFFs;
	cout << numFFs 
		 << " FFs -> " << NUM_STATE_BITS 
		 << " Reg Mapping." << endl; 
	
	// Data structure for mapping
	int_vec regArr(NUM_STATE_BITS,0);
	int_vec dffArr(numFFs,0);
	vector<int_vec> ffMapArr(numFFs, regArr);

	// ALL ZEROS
	string init0str(NUM_STATE_BITS, '0');
	cktState *initS0 = new cktState(init0str.c_str(), 0);

	string init0str_g(gCkt->numStateFFs, '0');
	cktState *initS0_g = new cktState(init0str_g.c_str(), 0);

//	gCkt->setCktState(initS0_g);
//	gCkt->printCurrState();
//	vecIn_t inp (gCkt->numInputs, 'X');
//
//	gCkt->simOneVector(inp);
//	gCkt->printCurrState();
//
//	cout << endl
//		 << "RTL Simulation" << endl;
//	vCkt->setCktState(initS0);
//	
//	vCkt->printCurrState();	
//	vCkt->printInputs();
//	vCkt->printNextState();
//	vCkt->printOutputs();
	
	vector<cktState*> gCktStateVec;
	vector<cktState*> vCktStateVec;

	gCktStateVec.push_back(initS0_g);
	vCktStateVec.push_back(initS0);

	// ALL ONES
//	string init1str(NUM_STATE_BITS, '1');
//	cktState *initS1 = new cktState(init1str.c_str(), 0);
//
//	string init1str_g(gCkt->numStateFFs, '1');
//	cktState *initS1_g = new cktState(init1str_g.c_str(), 0);
//
//	gCktStateVec.push_back(initS1_g);
//	vCktStateVec.push_back(initS1);

	const int NUM_INPUT_COMBI = 4;//1 << NUM_INPUT_BITS;
	int NUM_ROUNDS = vecFromFile.size() - NUM_INPUT_COMBI;
//	if (NUM_ROUNDS > 100)
//		NUM_ROUNDS = 100;
	
	vecIn_t inp (gCkt->numInputs, 'X');
	
	for (int cnt = 0; cnt < NUM_ROUNDS; ++cnt) {
		//cktState *gInitSt = gCktStateVec[2*cnt];
		//cktState *vInitSt = vCktStateVec[2*cnt];

		vector<vecIn_t> inputVec(NUM_INPUT_COMBI, inp);
		vecIn_t vecIn;
		for (int idx = 0; idx < NUM_INPUT_COMBI; idx++) {
//			RandomVecIn(vecIn, NUM_INPUT_BITS);
//			vecIn += '1';
			inputVec[idx] = vecFromFile[cnt+idx];
		}

		// ALL ZEROS
		for (int idx = 0; idx < NUM_INPUT_COMBI; idx++) {
			vecIn = inputVec[idx];
			cout << "Vector: " << vecIn << endl;

			cktState *gInitSt = gCktStateVec[cnt];
			cktState *vInitSt = vCktStateVec[cnt];

			cout << "Gate Level Simulation" << endl;
			gCkt->setCktState(*gInitSt);
			gCkt->simOneVector(vecIn);
			
			cout << "RTL Simulation" << endl;
			vCkt->setCktState(*vInitSt);
			vCkt->simOneVector(vecIn);

			gCkt->printInputs();
			vCkt->printInputs();

			gCkt->printCurrState();
			vCkt->printCurrState();

			gCkt->printNextState();
			vCkt->printNextState();

			gCkt->printOutputs();
			vCkt->printOutputs();
			
			cktState *gFinalSt = new cktState(gCkt, cnt*NUM_INPUT_COMBI + 2*idx);
			cktState *vFinalSt = new cktState(vCkt, cnt*NUM_INPUT_COMBI + 2*idx);

			string gInitStr0 = gInitSt->getState();
			string vInitStr0 = vInitSt->getState();

			string gFinalStr0 = gFinalSt->getState();
			string vFinalStr0 = vFinalSt->getState();
		
			int_vec gList;
			assert(gInitStr0.length() == gFinalStr0.length());
			for (uint j = 0; j < gFinalStr0.length(); ++j) {
				if (gInitStr0[j] != gFinalStr0[j])
					gList.push_back(j);
			}

			cout << "gList: ";
			printVec(gList);

			int_vec vList;
			assert(vInitStr0.length() == vFinalStr0.length());
			for (uint j = 0; j < vFinalStr0.length(); ++j) {
				if (vInitStr0[j] != vFinalStr0[j])
					vList.push_back(j);
			}

			cout << "vList: ";
			printVec(vList);

			/* Compute the list of toggled FFs in both 
				gList = { ... }
				vList = { ... }

				for each element idx in gList
					for each element jdx in vList
						if dffArr[idx] = 0 && regArr[jdx] = 0
							ffMap[idx][jdx] = 1
							tmpDFF++

				// Alt idea
				for each element idx in gList
					dffArr[idx]++
					for each element jdx in vList
							ffMap[idx][jdx]++

				for each element jdx in vList
					regArr[jdx]++
			*/

			for (int_vec_iter gt = gList.begin(); gt != gList.end(); ++gt) {
				dffArr[*gt]++;
				for (int_vec_iter vt = vList.begin(); vt != vList.end(); ++vt)
					ffMapArr[*gt][*vt]++;
			}

			for (int_vec_iter vt = vList.begin(); vt != vList.end(); ++vt)
				regArr[*vt]++;

			/* Selection criteria for state
				- if stateStr is unique, then push into vector 
				- if any improvement in mapping was made
			*/
			gCktStateVec.push_back(gFinalSt);
			vCktStateVec.push_back(vFinalSt);
			
						
//			cout << endl;
//
//			// ALL ONES
//			gInitSt = gCktStateVec[2*cnt+1];
//			vInitSt = vCktStateVec[2*cnt+1];
//
//			cout << "Gate Level Simulation" << endl;
//			gCkt->setCktState(*gInitSt);
//			gCkt->simOneVector(vecIn);
//			
//			cout << "RTL Simulation" << endl;
//			vCkt->setCktState(*vInitSt);
//			vCkt->simOneVector(vecIn);
//
//			gCkt->printInputs();
//			vCkt->printInputs();
//
//			gCkt->printCurrState();
//			vCkt->printCurrState();
//
//			gCkt->printNextState();
//			vCkt->printNextState();
//
//			gCkt->printOutputs();
//			vCkt->printOutputs();
//			
//			gFinalSt = new cktState(gCkt, cnt*NUM_INPUT_COMBI + 2*idx+1);
//			vFinalSt = new cktState(vCkt, cnt*NUM_INPUT_COMBI + 2*idx+1);
//
//			string gInitStr1 = gInitSt->getState();
//			string vInitStr1 = vInitSt->getState();
//
//			string gFinalStr1 = gFinalSt->getState();
//			string vFinalStr1 = vFinalSt->getState();
//		
//			gList.clear();
//			assert(gInitStr1.length() == gFinalStr1.length());
//			for (uint j = 0; j < gFinalStr1.length(); ++j) {
//				if (gInitStr1[j] != gFinalStr1[j])
//					gList.push_back(j);
//			}
//
//			cout << "gList: ";
//			printVec(gList);
//
//			vList.clear();
//			assert(vInitStr1.length() == vFinalStr1.length());
//			for (uint j = 0; j < vFinalStr0.length(); ++j) {
//				if (vInitStr1[j] != vFinalStr1[j])
//					vList.push_back(j);
//			}
//
//			cout << "vList: ";
//			printVec(vList);
//			gCktStateVec.push_back(gFinalSt);
//			vCktStateVec.push_back(vFinalSt);
//
//			cout << endl << gCktStateVec.size() << endl;
		}

		cout << " - - - - - - - Mapping - - - - - - - -" << endl;

		for (int idx = 0; idx < numFFs; ++idx) {
			bool _print = false;
			for (int jdx = 0; jdx < NUM_STATE_BITS; ++jdx) {
				if ((ffMapArr[idx][jdx] == regArr[jdx]) && 
						(regArr[jdx] == dffArr[idx]) && (dffArr[idx])) {
					if (_print)
						cout << jdx << " ";
					else {
						cout << "FF " << idx << ": " << jdx << " ";
						_print = true;
					}
				}
						
			}
			if (_print)
				cout << endl;
		}
		cout << endl << "Next init state" << endl << endl;
	}
	return 0;
}

