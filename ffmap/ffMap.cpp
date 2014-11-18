#include "incl.h"
#include "gateLevelSim.h"
#include "rtLevelSim.h"
#include <algorithm>

//#define ABCD 
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

#define MAPPING_LIST_OFF
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

	int numVec = vecFromFile.size();
	
	vector<string> gCktStrVec(numVec+1);
	vector<string> vCktStrVec(numVec+1);

	/* ALL ZEROS STATE	*/
	string init0str(NUM_STATE_BITS, '0');
	cktState *initS0 = new cktState(init0str.c_str(), 0);

	string init0str_g(gCkt->numStateFFs, '0');
	cktState *initS0_g = new cktState(init0str_g.c_str(), 0);
	
	gCktStrVec[0] = init0str_g;
	vCktStrVec[0] = init0str;

	gCkt->setCktState(*initS0_g);
	vCkt->setCktState(*initS0);
	
	#ifndef ABCD
	for (int idx = 0; idx < numVec; ++idx) {
		vecIn_t vecIn = vecFromFile[idx];
		gCkt->simOneVector(vecIn);
		vCkt->simOneVector(vecIn);

		gCktStrVec[idx+1] = gCkt->getCktState();
		vCktStrVec[idx+1] = vCkt->getCktState();
		
		//gCkt->printNextState();

		//cout << "g: ";
		//gCkt->printOutputs();

		//cout << endl << idx << endl;
		
		//cout << "v: ";
		//vCkt->printOutputs();
		//vCkt->printNextState();

	}
	
//	exit(-1);

	// Data structure for mapping
	int_vec regArr(NUM_STATE_BITS,0);
	int_vec dffArr(numFFs,0);
	vector<int_vec> ffMapArr(numFFs, regArr);
	
	#ifndef MAPPING_LIST_OFF
	#if defined(__b10)
	int ffMapping[] = {8, 0, 7, 5, 3, 2, 6, 4, 13, 12, 1};
	#endif
	#endif
	for (int cnt = 0; cnt < numVec; ++cnt) {

		string& gInitStr0 = gCktStrVec[cnt];
		string& vInitStr0 = vCktStrVec[cnt];

		string& gFinalStr0 = gCktStrVec[cnt+1];
		string& vFinalStr0 = vCktStrVec[cnt+1];
	
		int_vec gList01, gList10, vMapList;	
		int gParityI = 0, gParityF = 0;
		assert(gInitStr0.length() == gFinalStr0.length());
		for (uint j = 0; j < gFinalStr0.length(); ++j) {
			if (gInitStr0[j] == '1')
				++gParityI;
			if (gFinalStr0[j] == '1')
				++gParityF;
			if (gInitStr0[j] != gFinalStr0[j]) { 
				if (gFinalStr0[j] == '1')
					gList01.push_back(j);
				else
					gList10.push_back(j);
				#ifndef MAPPING_LIST_OFF
				vMapList.push_back(ffMapping[j]);
				#endif
			}
		}

		cout << endl
			 << cnt << endl
			 << gParityI << " -> " << gParityF << endl
			 << gInitStr0 << endl
			 << gFinalStr0 << endl;

		cout << "g01[" << gList01.size() << "]: ";
		printVec(gList01);
		cout << "g10[" << gList10.size() << "]: ";
		printVec(gList10);

		int_vec vList01, vList10;
		int vParityI = 0, vParityF = 0;
		assert(vInitStr0.length() == vFinalStr0.length());
		for (uint j = 0; j < vFinalStr0.length(); ++j) {
			if (vInitStr0[j] == '1')
				++vParityI;
			if (vFinalStr0[j] == '1')
				++vParityF;
			if (vInitStr0[j] != vFinalStr0[j]) {
				if (vFinalStr0[j] == '1')
					vList01.push_back(j);
				else
					vList10.push_back(j);
			}
		}

		cout << endl
			 << vParityI << " -> " << vParityF << endl
			 << vInitStr0 << endl
			 << vFinalStr0 << endl;

		cout << "v01[" << vList01.size() << "]: ";
		printVec(vList01);
		cout << "v10[" << vList10.size() << "]: ";
		printVec(vList10);

//		assert (gParityI == vParityI);
//		assert (gParityF == vParityF);

		#ifndef MAPPING_LIST_OFF
		cout << "mList[" << vMapList.size() << "]: ";
		std::sort(vMapList.begin(), vMapList.end());
		printVec(vMapList);
		#endif

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

//		for (int_vec_iter gt = gList.begin(); gt != gList.end(); ++gt) {
//			dffArr[*gt]++;
//			for (int_vec_iter vt = vList.begin(); vt != vList.end(); ++vt)
//				ffMapArr[*gt][*vt]++;
//		}
//
//		for (int_vec_iter vt = vList.begin(); vt != vList.end(); ++vt)
//			regArr[*vt]++;
	}
	
	cout << " - - - - - - - Mapping - - - - - - - -" << endl;

	for (int idx = 0; idx < numFFs; ++idx) {
		bool _print = false;
//		cout << dffArr[idx] << ":= ";	
		for (int jdx = 0; jdx < NUM_STATE_BITS; ++jdx) {
//			cout << ffMapArr[idx][jdx] << "," << regArr[jdx] << " ";
			if ((ffMapArr[idx][jdx] == regArr[jdx]) 
					&& (regArr[jdx] == dffArr[idx]) 
					/*&& (dffArr[idx])*/ ) {
				if (_print)
					cout << jdx << " ";
				else {
					cout << "FF " << idx << ": " << jdx << " ";
					_print = true;
				}
			}
		}
		cout << endl;			
//		if (_print)
//			cout << endl;
	}
	#endif
	
	#ifndef MAPPING_LIST_OFF
	char gOutName[80];
	sprintf(gOutName, "%s.gout", gCktName);
	
	char vOutName[80];
	sprintf(vOutName, "%s.vout", gCktName);
	
	ifstream gOutFile;
	gOutFile.open(gOutName, ios::in);

	if (gOutFile == NULL) {
		cerr << "Unable to open " << gOutName << endl;
		exit(-1);
	}

	ofstream vOutFile;
	vOutFile.open(vOutName, ios::out);
	
	if (vOutFile == NULL) {
		cerr << "Unable to open " << vOutName << endl;
		exit(-1);
	}
	
	#if defined(__b11)
//	int regMapping[] = {1, 10, 5, 4, 7, 3, 6, 2, 0, -1, -1, -1, 9 ,8};
	int regMapping[] = {2, 15, 9, 6, 12, 5, 11, 4, 1, -1, -1, -1, 14, 13};
	#endif

	#if defined(__b10)
//	int regMapping[] = {1, 10, 5, 4, 7, 3, 6, 2, 0, -1, -1, -1, 9 ,8};
	int regMapping[] = {2, 15, 9, 6, 12, 5, 11, 4, 1, -1, -1, -1, 14, 13};
	#endif
	while (gOutFile) {
		string gStr;
		getline(gOutFile, gStr);
		if (gStr.length() == 0)
			break;
		//cout << gStr.length() << endl;
		//assert(gStr.length() == gCkt->numFFs);

		/* Convert to RTL level */
		string vStr(NUM_STATE_BITS, '0');
		for (int idx = 0; idx < NUM_STATE_BITS; ++idx) {
			if (regMapping[idx] == -1)
				vStr[idx] = 'X';
			else
				vStr[idx] = gStr[regMapping[idx]];
		}
		vOutFile << vStr << endl;
	}
	vOutFile.close();
	gOutFile.close();
	#endif
	return 0;
}

