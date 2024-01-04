#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class node{
	public :int nodeID;
    public :node *next;
    public :node(int ID){
		if (ID <= 0){
			exit(1);
		}
		nodeID = ID;
        next = nullptr;
    }
   
};
class coloring{
	public :int numNodes;
	public :int numUncolor;
	public :node** hLList;
	public :int* colorARY;
	public :coloring(ifstream& inFile){
		if (inFile.is_open()){
			inFile >> numNodes;
		}
		numUncolor = numNodes;
		hLList = new node*[numNodes+1];
		for (int i = 0; i < (numNodes+1); i++){
			hLList[i] = nullptr;
		}
		colorARY = new int[numNodes+1];
		for (int i = 0; i < (numNodes+1); i++){
			colorARY[i] = 0;
		}
	}
	
	void loadGraph(ifstream& inFile){
		int i;
		int j;
		if (inFile.is_open()){
			while(inFile >> i) {
				inFile >> j;
				hashInsert(i,j);
				hashInsert(j,i);
			}
		}
	}
	
	void hashInsert(int ID1, int ID2){
		node* newNode = new node(ID2);
		node* last = hLList[ID1];
		if (last == nullptr){
			hLList[ID1] = newNode;
		}
		else{
			while (last->next != nullptr){
				last = last->next;
	 		}
	 		last->next = newNode;
		}
	}
	
	void printHashTable(ofstream& outFile){
		for (int i = 1; i < (numNodes + 1); i++){
			node* last;
			outFile << "hLList[" << i << "]";
			last = hLList[i];
			while (last != nullptr){
				outFile << " -> " << last->nodeID;
				last = last->next;
	 		}
	 		outFile << endl;
		}
	}
	
	void method1(ofstream& outFile1, ofstream& outFile2){
		int newColor = 0;
		while (numUncolor > 0){
			newColor++;
			int uncolorNodeID = 1;
			while (uncolorNodeID <= numNodes){
				if (colorARY[uncolorNodeID] == 0){
					if (checkNeighbors(uncolorNodeID, newColor)){
						colorARY[uncolorNodeID] = newColor;
						numUncolor--;
					}
				}	
				uncolorNodeID++;
			}
			printAry(outFile2, 1);
		}
		printAry(outFile1, 1);
	}
	
	void method2(ofstream& outFile1, ofstream& outFile2){
		int newColor = 0;
		int lastUsedColor = 0;
		int nextNodeID = 0;
		while (numNodes > nextNodeID){
			nextNodeID++;
			int nextUsedColor = 1;
			bool coloredFlag = false;
			while (coloredFlag == false && nextUsedColor <= lastUsedColor){
				if (lastUsedColor > 0 && checkNeighbors(nextNodeID, nextUsedColor)){
					colorARY[nextNodeID] = nextUsedColor;
					coloredFlag = true;
				}
				else{
					nextUsedColor++;
				}
			}
			if (coloredFlag == false){
				newColor++;
				colorARY[nextNodeID] = newColor;
				lastUsedColor++;
				outFile2 << "the new color is now " << newColor << " and the last used color is " << lastUsedColor << endl;
			}
			printAry(outFile2, 2);
		}
		printAry(outFile1, 2);
	}
	
	bool checkNeighbors(int ID, int color){
		node* nextNode = hLList[ID];
		while (nextNode != nullptr){
			if (nextNode == nullptr){
				return true;
			}
			if (colorARY[nextNode->nodeID] == color){
				return false;
			}
			nextNode = nextNode->next;
		}
		return true;
	}
	
	void printAry(ofstream& outFile, int method){
		outFile << "Method " << method << " was used to color the graph\n" << "Below is the result of the color assignments\n\n";
		outFile << numNodes << endl;
		for (int i = 1; i < (numNodes + 1); i++){
			outFile << i << " " << colorARY[i] << endl;
		}
	}
};
int main (int argc, char **argv) {
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[3]);
	outFile2.open(argv[4]);
	coloring c(inFile);
	c.loadGraph(inFile);
	c.printHashTable(outFile2);
	int whichMethod = atoi(argv[2]); 
	if (whichMethod == 1){
		c.method1(outFile1, outFile2);
	}
	else if (whichMethod == 2){
		c.method2(outFile1, outFile2);
	}
	else{
		outFile2 << "argv[2] only accepts 1 or 2";
		inFile.close();
		outFile1.close();
		outFile2.close();
		exit(1);
	}
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}
