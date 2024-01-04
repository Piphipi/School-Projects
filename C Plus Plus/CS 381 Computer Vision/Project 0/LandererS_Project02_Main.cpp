#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void processing(ifstream& inFile, ofstream& outFile, int thrVal, int numCols){
	int col = 0;
	if (inFile.is_open()){
		int pixelVal; //step 0
		while(inFile >> pixelVal) { //step 1
    		if (pixelVal >= thrVal){ //step 2
    			outFile << pixelVal << " ";
    			col++;
			}
			else{ // step 2
				outFile << "0 ";
				col++;
			}
			if (col >= numCols){
				outFile << endl;
				col = 0;
			}	
		}
		
	}
	else cout << "Error: Unable to open a file"; 
}

int main (int argc, char **argv) {
	ifstream inFile; //variables
	ofstream outFile;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int thrVal;
	inFile.open(argv[1]); //step 0
	outFile.open(argv[2]);
	if (inFile.is_open()){ //step 1
		inFile >> numRows;
		inFile >> numCols;
		inFile >> minVal;
		inFile >> maxVal;
	}
	else cout << "Error: Unable to open a file and/or Header read error"; 
	cout << "Please enter a threashold value."; // step 2
	cin >> thrVal;
	outFile << numRows << " " << numCols << " 0 " << maxVal << endl; //step 3
	processing(inFile, outFile, thrVal, numCols); //step 4
	inFile.close(); //step 5
	outFile.close();
	return 0;
}

