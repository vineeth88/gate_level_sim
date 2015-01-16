#include "gateLevelSim.h"
#include <deque>

using namespace std;

//const int ZERO = 0;
//const int ONES = 1;
//const int XVAL = -1;
const int DVAL = 3;
const int DBAR = -3;

void printInpVec(const vector<int>&);

enum {
	G_JUNK, 	/* 0 */
	G_INPUT, 	/* 1 */
	G_OUTPUT, 	/* 2 */
	G_XOR, 		/* 3 */
	G_XNOR, 	/* 4 */
	G_DFF, 		/* 5 */
	G_AND, 		/* 6 */
	G_NAND, 	/* 7 */
	G_OR, 		/* 8 */
	G_NOR, 		/* 9 */
	G_NOT, 		/* 10 */
	G_BUF, 		/* 11 */
};

bool INIT = false;
char INIT_VAL = '0';
bool PRINT_INPUT = false;
bool PRINT_OUTPUT = false;
bool PRINT_CURR_STATE = false;
bool PRINT_NEXT_STATE = false;

#define EVTSIM_DIFF_ON
#define EVTSIM_SCHED_ON
//#define EVTSIM_PRINT_ON

bool SET_CKT_STATE = false;
//int main(int argc, char **argv) {
//	
//	char cktName[80];
//	if (argc < 2) {
//		cout << "Enter circuit name\n"
//			 << "Usage: " << argv[0] << " [-ios] <ckt>" << endl
//			 << "The -i[v] option initialize FFs from <ckt>.init0 file."
//			 << "v=0: .init0, v=1: .init1" << endl
//			 << "The -o option prints the PO values" << endl
//			 << "The -s option prints the PPO" << endl
//			 << "The -a option prints input/output/state values" << endl;
//		exit(-1);
//	}
//	else {
//		for (int idx = 1; idx < argc; ++idx) {
//			if (argv[idx][0] == '-') {
//				for (int jdx = 1; jdx < strlen(argv[idx]); jdx++) {
//					switch(argv[idx][jdx]) {
//						case 'i':	INIT = true;
//									INIT_VAL = '0';
//
//									if (argv[idx][jdx+1] == '1') {
//										INIT_VAL = '1';
//										jdx++;
//									}
//									else if (argv[idx][jdx+1] == '0') 
//										jdx++;
//
//									break;
//
//						case 'o':	PRINT_OUTPUT = true;
//									break;
//
//						case 's':	PRINT_NEXT_STATE = true;
//									break;
//
//						case 'a':	PRINT_NEXT_STATE = true;
//									PRINT_INPUT = true;
//									PRINT_OUTPUT = true;
//									PRINT_CURR_STATE = true;
//									break;
//
//						default :	cout << "\nUnknown choice!" << argv[idx][jdx] 
//										 << "\nExiting ... " << endl;
//									exit(-1);
//									break;
//					}
//				}
//			}
//			else {
//				sprintf(cktName, "%s", argv[idx]);
//				cout << "Using " << cktName << endl;
//				break;
//			}
//		}
//	}
//
//	gateLevelCkt *ckt = new gateLevelCkt(cktName);	
//	
//	ckt->simulateVectors();
//
//	delete ckt;
//
//	return 0;
//}

gateLevelCkt::gateLevelCkt(char *cktname) {
	FILE *levFile;
	char fName[256];
	int i, j, count;
	char c;
	int gatenum, junk;
	int f1, f2, f3;
	
	strcpy(cktName, cktname);
	sprintf(fName, "%s.lev", cktname);
//	strcpy(fName, cktname);
//	strcat(fName, ".lev");
	levFile = fopen(fName, "r");
	if (levFile == NULL) {
		fprintf(stderr, "Can't open .lev file\n");
		exit(-1);
	}

	numGates = maxLevels = 0;
	numInputs = numOutputs = numFFs = 0;

	fscanf(levFile, "%d", &count); // number of gates
	fscanf(levFile, "%d", &junk); // skip the second line

	// allocate space for gates data structure
	gtype = vector<uchar>(count);
	fanin = vector<short>(count); 
	fanout = vector<short>(count);	
	levelNum = vector<int>(count);
	
	dffList = vector<int>();
	stateFFList = vector<int>();

	vector<int> tmp = vector<int>();
	inList = vector< vector<int> >(count, tmp);
	outList = vector< vector<int> >(count, tmp);
	
	numGates = count;
	// now read in the circuit
	for (i = 1; i < count; i++) {
		fscanf(levFile, "%d", &gatenum);
		fscanf(levFile, "%d", &f1);
		fscanf(levFile, "%d", &f2);
		fscanf(levFile, "%d", &f3);

	//	numGates++;
		gtype[gatenum] = (uchar) f1;
		if (gtype[gatenum] > 13)
			printf("gate %d is an unimplemented gate type\n", gatenum);
		else if (gtype[gatenum] == G_INPUT)
			numInputs++;
		else if (gtype[gatenum] == G_OUTPUT)
			numOutputs++;
		else if (gtype[gatenum] == G_DFF) {
			dffList.push_back(gatenum);		
			numFFs++;
		}

		f2 = (int) f2;
		levelNum[gatenum] = f2;

		if (f2 >= (maxLevels))
			maxLevels = f2 + 5;

		fanin[gatenum] = (int) f3;
		// now read in the inList
		inList[gatenum] = vector<int>(fanin[gatenum]);
		for (j = 0; j < fanin[gatenum]; j++) {
			fscanf(levFile, "%d", &f1);
			inList[gatenum][j] = (int) f1;
		}

		for (j = 0; j < fanin[gatenum]; j++) // followed by samethings
			fscanf(levFile, "%d", &junk);

		// read in the fanout list
		fscanf(levFile, "%d", &f1);
		fanout[gatenum] = (int) f1;

		outList[gatenum] = vector<int>(fanout[gatenum]);		
		for (j = 0; j < fanout[gatenum]; j++) {
			fscanf(levFile, "%d", &f1);
			outList[gatenum][j] = (int) f1;
		}

		fscanf(levFile, "%d", &f1);
		fscanf(levFile, "%d", &f2);
		
		// skip till end of line
		while ((c = getc(levFile)) != '\n' && c != EOF)
			;
	} // for (i...)
	fclose(levFile);
	
	value = vector<char>(numGates+1, 'X');
	
	numStateFFs = 0;
	for (int_vec_iter ff = dffList.begin(); ff != dffList.end(); ++ff) {
//		if ((fanout[*ff] == 1) && (gtype[outList[*ff][0]] == G_OUTPUT))
//			continue;
		if (!checkFanoutCone(*ff)) {
			stateFFList.push_back(*ff);
			numStateFFs++;
		}
	}
	
	printf("Successfully read in circuit:\n");
	printf("\t%d PIs.\n", numInputs);
	printf("\t%d POs.\n", numOutputs);
	printf("\t%d FFs.\n", numFFs);
	printf("\t%d Total number of gates\n", numGates);
	printf("\t%d Levels in the circuit.\n", maxLevels / 5);
	
	cout << "FFs: " << stateFFList.size() << "\n";
	for (vector<int>::iterator it = stateFFList.begin();
			it != stateFFList.end(); ++it) 
		cout << *it << " ";
	cout << endl;
}

//void gateLevelCkt :: simulateVectors() {
//	
//	assert(dffList.size() == (uint)numFFs);
//	vector<char> dffValue (numFFs, 'X');
//
//	if (INIT) {
//		FILE* initFile;
//		char initName[80];
//
//		sprintf(initName, "%s.init%c", cktName, INIT_VAL);
//		initFile = fopen(initName, "r");
//
//		if (initFile == NULL) {
//			fprintf(stderr, "Can't open %s file\n", initName);
//			exit(-1);
//		}
//		
//		char ch;
//		for (int i = 0; i < numFFs; i++) {
//			fscanf(initFile, "%c", &ch);
//			dffValue[i] = ch;
//		}
//
//		fclose(initFile);
//
//	}
//	for (int i = 0; i < dffList.size(); ++i) {
//		int gate = dffList[i];
//		value[inList[gate][0]] = dffValue[i];
//	}
//
//	vector< vector<int> > inputVec;
//	readVecFile(inputVec);
//	for (vector< vector<int> >::iterator vt = inputVec.begin(); 
//			vt != inputVec.end(); ++vt) {
////		if (PRINT_INPUT) {
////			cout << "Input : ";
////			printInpVec(*vt);
////		}
//
//		logicSim(*vt);
//	
//		if (PRINT_INPUT)
//			printInputs();
//
//		if (PRINT_OUTPUT)
//			printOutputs();
//
//		if (PRINT_CURR_STATE)
//			printCurrState();
//		
//		if (PRINT_NEXT_STATE)
//			printNextState();
//		
//		if (PRINT_NEXT_STATE)
//			cout << endl;
//
////		printValues();
//	}
//}

void printInpVec(const vector<int>& vecIn) {
	for (uint idx = 0; idx < vecIn.size(); ++idx) {
		cout << vecIn[idx] - '0';
	}
	cout << endl;
}

void gateLevelCkt :: printInputs() {
	cout << "Input : ";
	for (int idx = 1; idx <= numInputs; ++idx)
		cout << value[idx];
	cout << endl;
}

void gateLevelCkt :: printCurrState() {
	cout << "P state : ";	
//	for (uint idx = 0; idx < dffList.size(); ++idx) {
//		int gate = dffList[idx];
//		cout << value[gate];
//	}
//	cout << endl;
	for (uint idx = 0; idx < stateFFList.size(); ++idx) {
		int gate = stateFFList[idx];
		cout << value[gate];
	}
	cout << endl;
}

void gateLevelCkt :: printNextState() {
	cout << "N state : ";	
//	for (uint idx = 0; idx < dffList.size(); ++idx) {
//		int gate = inList[dffList[idx]][0];
//		cout << value[gate];
//	}
//	cout << endl;
	for (uint idx = 0; idx < stateFFList.size(); ++idx) {
		int gate = inList[stateFFList[idx]][0];
		cout << value[gate];
	}
	cout << endl;
}

void gateLevelCkt :: printOutputs() {
	cout << "Output : ";
	for (int idx = 1; idx <= numGates; ++idx) {
		if (gtype[idx] == G_OUTPUT)
			cout << value[idx];
	}
	cout << endl;
}
			
void gateLevelCkt :: printValues() {
	for (int gate = 1; gate <= numGates; ++gate) {
//		if (gtype[gate] != G_OUTPUT)
//			continue;
		cout << value[gate];
}
	cout << endl;
}

void gateLevelCkt :: simOneVector(const vecIn_t& vecIn) {
	
	int numLevels = maxLevels / 5;
	int_vec tmp;
	vector<int_vec>levelEvents(numLevels, tmp);
	#ifdef EVTSIM_SCHED_ON
	vector<char> sched(numGates+1, 0);
	#endif

	assert(dffList.size() == (uint)numFFs);
	if (SET_CKT_STATE) {
//		cout << "--1--" << endl;
		for (uint i = 0; i < dffList.size(); ++i) {
			int gate = dffList[i];
			// Simulate all fanouts of FF
			for (int j = 0; j < fanout[gate]; ++j) {
				int fngate = outList[gate][j];
				int level = levelNum[fngate];
				#ifdef EVTSIM_SCHED_ON
				if (sched[fngate] == 0) {
					levelEvents[level/5].push_back(fngate);
					sched[fngate] = 1;
				}
				#else
				levelEvents[level/5].push_back(fngate);
				#endif
			}

			// Simulate the succOfPred != FF 
			int ingate = inList[gate][0];
			for (int j = 0; j < fanout[ingate]; ++j) {
				int fngate = outList[ingate][j];
				if (fngate == gate)
					continue;
				int level = levelNum[fngate];
				#ifdef EVTSIM_SCHED_ON
				if (sched[fngate] == 0) {
					levelEvents[level/5].push_back(fngate);
					sched[fngate] = 1;
				}
				#else
				levelEvents[level/5].push_back(fngate);
				#endif
			}
			#ifdef EVTSIM_PRINT_ON
			cout << "Scheduled fn[" << gate <<"]" << endl;
			#endif

		}
		SET_CKT_STATE = false;
	}
	else {
		for (uint i = 0; i < dffList.size(); ++i) {
			int gate = dffList[i];
			#ifdef EVTSIM_DIFF_ON
			if (value[gate] != value[inList[gate][0]]) {
			#endif
				value[gate] = value[inList[gate][0]];
				for(int j = 0; j < fanout[gate]; ++j) {
					int fngate = outList[gate][j];
					int level = levelNum[fngate];
					#ifdef EVTSIM_SCHED_ON
					if (sched[fngate] == 0) {
						levelEvents[level/5].push_back(fngate);
						sched[fngate] = 1;
					}
					#else
					levelEvents[level/5].push_back(fngate);
					#endif
				}
				#ifdef EVTSIM_PRINT_ON
				cout << "Scheduled fn[" << gate <<"]" << endl;
				#endif
			#ifdef EVTSIM_DIFF_ON
			}
			#endif
		}
	}	

	// Schedule inputs
	assert(vecIn.length() == (uint)numInputs);
	for (int gate = 1; gate <= numInputs; ++gate) {
		#ifdef EVTSIM_DIFF_ON
		if (value[gate] != vecIn[gate-1]) {
		#endif
			value[gate] = vecIn[gate-1];
			for(int j = 0; j < fanout[gate]; ++j) {
				int fngate = outList[gate][j];
				int level = levelNum[fngate];
				#ifdef EVTSIM_SCHED_ON
				if (sched[fngate] == 0) {
					levelEvents[level/5].push_back(fngate);
					sched[fngate] = 1;
				}
				#else
				levelEvents[level/5].push_back(fngate);
				#endif
			}
			#ifdef EVTSIM_PRINT_ON
			cout << "Scheduled fn[" << gate <<"]" << endl;
			#endif

		#ifdef EVTSIM_DIFF_ON
		}
		#endif
	}

	/*	Simulate the event wheel */
	#ifdef EVTSIM_PRINT_ON
	int lvlNum = 0;
	cout << "Event Wheel" << endl;
	#endif

	for (vector<int_vec>::iterator vt = levelEvents.begin();
			vt != levelEvents.end(); ++vt) {
		#ifdef EVTSIM_PRINT_ON
		cout << lvlNum << ": ";
		#endif
		for (int_vec_iter it = vt->begin(); it != vt->end(); ++it) {
			char prev_value = value[*it];
			eval(*it);

			#ifdef EVTSIM_PRINT_ON
			cout << *it << " ";
			#endif

			#ifdef EVTSIM_SCHED_ON
			sched[*it] = 0;
			#endif
			
			#ifdef EVTSIM_DIFF_ON
			if (value[*it] != prev_value) {
			#endif
				for(int j = 0; j < fanout[*it]; ++j) {
					int fngate = outList[*it][j];
					int level = levelNum[fngate];
					#ifdef EVTSIM_SCHED_ON
					if (sched[fngate] == 0) {
						levelEvents[level/5].push_back(fngate);
						sched[fngate] = 1;
					}
					#else
					levelEvents[level/5].push_back(fngate);
					#endif
				}
			#ifdef EVTSIM_DIFF_ON
			}
			#endif
		}
		#ifdef EVTSIM_PRINT_ON
		cout << endl;
		lvlNum++;
		#endif
	}
	
	#ifdef EVTSIM_PRINT_ON
	#ifdef EVTSIM_SCHED_ON
	cout << "Scheduled gates: " << endl;
	for (uint gate = 1; gate <= sched.size(); ++gate)
		if (sched[gate])
			cout << "(" << gate << "," << levelNum[gate] << ")";
	cout << endl;	
	#else
	cout << "Scheduled gates: " << endl;
	for (vector<int>::iterator it = levelEvents[0].begin();
			it != levelEvents[0].end(); ++it)
		cout << *it << " ";
	cout << endl;
	#endif
	#endif
}

// TODO!! Has to be modified accordingly to changes above
void gateLevelCkt :: simOneVector(const int_vec& vecIn) {
	
	int numLevels = maxLevels / 5;
	int_vec tmp;
	vector<int_vec>levelEvents(numLevels, tmp);
	#ifdef EVTSIM_SCHED_ON
	vector<char> sched(numGates+1, 0);
	#endif
	
	/* 	Set the curr state values of the FFs	
	   	If curr state != next state, schedule */
	assert(dffList.size() == (uint)numFFs);
	for (uint i = 0; i < dffList.size(); ++i) {
		int gate = dffList[i];
		if (value[gate] != value[inList[gate][0]]) {
			value[gate] = value[inList[gate][0]];
			for(int j = 0; j < fanout[gate]; ++j) {
				int fngate = outList[gate][j];
				int level = levelNum[fngate];
				#ifdef EVTSIM_SCHED_ON
				if (sched[fngate] == 0) {
					levelEvents[level/5].push_back(fngate);
					sched[fngate] = 1;
				}
				#else
				levelEvents[level/5].push_back(fngate);
				#endif
			}
		}
	}
	
	/*	Set the input values */
	assert(vecIn.size() == (uint)numInputs);
	for (int gate = 1; gate <= numInputs; ++gate) {
//		if (value[gate] != vecIn[gate-1] + '0') {
			value[gate] = vecIn[gate-1] + '0';
			for(int j = 0; j < fanout[gate]; ++j) {
				int fngate = outList[gate][j];
				int level = levelNum[fngate];
				#ifdef EVTSIM_SCHED_ON
				if (sched[fngate] == 0) {
					levelEvents[level/5].push_back(fngate);
					sched[fngate] = 1;
				}
				#else
				levelEvents[level/5].push_back(fngate);
				#endif
			}
			cout << "Scheduled fn[" << gate <<"]" << endl;
//		}
	}

	/*	Simulate the event wheel */
	#ifdef EVTSIM_PRINT_ON
	int lvlNum = 0;
	cout << "Event Wheel" << endl;
	#endif
	for (vector<int_vec>::iterator vt = levelEvents.begin();
			vt != levelEvents.end(); ++vt) {
		#ifdef EVTSIM_PRINT_ON
		cout << lvlNum << ": ";
		#endif
		for (int_vec_iter it = vt->begin(); it != vt->end(); ++it) {
			char prev_value = value[*it];
			eval(*it);
			#ifdef EVTSIM_SCHED_ON
			sched[*it] = 0;
			#endif
			cout << *it << " ";
			if (value[*it] != prev_value) {
				for(int j = 0; j < fanout[*it]; ++j) {
					int fngate = outList[*it][j];
					int level = levelNum[fngate];
					#ifdef EVTSIM_SCHED_ON
					if (sched[fngate] == 0) {
						levelEvents[level/5].push_back(fngate);
						sched[fngate] = 1;
					}
					#else
					levelEvents[level/5].push_back(fngate);
					#endif
				}
			}
		}
		#ifdef EVTSIM_PRINT_ON
		cout << endl;
		lvlNum++;
		#endif
	}
	
	#ifdef EVTSIM_SCHED_ON
	cout << "Scheduled gates: " << endl;
	for (uint gate = 1; gate <= sched.size(); ++gate)
		if (sched[gate])
			cout << "(" << gate << "," << levelNum[gate] << ")";
	cout << endl;	
	#else
	cout << "Scheduled gates: " << endl;
	for (vector<int>::iterator it = levelEvents[0].begin();
			it != levelEvents[0].end(); ++it)
		cout << *it << " ";
	cout << endl;
	#endif
}

int gateLevelCkt::eval(int gate) {
	
	int opVal;
	bool xFlag;
	switch (gtype[gate]) {

		case G_INPUT:
			break;
			
		case G_OUTPUT:
			opVal = value[inList[gate][0]] - '0';
			break;
			
		case G_XOR:
			opVal = 0;
			xFlag = false;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == 'X') {
					xFlag = true;
					break;
				}
				else if (value[inList[gate][i]] == '1')
					opVal++;
			}
			if (xFlag)
				opVal = XVAL;
			else
				opVal = opVal % 2;
			break;
		
		case G_XNOR:
			opVal = 1;
			xFlag = false;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == 'X') {
					xFlag = true;
					break;
				}
				else if (value[inList[gate][i]] == '1')
					opVal++;
			}
			if (xFlag)
				opVal = XVAL;
			else
				opVal = opVal % 2;
			break;
		
		case G_DFF:
			break;

		case G_AND:
			opVal = 1;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == '0') {
					opVal = 0;
					break;
				}
				else if (value[inList[gate][i]] == 'X')
					opVal = XVAL;
			}
			break;
				 
		case G_NAND:
			opVal = 0;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == '0') {
					opVal = 1;
					break;
				}
				else if (value[inList[gate][i]] == 'X')
					opVal = XVAL;
			}
			break;
			
		case G_OR:
			opVal = 0;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == '1') {
					opVal = 1;
					break;
				}
				else if (value[inList[gate][i]] == 'X')
					opVal = XVAL;
			}
			break;
			
		case G_NOR:
			opVal = 1;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == '1') {
					opVal = 0;
					break;
				}
				else if (value[inList[gate][i]] == 'X')
					opVal = XVAL;
			}
			break;

		case G_NOT:
			if (value[inList[gate][0]] == 'X')
				opVal = XVAL;
			else if (value[inList[gate][0]] == '0')
				opVal = 1;
			else
				opVal = 0;
			break;

		case G_BUF:
			opVal = value[inList[gate][0]] - '0';
			break;

		default:
			printf("\nUnknown gate type. Aborting Evaluation");
			return -1;
	}
	

	if((gtype[gate] != G_INPUT) && (gtype[gate] != G_DFF)) {
		assert((opVal == 0) || (opVal == 1) || (opVal == XVAL));
		value[gate] = (char) (opVal + '0');
	}

	return 0;
}

string gateLevelCkt :: getCktState() const {
	string state(numStateFFs, 'X');
	for (int ffIdx = 0; ffIdx < numStateFFs; ++ffIdx)
		state[ffIdx] = value[inList[stateFFList[ffIdx]][0]];
	
	return state;
}

void gateLevelCkt :: setCktState(const cktState& state) {
	string stateVal = state.getState();
	//cout << stateVal.length() << ": : " <<endl;
	assert(stateVal.length() == (uint)numStateFFs);
	
//	for (int ffIdx = 0; ffIdx < dffList.size(); ++ffIdx) {
//		int gate = dffList[ffIdx];
//		value[gate] = '0';
//	}

	for (int ffIdx = 0; ffIdx < numStateFFs; ++ffIdx) {
		int gate = stateFFList[ffIdx];
		value[gate] = stateVal[ffIdx];
	}

	SET_CKT_STATE = true;
//	cout << endl;
}

//void gateLevelCkt :: setCktState(const cktState& state) {
//	string stateVal = state.getState();
//	//cout << stateVal.length() << ": : " <<endl;
//	assert(stateVal.length() == (uint)numStateFFs);
//	
////	cout << "S: " << stateVal << endl;
//	for (int ffIdx = 0; ffIdx < numStateFFs; ++ffIdx) {
//		int gate = stateFFList[ffIdx];
//		value[inList[gate][0]] = stateVal[ffIdx];
//		if (stateVal[ffIdx] == 'X')
//			value[gate] = (char) ((rand() & 0x1) + '0');
//		else
//			value[gate] = (char) (((stateVal[ffIdx] - '0') ^ 1) + '0');
////		cout << value[inList[gate][0]] << "-" << value[gate] << " ";
//	}
////	cout << endl;
//}

bool gateLevelCkt :: checkFanoutCone(int ffInd) {
	
	bool outFF = true;
	deque<int> qBFS;
	qBFS.push_back(ffInd);
	
	vector<int> visited(numGates, 1);

	while(outFF && qBFS.size()) {
		int gate = qBFS.front();
		qBFS.pop_front();
		for (int idx = 0; idx < fanout[gate]; ++idx) {
			int fngate = outList[gate][idx];
			if ((gtype[fngate] == G_DFF) && (fngate != ffInd)) {
				outFF = false;
				break;
			}
			else if (visited[fngate]) {
				qBFS.push_back(fngate);
				visited[fngate] = 0;
			}
		}
	}
	
	return outFF;
}


cktState :: cktState(const gateLevelCkt* ckt, int idx) {
	
	stateIdx = idx;
	stateVal = ckt->getCktState();

}
	
//void gateLevelCkt :: readVecFile(vector< vector<int> >& inputVec) {
//
//    inputVec.clear();
//
//    char vecName[100];
//    sprintf(vecName, "%s.vec", cktName);
//
//	FILE *vecFile;
//	vecFile = fopen(vecName, "r");
//	if (vecFile == NULL) {
//		fprintf(stderr, "Can't open %s file\n", vecName);
//		exit(-1);
//	}
//	
//	int vecWidth;
//	fscanf(vecFile, "%d", &vecWidth); // number of gates
//	assert(vecWidth == numInputs);
//
//	char ch;
//	fscanf(vecFile, "%c", &ch);
//	while ((ch == ' ') || (ch == '\n'))
//		fscanf(vecFile, "%c", &ch);
//	
//	int numVec = 0;
//	while (ch != 'E') {
//		vector<int> vecIn(vecWidth);
//		vecIn[0] = ch;
//
//		for (int i = 1; i < vecWidth; ++i) {
//			fscanf(vecFile, "%c", &ch);
//			vecIn[i] = ch;
//		}
//
//		numVec++;	
//		inputVec.push_back(vecIn);
//
//		fscanf(vecFile, "%c", &ch);	
//		while ((ch == ' ') || (ch == '\n'))
//			fscanf(vecFile, "%c", &ch);
//	}
//
//	cout << "Read " << numVec << " vectors" << endl;
//	fclose(vecFile);
//}
