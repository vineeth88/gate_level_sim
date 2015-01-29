#include "verInclude.h"
#include "rtLevelSim.h"
#include "incl.h"
#include "verilated.h"

char vecFile[100];
int printCnt(int_vec& vec);
void printVec(int_vec& vec_);

#ifdef MEM_ALLOC_DBG_ON
int state_t::mem_alloc_cnt = 0;
#endif

int main(int argc, char* argv[]) {

	Vtop *cktVar = new Vtop;
	int_vec branch_counters(NUM_BRANCH, 0);

	if (argc >= 2) {
        if (strcmp(argv[1], "-h") == 0) {
            cout << "Input format: " << endl
                << argv[0] << " "
                << argv[1] << " <ckt>.vec" << endl;
			exit(-1);
        }
        else {
            sprintf(vecFile, "%s", argv[1]);
            cout << "Reading " << vecFile << endl;
        }
	}		

	vector<vecIn_t> inputVec;
	rtLevelCkt *rtlCkt = new rtLevelCkt(cktVar); 
	sprintf(rtlCkt->cktName, "%s", vecFile);
	rtlCkt->readVecFile(inputVec);
	
	int_vec branch_index;
	for (int idx = 0; idx < inputVec.size(); ++idx) {
		rtlCkt->resetCounters();
		rtlCkt->simOneVector(inputVec[idx]);
		cout << rtlCkt->getCktState() << endl;

		rtlCkt->getCoverage(branch_index);
		for (int_vec_iter it = branch_index.begin(); 
				it != branch_index.end(); ++it) {
			branch_counters[*it]++;
			cout << *it << " ";
		}
		cout << endl;
	}

	int num = printCnt(branch_counters);
	cout << num << " branches reached" << endl;

}

int printCnt(int_vec& vec) {
	
	int num = 0;
	for (uint v = 0; v < vec.size(); ++v) {
		cout << v << "(" << vec[v] << ") ";
		if (vec[v])
			num++;
	}
	cout << endl;
	return num;
}

void printVec(int_vec& vec_) {
	for (int_vec_iter vt = vec_.begin();
			vt != vec_.end(); ++vt)
		cout << *vt << " ";
}
