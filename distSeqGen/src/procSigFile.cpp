#include "incl.h"
#include <sstream>
#include <map>

using namespace std;

typedef map<string, int> stateHTable_t;

void genDetectedFaultsMap (char[], vector<char>&);
void procSig2 (char[], const vector<char>&, const stateHTable_t&); 
void procSig1 (char[], const vector<char>&, const stateHTable_t&);
void genHTableSig (char[], const vector<char>&, stateHTable_t&);

//struct sigPair_t {
//	int vecNo;
//	string sig;
//
//	sigPair_t(int v, string s) {
//		vecNo = v;
//		sig = s;
//	}
//};
bool dropDet;

int main(int argc, char** argv) {
	
	if (argc < 2) {
		cout << "provide fsig file" << endl;
	}
	dropDet = true;
	if (argc >= 3) {
		dropDet = atoi(argv[2]);
	}
	
	cout << "dropDet " << dropDet << endl;
	char cktName[256];
	sprintf(cktName, "%s", argv[1]);
	
	vector<char> detFaultMap;
	genDetectedFaultsMap(cktName, detFaultMap);
	genHTableSig(cktName, detFaultMap);
//	procSig2(cktName, detFaultMap);
//	procSig1(cktName, detFaultMap);
	return 0;
	
}

void genHTableSig (char cktName[], const vector<char>& detFaultMap) {
	
	char ffSigName[256];
	sprintf(ffSigName, "%s.fsig", cktName);
	ifstream ffSigFile;
	ffSigFile.open(ffSigName, ios::in);
	if (ffSigFile == NULL) {
		cerr << "Cant open " << ffSigName << " file" << endl;
		exit(-1);
	}

	typedef pair<stateHTable_t::iterator, bool> retVal_t;

	stateHTable_t stateTable;
	string ffStr;
	int Unique_State_ID = 0;
	int vecNo = 0;
	
	vector<int> ffSigVec;
	while(ffSigFile) {
		string line;
		getline(ffSigFile, line);
		
		int ind = line.find("vec");
		if (ind != string::npos) {

			int scIndex = line.find(":");
			stringstream ss;
			ss << line.substr(4,scIndex-4);
			ss >> vecNo;
			
			ffStr = line.substr(scIndex+2);
			retVal_t ret;
			ret = stateTable.insert(make_pair(ffStr, Unique_State_ID));

			if(ret.second) {
				ffSigVec.push_back(Unique_State_ID);
				++Unique_State_ID;
			}
			else {
				ffSigVec.push_back(ret.first->second);
			}
					
			assert(vecNo == ffSigVec.size()-1);
		}
		else {
			/*	FF Signature */
			int size = line.find(" ");	
			//cout << size;

			string sig = line.substr(0,size);

			int faultNo = 0;
			stringstream ss;
			ss << line.substr(size+1);
			ss >> faultNo;
			
			if (dropDet) {
				if ((faultNo < detFaultMap.size()) && (detFaultMap[faultNo]))
					continue;
			}

			retVal_t ret;
			ret = stateTable.insert(make_pair(sig, Unique_State_ID));
			if(ret.second) {
				++Unique_State_ID;
			}

//			sigPair_t currPair = make_pair(vecNo, sig);
//			vecMap_iter mapIt = faultSigMap.find(faultNo);
//			if (mapIt == faultSigMap.end()) {
//				vector<sigPair_t> tmpVec;
//				tmpVec.push_back(currPair);
//				retPair_t ret = faultSigMap.insert(make_pair(faultNo, tmpVec));
//				if (ret.second == false)
//					cout << "Unable to add V:" << vecNo 
//						 << "@ F: " << faultNo << endl;
//			}
//			else {
//				mapIt->second.push_back(currPair);
//			}
		}
	}

	ffSigFile.close();

	ofstream stateFile;
	char stateName[256];
	sprintf(stateName, "%s.ustb", cktName);
	stateFile.open(stateName, ios::out);
	if(stateFile == NULL) {	
		cout << "Unable to write into " << stateName << endl;
		for(stateHTable_t::iterator it = stateTable.begin(); 
				it != stateTable.end(); ++it)
			cout << it->first << " " << it->second << endl;
	}
	else {
		for(stateHTable_t::iterator it = stateTable.begin(); 
				it != stateTable.end(); ++it)
			stateFile << it->first << " " << it->second << endl;
	}
	
	cout << endl
		 << stateTable.size() << " Unique states" << endl;
	assert(stateTable.size() == Unique_State_ID);
}

void genDetectedFaultsMap (char cktName[], vector<char>& detFaultMap) {
	int maxFaultNo = 1;
	int numFaultsDet = 0;
//	vector<char> detFaultMap;

	char detName[256];
	sprintf(detName, "%s.det", cktName);
	ifstream detFile;
	detFile.open(detName, ios::in);
	if (detFile == NULL) {
		cerr << "Cant open " << detName << " file" << endl;
		exit(-1);
	}

	vector<int> detFaultList;
	while(detFile) {
		string line;
		getline(detFile, line);
		int ind = line.find(" ");
		stringstream ss;
		ss << line.substr(0,ind);
		int num;
		ss >> num;
		detFaultList.push_back(num);
	}
	detFile.close();

	maxFaultNo = detFaultList.back();
	numFaultsDet = detFaultList.size();
	detFaultMap = vector<char>(maxFaultNo, 0);

	for (vector<int>::iterator v = detFaultList.begin();
			v != detFaultList.end(); ++v) {
		detFaultMap[*v] = 1;
	}
	detFaultList.clear();

}

void procSig2 (char cktName[], const vector<char>& detFaultMap, const stateHTable_t& stateTable) {
	
	vector<string> ffSigVec;
	
	char ffSigName[256];
	sprintf(ffSigName, "%s.fsig", cktName);
	ifstream ffSigFile;
	ffSigFile.open(ffSigName, ios::in);
	if (ffSigFile == NULL) {
		cerr << "Cant open " << ffSigName << " file" << endl;
		exit(-1);
	}
	
	typedef pair<int, string> sigPair_t;
	typedef map<int, vector<sigPair_t> > vecMap_t;
	typedef vecMap_t::iterator vecMap_iter;

	typedef pair< vecMap_iter, bool> retPair_t;

	string ffStr;
	int vecNo = 0;
	vecMap_t faultSigMap;

	while(ffSigFile) {
		string line;
		getline(ffSigFile, line);
		
		int ind = line.find("vec");
		if (ind != string::npos) {

			int scIndex = line.find(":");
			stringstream ss;
			ss << line.substr(4,scIndex-4);
			ss >> vecNo;
			
			ffStr = line.substr(scIndex+2);
			ffSigVec.push_back(ffStr);
			assert(vecNo == ffSigVec.size()-1);
		}
		else {
			/*	FF Signature */
			int size = line.find(" ");	
			//cout << size;

			string sig = line.substr(0,size);

			int faultNo = 0;
			stringstream ss;
			ss << line.substr(size+1);
			ss >> faultNo;
			
			if (dropDet) {
				if ((faultNo < detFaultMap.size()) && (detFaultMap[faultNo]))
					continue;
			}

			sigPair_t currPair = make_pair(vecNo, sig);
			vecMap_iter mapIt = faultSigMap.find(faultNo);
			if (mapIt == faultSigMap.end()) {
				vector<sigPair_t> tmpVec;
				tmpVec.push_back(currPair);
				retPair_t ret = faultSigMap.insert(make_pair(faultNo, tmpVec));
				if (ret.second == false)
					cout << "Unable to add V:" << vecNo 
						 << "@ F: " << faultNo << endl;
			}
			else {
				mapIt->second.push_back(currPair);
			}
		}
	}
	ffSigFile.close();	

	int vec = 0;
	for(vector<string>::iterator it = ffSigVec.begin(); 
			it != ffSigVec.end(); ++it, vec++) 
		cout << vec << ": " << *it << endl;

	cout << endl;
//	#ifdef FAULT_DICT
	for(vecMap_iter it = faultSigMap.begin(); it != faultSigMap.end(); ++it) {
		cout << "Fault " << it->first << endl;
		for(vector<sigPair_t>::iterator st = it->second.begin(); 
				st != it->second.end(); ++st) {
			#ifdef REPRESENT_AS_D
			string ffStr = ffSigVec[st->first];
			string fvStr = st->second;
			for (int l = 0; l < fvStr.length(); ++l) {
				if (fvStr[l] == ffStr[l])
					cout << ffStr[l];
				else if ((fvStr[l] == '0') && (ffStr[l] == '1'))
					cout << "d";
				else
					cout << "b";
			}
			#endif
			cout //<< " (" 
				<< ffSigVec[st->first] << ", " << st->second 
				<< ", " << it->first 
				<< " " << st->first //<< ") " 
				<< endl;
		}
		cout << endl;
	}
//	#endif

}

void procSig1 (char cktName[], const vector<char>& detFaultMap) {

	typedef map<string, vector<int> > sigMap_t;
	typedef pair<sigMap_t::iterator, bool> retPair_t;
	
	char ffSigName[256];
	sprintf(ffSigName, "%s.fsig", cktName);
	ifstream ffSigFile;
	ffSigFile.open(ffSigName, ios::in);
	if (ffSigFile == NULL) {
		cerr << "Cant open " << ffSigName << " file" << endl;
		exit(-1);
	}

	string ffStr;
	int vecNo = -1;
	sigMap_t ffSigMap;

	while(ffSigFile) {
		string line;
		getline(ffSigFile, line);
		
		int ind = line.find("vec");
		if (ind != string::npos) {

			//Flush everything from previous vector
			if (vecNo >= 0) {
				
				cout << "FF(" << vecNo << ") " << endl
					 << ffStr << endl;
				for(sigMap_t::iterator sig_it = ffSigMap.begin(); 
						sig_it != ffSigMap.end(); ++sig_it) {
					cout << sig_it->first << " ";
					string fvStr = sig_it->first;
					int hamm_dist = 0;
					for (int l = 0; l < fvStr.length(); ++l) 
						hamm_dist += fvStr[l] ^ ffStr[l];
					cout << hamm_dist << ": ";
					for (vector<int>::iterator it = sig_it->second.begin(); 
							it != sig_it->second.end(); ++it)
						cout << *it << " ";
					cout << endl;
				}

				cout << " ...... " << endl;
				ffSigMap.clear();
			}

			int scIndex = line.find(":");
			//cout << 4 << "-" << scIndex << endl;
			stringstream ss;
			ss << line.substr(4,scIndex-4);
			ss >> vecNo;
//			cout << line.substr(4,scIndex-4) << endl;
			
			ffStr = line.substr(scIndex+2);
		}
		else {
			/*	FF Signature */
			int size = line.find(" ");	
			string sig = line.substr(0,size);

			int faultNo = 0;
			stringstream ss;
			ss << line.substr(size+1);
			ss >> faultNo;
			
			if (dropDet) {
				if ((faultNo < detFaultMap.size()) && (detFaultMap[faultNo]))
					continue;
			}

			sigMap_t::iterator sig_it = ffSigMap.find(sig);
			if (sig_it != ffSigMap.end()) {
				(sig_it->second).push_back(faultNo);
			}
			else {
				vector<int> tmpVec;
				tmpVec.push_back(faultNo);
				retPair_t ret = ffSigMap.insert(make_pair(sig, tmpVec));
			}
		}
	}
	
	ffSigFile.close();

	cout << "FF(" << vecNo << ") " << endl
		 << ffStr << endl;
	for(sigMap_t::iterator sig_it = ffSigMap.begin(); 
			sig_it != ffSigMap.end(); ++sig_it) {
		cout << sig_it->first << " ";
		string fvStr = sig_it->first;
		int hamm_dist = 0;
		for (int l = 0; l < fvStr.length(); ++l) 
			hamm_dist += fvStr[l] ^ ffStr[l];
		cout << hamm_dist << ": ";
		for (vector<int>::iterator it = sig_it->second.begin(); 
				it != sig_it->second.end(); ++it)
			cout << *it << " ";
		cout << endl;
	}

	cout << " ...... " << endl;
	ffSigMap.clear();
}
