#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class node{
	public :int jobID;
	public :int jobTime;
    public :node *next;
    public :node(int ID, int time){
		jobID = ID;
        jobTime = time;
        next = nullptr;
    }
   
    void printNode (node* pNode, ofstream& outFile){
        if(pNode->next == nullptr){
            outFile << ("(") <<(pNode->jobID) << (", NULL) --> NULL") << endl;
    	}
        else{
            outFile << ("(") << (pNode->jobID) << (", ") << ((pNode->next)->jobID ) << ( ") --> ");
        }
    }
};
class schedule{
	public :int numNodes;
	public :int numProcs;
	public :int procUsed;
	public :int currentTime;
	public :int totalJobTime;
	public :int* jobTimeAry;
	public :int** Matrix;
	public :int** Table;
	public :node* OPEN;
	public :initiSchedule(ifstream& inFile1, ifstream& inFile2, ofstream& outFile1, ofstream& outFile2 , int procs){
		if (inFile1.is_open()){
			while(inFile1 >> numNodes) {
				inFile2 >> numNodes;
				break;
			}
		}
		numProcs = procs;
		if (numProcs <= 0){
			outFile1 << "need 1 or more processors\n";
			return 1;
		}
		else if (numProcs > numNodes){
			numProcs = numNodes;
		}
		OPEN = new node(0, 0);
		currentTime = 0;
		procUsed = 0;
		Matrix = new int*[(numNodes+1)];
		for (int i = 0; i < (numNodes+1); i++){
			Matrix[i] = new int [(numNodes+1)];
			for (int j = 0; j < (numNodes+1); j++){
				Matrix [i][j] = 0;
			}
		}
		loadMatrix(inFile1);
		setMatrix();
		printMatrix(outFile2);
		jobTimeAry = new int[numNodes+1];
		for (int i = 0; i < (numNodes+1); i++){
			jobTimeAry[i] = 0;
		}
		totalJobTime = loadJobTimeAry(inFile2);
		Table = new int* [numProcs+1];
		for (int i = 0; i < (numProcs+1); i++){
			Table[i] = new int [totalJobTime+1];
			for (int j = 0; j < totalJobTime+1; j++){
				Table [i][j] = 0;
			}
		}
		printTable(outFile1);
	}
	
	void loadMatrix(ifstream& inFile){
		Matrix [0][0] = numNodes;
		int i;
		int j;
		if (inFile.is_open()){
			while(inFile >> i) {
				inFile >> j;
				Matrix[i][j] = 1;
			}
		}
	}
	
	int loadJobTimeAry(ifstream& inFile){
		int num;
		int time;
		int total;
		if (inFile.is_open()){
			while(inFile >> num) {
				inFile >> time;
				jobTimeAry[num] = time;
				total = total + time;
			}
		}
		return total;
	}
	
	void setMatrix(){
		for(int i = 1; i < numNodes+1; i++){
			for(int j = 1; j < numNodes+1; j++){
				if(Matrix[i][j] > 0){
					Matrix[i][0]++;
					Matrix[0][j]++;
				}
			}
			Matrix[i][i] = 1;
		}
	}
	
	void printMatrix(ofstream& outFile){
		outFile << endl;
		for (int i = 0; i < (numNodes+1); i++){
			for (int j = 0; j < (numNodes+1); j++){
				//outFile << ("Matrix[") << (i) <<("][") << (j) << ("] = ") << (Matrix[i][j]) << endl;
				/*if(i == -1){//print coloum numbers
					if (j!=-1){
						if (j > 9){
							outFile << j << " ";
						}
						else{
							outFile << j << "  ";
						}
					}
					else{
						outFile  << "     ";
					}
				}
				else if(j == -1){//print row numbers
					if (i!=-1){
						if (i > 9){
							outFile << i << "|  ";
						}
						else{
							outFile << i << " |  ";
						}
					}
				}*/
				//else{
					if (Matrix[i][j] > 9){//print values
						outFile << Matrix[i][j] << " ";
					}
					else{
						outFile << Matrix[i][j] << "  ";
					}
			}
			outFile << endl;
		}
		outFile << endl;
	}
	
	int findOrphan(){
		for (int j = 1; j < (numNodes+1); j++){
			if (Matrix[0][j] == 0 && Matrix [j][j] == 1){
				Matrix[j][j] = 2;
				return j;
			}
		}
		return -1;
	}
	
	void openInsert(node* newNode){
		node* spot = OPEN;
        while(spot->next != nullptr && ((spot->next)->jobID < newNode->jobID)){
            spot = spot->next;
        }
        newNode->next = spot->next;
        spot->next = newNode;
	}
	
	void printOPEN(ofstream& outFile2){
		node* temp;
    	temp = OPEN;
    	while(temp != nullptr){
    		temp->printNode(temp, outFile2);
    		temp = temp->next;
		}
	}
	
	int getNextProc (){
		for (int i = 1; i < numProcs+1; i++){
			if (Table[i][currentTime] == 0){
				return i;
			}
		}
		return -1;
	}
	
	void fillOPEN(ofstream& outFile2){
		int ID = 1;
		while (ID > 0){
			ID = findOrphan();
			if (ID > 0){
				node* newNode = new node(ID, jobTimeAry[ID]);
				openInsert(newNode);
				printOPEN(outFile2);
			}
		}
	}
	
	void fillTable(){
		int availProc = 0;
		while(availProc >= 0 && OPEN->next != nullptr && availProc < numProcs){
			availProc = getNextProc();
			if (availProc >= 0){
				node* newJob = OPEN->next;
				OPEN->next = newJob->next;
				putJobOnTable(availProc, newJob->jobID, newJob->jobTime);
				if (availProc > procUsed){
					procUsed++;
				}
			}
		}
	}
	
	void putJobOnTable(int availProc, int ID, int jobTime){
		int time = currentTime;
		int endTime = time + jobTime;
		while (time < endTime){
			Table[availProc][time] = ID;
			time++;
		}
	}
	
	void printTable(ofstream& outFile){
		outFile << ("ProcUsed: ") << (procUsed) << ("  currentTime: ") << (currentTime) << endl << endl;
		outFile << ("Time:\t");
		for(int i = 0; i <= currentTime; i++){
			outFile << ("| ") << (i) << (" \t");
		}
		outFile << endl;
		for (int i = 1; i <= procUsed; i++){
			outFile << ("Proc: ") << (i) << ("\t");
			for(int j = 0; j < (currentTime +1) && (j < totalJobTime+1); j++){
				int id = Table[i][j];
				if(id > 0){
					outFile << ("| ") << (id) << ("\t");
				}
				else{
					outFile << ("| - \t");
				}
			}
			outFile << endl;
		}
		outFile << endl;
	}
	
	bool checkCycle(){
		if(OPEN->next == nullptr && !isGraphEmpty()){
			for (int i =1; i <= numProcs; i++){
				if (Table[i][currentTime-1] > 0){
					return false;
				}
			}
			return true;
		}
	}
	
	bool isGraphEmpty(){
		if (Matrix[0][0] == 0){
			return true;
		}
		else{
			return false;
		}
	}
	
	void deleteDoneJobs(ofstream& outFile2){
		int proc = 1;
		while (proc <= procUsed){
			if (Table[proc][currentTime] <= 0 && Table[proc][currentTime-1] > 0){
				int ID = Table[proc][currentTime-1];
				deleteJob(ID);
			}
			printMatrix(outFile2);
			proc++;
		}
	}
	
	void deleteJob(int ID){
		Matrix[ID][ID] = 0;
		Matrix[0][0]--;
		int j = 1;
		while (j <= numNodes){
			if (Matrix[ID][j] > 0){
				Matrix[0][j]--;
			}
			j++;
		}
	}
	
};
int main (int argc, char **argv) {
	ifstream inFile1;
	ifstream inFile2;
	ofstream outFile1;
	ofstream outFile2;
	inFile1.open(argv[1]);
	inFile2.open(argv[2]);
	int numProcessor = atoi(argv[3]); 
	outFile1.open(argv[4]);
	outFile2.open(argv[5]);
	schedule s;
	s.initiSchedule(inFile1, inFile2, outFile1, outFile2, numProcessor);
	while(!(s.isGraphEmpty())){
		s.fillOPEN(outFile2);
		s.printOPEN(outFile2);
		s.fillTable();
		s.printTable(outFile1);
		s.currentTime++;
		s.deleteDoneJobs(outFile2);
		if(s.checkCycle()){
			outFile1 << ("There is a cycle in the graph!!!");
			return 1;
		}
	}
	s.printTable(outFile1);
	inFile1.close();
	inFile2.close();
	outFile1.close();
	outFile2.close();
	return 0;
}
