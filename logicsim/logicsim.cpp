/* 
  	Name		: logicsim
 	Author		: Vineeth V Acharya
 	Description : Test Logic Simulator. Written 09/26/2014

*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <vector>
#include <fstream>

using namespace std;

const int ZERO = 0;
const int ONES = 1;
const int XVAL = -1;
const int DVAL = 3;
const int DBAR = -3;

typedef unsigned char uchar;
typedef unsigned int uint;

void printVec(const vector<int>&);

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

class circuit {

	int maxLevels;
	int numInputs;
	int numOutputs;
	int numFFs;

	vector<uchar> gtype;
	vector<short> fanin;
	vector<short> fanout;
	vector<int> levelNum;
	vector< vector<int> > inList;
	vector< vector<int> > outList;
	
	vector<int> value;		
	/* Can use 2 bit logic (ZERO: 00 | XVAL: 01 | ONES: 11) */
// 	vector<int> value1, value2;

	vector<int> dffList;	// stores gateIds of all FFs

	circuit();
	public:
	
	char cktName[80];
	int numGates;
	circuit(char*);
	int eval(int);	
	void logicSim(const vector<int>&);	
	void simulateVectors();

	void printValues();
	void printInputs();
	void printOutputs();
	void printCurrState();
	void printNextState();

	void readVecFile(vector< vector<int> >&);
};

bool INIT0 = false;
bool PRINT_INPUT = false;
bool PRINT_OUTPUT = false;
bool PRINT_CURR_STATE = false;
bool PRINT_NEXT_STATE = false;

int main(int argc, char **argv) {
	
	char cktName[80];
	if (argc < 2) {
		cout << "Enter circuit name\n"
			 << "Usage: " << argv[0] << " [-ios] <ckt>" << endl
			 << "The -i option initialize FFs from <ckt>.init file" << endl
			 << "The -o option prints the PO values" << endl
			 << "The -s option prints the PPO" << endl
			 << "The -a option prints input/output/state values" << endl;
		exit(-1);
	}
	else {
		for (int idx = 1; idx < argc; ++idx) {
			if (argv[idx][0] == '-') {
				for (int jdx = 1; jdx < strlen(argv[idx]); jdx++) {
					switch(argv[idx][jdx]) {
						case 'i':	INIT0 = true;
									break;

						case 'o':	PRINT_OUTPUT = true;
									break;

						case 's':	PRINT_NEXT_STATE = true;
									break;

						case 'a':	PRINT_NEXT_STATE = true;
									PRINT_INPUT = true;
									PRINT_OUTPUT = true;
									PRINT_CURR_STATE = true;
									break;

						default :	cout << "\nUnknown choice!" << argv[idx][jdx] 
										 << "\nExiting ... " << endl;
									exit(-1);
									break;
					}
				}
			}
			else {
				sprintf(cktName, "%s", argv[idx]);
				cout << "Using " << cktName << endl;
				break;
			}
		}
	}

	circuit *ckt = new circuit(cktName);	
	
	ckt->simulateVectors();

	delete ckt;

	return 0;
}

circuit::circuit(char *cktname) {
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
	
	value = vector<int>(numGates+1, XVAL);
	
	printf("Successfully read in circuit:\n");
	printf("\t%d PIs.\n", numInputs);
	printf("\t%d POs.\n", numOutputs);
	printf("\t%d FFs.\n", numFFs);
	printf("\t%d Total number of gates\n", numGates);
	printf("\t%d Levels in the circuit.\n", maxLevels / 5);
	
//	cout << "FFs: \n";
//	for (vector<int>::iterator it = dffList.begin();
//			it != dffList.end(); ++it) 
//		cout << *it << " ";
//	cout << endl;
}

void circuit :: simulateVectors() {
	
	assert(dffList.size() == (uint)numFFs);
	vector<int> dffValue (numFFs, XVAL);

	if (INIT0) {
		FILE* initFile;
		char initName[80];

		sprintf(initName, "%s.init", cktName);
		initFile = fopen(initName, "r");

		if (initFile == NULL) {
			fprintf(stderr, "Can't open %s file\n", initName);
			exit(-1);
		}
		
		char ch;
		for (int i = 0; i < numFFs; i++) {
			fscanf(initFile, "%c", &ch);
			if (ch == 'X') 
				dffValue[i] = XVAL;
			else
				dffValue[i] = ch - '0';
		}

		fclose(initFile);

	}
	for (int i = 0; i < dffList.size(); ++i) {
		int gate = dffList[i];
		value[inList[gate][0]] = dffValue[i];
	}

	vector< vector<int> > inputVec;
	readVecFile(inputVec);
	for (vector< vector<int> >::iterator vt = inputVec.begin(); 
			vt != inputVec.end(); ++vt) {
		if (PRINT_INPUT) {
			cout << "Input : ";
			printVec(*vt);
		}

		logicSim(*vt);
		//printValues();
	
		if (PRINT_OUTPUT)
			printOutputs();

		if (PRINT_CURR_STATE)
			printCurrState();
		
		if (PRINT_NEXT_STATE)
			printNextState();
		
		if (PRINT_NEXT_STATE)
			cout << endl;
	}
}

void printVec(const vector<int>& vecIn) {
	for (int idx = 0; idx < vecIn.size(); ++idx) {
		cout << vecIn[idx];
	}
	cout << endl;
}

void circuit :: printInputs() {
	cout << "Input : ";
	for (int idx = 1; idx <= numInputs; ++idx) {
		if (value[idx] == XVAL)
			cout << "X";
		else
			cout << value[idx];
	}
	cout << endl;
}

void circuit :: printCurrState() {
	cout << "P state : ";	
	for (int idx = 0; idx < dffList.size(); ++idx) {
		int gate = dffList[idx];
		if (value[gate] == XVAL)
			cout << "X";
		else
			cout << value[gate];
	}
	cout << endl;
}

void circuit :: printNextState() {
	cout << "N state : ";	
	for (int idx = 0; idx < dffList.size(); ++idx) {
		int gate = inList[dffList[idx]][0];
		if (value[gate] == XVAL)
			cout << "X";
		else
			cout << value[gate];
	}
	cout << endl;
}

void circuit :: printOutputs() {
	cout << "Output : ";
	for (int idx = 1; idx <= numGates; ++idx) {
		if (gtype[idx] == G_OUTPUT) {
			if (value[idx] == XVAL)
				cout << "X";
			else
				cout << value[idx];
		}
	}
	cout << endl;
}
			
void circuit :: printValues() {
	for (int gate = 1; gate <= numGates; ++gate) {
//		if (gtype[gate] != G_OUTPUT)
//			continue;

		if (value[gate] == XVAL)
			cout << "X";
		else
			cout << value[gate];
	}
	cout << endl;
}

void circuit :: logicSim(const vector<int>& vecIn) {
	
	int numLevels = maxLevels / 5;
	vector<int> tmp;
	vector< vector<int> >levelEvents(numLevels, tmp);
	
	assert(dffList.size() == (uint)numFFs);
	for (int i = 0; i < dffList.size(); ++i) {
		int gate = dffList[i];
		if (value[gate] != value[inList[gate][0]]) {
			value[gate] = value[inList[gate][0]];
			for(int j = 0; j < fanout[gate]; ++j) {
				int level = levelNum[outList[gate][j]];
				levelEvents[level/5].push_back(outList[gate][j]);
			}
		}
	}

	assert(vecIn.size() == (uint)numInputs);
	for (int gate = 1; gate <= numInputs; ++gate) {
		if (value[gate] != vecIn[gate-1]) {
			value[gate] = vecIn[gate-1];
			for(int j = 0; j < fanout[gate]; ++j) {
				int level = levelNum[outList[gate][j]];
				levelEvents[level/5].push_back(outList[gate][j]);
			}
		}
	}
	

	vector<int>* vecPtr;
	for (vector< vector<int> >::iterator vt = levelEvents.begin();
			vt != levelEvents.end(); ++vt) {
		for (vector<int>::iterator it = vt->begin(); it != vt->end(); ++it) {
			int prev_value = value[*it];
			eval(*it);
			if (value[*it] != prev_value) {
				for(int j = 0; j < fanout[*it]; ++j) {
					int level = levelNum[outList[*it][j]];
					levelEvents[level/5].push_back(outList[*it][j]);
				}
			}
		}
	}
}

int circuit::eval(int gate) {
	
	int opVal;
	bool xFlag;
	switch (gtype[gate]) {

		case G_INPUT:
			break;
			
		case G_OUTPUT:
			opVal = value[inList[gate][0]];
			break;
			
		case G_XOR:
			opVal = 0;
			xFlag = false;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == XVAL) {
					xFlag = true;
					break;
				}
				else if (value[inList[gate][i]] == 1)
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
				if (value[inList[gate][i]] == XVAL) {
					xFlag = true;
					break;
				}
				else if (value[inList[gate][i]] == 1)
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
				if (value[inList[gate][i]] == 0) {
					opVal = 0;
					break;
				}
				else if (value[inList[gate][i]] == XVAL)
					opVal = XVAL;
			}
			break;
				 
		case G_NAND:
			opVal = 0;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == 0) {
					opVal = 1;
					break;
				}
				else if (value[inList[gate][i]] == XVAL)
					opVal = XVAL;
			}
			break;
			
		case G_OR:
			opVal = 0;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == 1) {
					opVal = 1;
					break;
				}
				else if (value[inList[gate][i]] == XVAL)
					opVal = XVAL;
			}
			break;
			
		case G_NOR:
			opVal = 1;
			for (int i = 0; i < fanin[gate]; i++) {
				if (value[inList[gate][i]] == 1) {
					opVal = 0;
					break;
				}
				else if (value[inList[gate][i]] == XVAL)
					opVal = XVAL;
			}
			break;

		case G_NOT:
			if (value[inList[gate][0]] == XVAL)
				opVal = XVAL;
			else if (value[inList[gate][0]] == 0)
				opVal = 1;
			else
				opVal = 0;
			break;

		case G_BUF:
			opVal = value[inList[gate][0]];
			break;

		default:
			printf("\nUnknown gate type. Aborting Evaluation");
			return -1;
	}

	if((gtype[gate] != G_INPUT) && (gtype[gate] != G_DFF))
		value[gate] = opVal;

	return 0;
}

void circuit :: readVecFile(vector< vector<int> >& inputVec) {

    inputVec.clear();

    char vecName[100];
    sprintf(vecName, "%s.vec", cktName);

	FILE *vecFile;
	vecFile = fopen(vecName, "r");
	if (vecFile == NULL) {
		fprintf(stderr, "Can't open %s.vec file\n", vecName);
		exit(-1);
	}
	
	int vecWidth;
	fscanf(vecFile, "%d", &vecWidth); // number of gates
	assert(vecWidth == numInputs);

	char ch;
	fscanf(vecFile, "%c", &ch);
	while ((ch == ' ') || (ch == '\n'))
		fscanf(vecFile, "%c", &ch);
	
	int numVec = 0;
	while (ch != 'E') {
		vector<int> vecIn(vecWidth);
		if (ch == 'X')
			vecIn[0] = XVAL;
		else
			vecIn[0] = ch - 48;

		for (int i = 1; i < vecWidth; ++i) {
			fscanf(vecFile, "%c", &ch);
			if (ch == 'X')
				vecIn[i] = XVAL;
			else
				vecIn[i] = ch - 48;
		}

		numVec++;	
		inputVec.push_back(vecIn);

		fscanf(vecFile, "%c", &ch);	
		while ((ch == ' ') || (ch == '\n'))
			fscanf(vecFile, "%c", &ch);
	}

	cout << "Read " << numVec << " vectors" << endl;
	fclose(vecFile);
}
