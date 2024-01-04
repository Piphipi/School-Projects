#include <iostream>
#include <fstream>
using namespace std;
class bucketSort{
    public :int min, max, lastIndex;
    public :int * BucketAry;
 	void findMinMax(ifstream& inFile){
		if (inFile.is_open()){
   			int num;
   			bool first = false;
			while(inFile >> num) {
				if(!first){
					min = num;
					max = num;
					first = true;
				}
				else if(num < min){
					min = num;
				}
				else if(num > max){
					max = num;
				}
			}
		}
		else cout << "Error: Unable to open a file"; 
	}
    public :bucketSort(ifstream& inFile){
    	findMinMax(inFile);
    	lastIndex = (max - min);
    	BucketAry = new int[lastIndex + 1]();
    	
    }

    int getIndex(int data){
    	return (data - min);
	}
	void printSortData(ofstream& outFile){
		for (int i = 0; i < lastIndex + 1; i++){
			if (BucketAry[i] != 0){
				while(BucketAry[i] != 0){
					outFile << (min + i) << endl;
					BucketAry[i]--;
				}
			}
		}
	}
};

 
int main (int argc, char **argv) {
	ifstream inFile;
	ofstream outFile;
	inFile.open(argv[1]);
	outFile.open(argv[2]);
	bucketSort b(inFile);
	inFile.close();
	inFile.open(argv[1]);
	if (inFile.is_open()){
    	int num;
    	int index;
		while(inFile >> num) {
			index = b.getIndex(num);
			b.BucketAry[index]++;
		}
	}
	else cout << "Error: Unable to open a file"; 
	b.printSortData(outFile);
	delete b.BucketAry;
	inFile.close();
	outFile.close();
	return 0;
}
