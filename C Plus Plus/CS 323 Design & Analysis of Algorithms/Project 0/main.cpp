#include <iostream>
#include <fstream>
#include <string>
using namespace std;
void processInts(ifstream& inFile1, ofstream& outFile1){
	int perLine = 0;
	if (inFile1.is_open()){
    	int num;
		while(inFile1 >> num) {
			outFile1 << num << " ";	
			perLine++;
			if (perLine == 5){
				outFile1 << endl;
				perLine =0;
			}
		}
	}
	else cout << "Error: Unable to open a file"; 
}
void processWords(ifstream& inFile2, ofstream& outFile2){
	int perLine = 0;
	if (inFile2.is_open()){
    	string read;
		while(inFile2 >> read) {
			outFile2 << read << " ";	
			perLine++;
			if (perLine == 5){
				outFile2 << endl;
				perLine =0;
			}
		}
	}
	else cout << "Error: Unable to open a file"; 
}
int main (int argc, char **argv) {
	ifstream inFile1, inFile2;
	ofstream outFile1, outFile2;
	inFile1.open(argv[1]);
	inFile2.open(argv[2]);
	outFile1.open(argv[3]);
	outFile2.open(argv[4]);
	processInts(inFile1, outFile1);
	processWords(inFile2, outFile2);
	inFile1.close();
	outFile1.close();
	inFile2.close();
	outFile2.close();
	return 0;
}

