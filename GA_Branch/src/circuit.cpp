#include "circuit.h"

void abstrCkt :: readVecFile(vector<int_vec>& inputVec) {

    inputVec.clear();

    char vecName[100];
    sprintf(vecName, "%s.vec", cktName);

	FILE *vecFile;
	vecFile = fopen(vecName, "r");
	if (vecFile == NULL) {
		fprintf(stderr, "Can't open %s file\n", vecName);
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
		int_vec vecIn(vecWidth);
		vecIn[0] = ch - '0';

		for (int i = 1; i < vecWidth; ++i) {
			fscanf(vecFile, "%c", &ch);
			vecIn[i] = ch - '0';
		}

		numVec++;	
		inputVec.push_back(vecIn);

		fscanf(vecFile, "%c", &ch);	
		while ((ch == ' ') || (ch == '\n'))
			fscanf(vecFile, "%c", &ch);
	}

	cout << "Read " << numVec << " vectors" << endl;
	fclose(vecFile);
};

void abstrCkt :: readVecFile(vector<vecIn_t>& inputVec) {

    inputVec.clear();

    char vecName[100];
    sprintf(vecName, "%s.vec", cktName);

	FILE *vecFile;
	vecFile = fopen(vecName, "r");
	if (vecFile == NULL) {
		fprintf(stderr, "Can't open %s file\n", vecName);
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
		vecIn_t vecIn(vecWidth, 'X');
		vecIn[0] = ch;

		for (int i = 1; i < vecWidth; ++i) {
			fscanf(vecFile, "%c", &ch);
			vecIn[i] = ch;
		}

		numVec++;	
		inputVec.push_back(vecIn);

		fscanf(vecFile, "%c", &ch);	
		while ((ch == ' ') || (ch == '\n'))
			fscanf(vecFile, "%c", &ch);
	}

	cout << "Read " << numVec << " vectors" << endl;
	fclose(vecFile);
};

//cktState :: cktState(const cktState& copy_obj) {
//	this->stateVal = copy_obj.stateVal;
//	this->stateIdx = copy_obj.stateIdx;
//}
//
//cktState :: cktState(const char stateStr[], int idx) {
//	this->stateIdx = idx;
//	this->stateVal = string(stateStr);
//}
//
//string cktState :: getState() const {	
//	return stateVal;	
//}
//
//void cktState :: setState(const string& st) {
//	if (st.length() == stateVal.length())
//		stateVal = st;
//}
//
//bool cktState :: operator[] (int idx) const {
//	assert((uint)idx < stateVal.length());
//	return stateVal[idx] - '0';
//}

//bool cktState :: operator== (const cktState& lhs) const {
//	return (stateVal.compare(lhs.getState()) == 0);
//}
