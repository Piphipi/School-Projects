#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void loadImage (ifstream& inFile1, int numRows, int numCols, int** mirrorFramedAry){//works
	if (inFile1.is_open()){
		int read;
			for(int i = 0; i < numRows; i++){
				for(int j = 0; j < numCols; j++){
					inFile1 >> read;
					mirrorFramedAry [i + 2][j + 2] = read;
				}
			}
	}
	else cout << "Error: Unable to open a file"; 
}

int loadMaskAry(ifstream& inFile2, int* maskAry){//works
	int sum = 0;
	if (inFile2.is_open()){
		int read;
		for(int i = 0; i < 25; i++){
			inFile2 >> read;
			maskAry [i] = read;
			sum += read;
		}
	}
	else cout << "Error: Unable to open a file"; 
	return sum;
}

void mirrorFraming (int** mirrorFramedAry, int numRows, int numCols){//extends outermost rows and colums to the 2 blank rings, corners, then columns, then rows
	//note for mirroring corners, [2][2] is upper left corner, [2][numCols + 1] is lower left corner, 
	//[numRows + 1][2] is upper right corner, and [numRows + 1][numCols + 1] is lower right corner
	for(int i = 0; i < numRows + 4; i++){//mirrors corners
		if(i == 0){//for left corners(outer/leftmost portion)
    		mirrorFramedAry[i][0] = mirrorFramedAry[2][2];//upper left corner
			mirrorFramedAry[i][1] = mirrorFramedAry[2][2];//upper left corner
			mirrorFramedAry[i][numCols + 2] = mirrorFramedAry[2][numCols + 1];//lower left corner
			mirrorFramedAry[i][numCols + 3] = mirrorFramedAry[2][numCols + 1];//lower left corner
    	}
    	else if(i == 1){//for left corrners(inner/second leftmost portion)
    		mirrorFramedAry[i][0] = mirrorFramedAry[2][2];//upper left corner
			mirrorFramedAry[i][1] = mirrorFramedAry[2][2];//upper left corner
			mirrorFramedAry[i][numCols + 2] = mirrorFramedAry[2][numCols + 1];//lower left corner
			mirrorFramedAry[i][numCols + 3] = mirrorFramedAry[2][numCols + 1];//lower left corner
    	}
    	else if(i == numRows + 2){//right corners(inner/second rightmost portion)
        	mirrorFramedAry[i][0] = mirrorFramedAry[numRows + 1][2];//upper right corner
        	mirrorFramedAry[i][1] = mirrorFramedAry[numRows + 1][2];//upper right corner
        	mirrorFramedAry[i][numCols + 2] = mirrorFramedAry[numRows + 1][numCols + 1];//lower right corner
        	mirrorFramedAry[i][numCols + 3] = mirrorFramedAry[numRows + 1][numCols + 1];//lower right corner
    	}
    	else if(i == numRows + 3){//for right corners(outer/rightmost portion)
        	mirrorFramedAry[i][0] = mirrorFramedAry[numRows + 1][2];//upper right corner
        	mirrorFramedAry[i][1] = mirrorFramedAry[numRows + 1][2];//upper right corner
        	mirrorFramedAry[i][numCols + 2] = mirrorFramedAry[numRows + 1][numCols + 1];//lower right corner
        	mirrorFramedAry[i][numCols + 3] = mirrorFramedAry[numRows + 1][numCols + 1];//lower right corner
    	}
	}
	for (int i = 2; i < numRows + 2; i++){//mirrors sidereel columns
		mirrorFramedAry[i][0] = mirrorFramedAry[i][2];
    	mirrorFramedAry[i][1] = mirrorFramedAry[i][2];
    	mirrorFramedAry[i][numCols+2] = mirrorFramedAry[i][numCols+1];
    	mirrorFramedAry[i][numCols+3] = mirrorFramedAry[i][numCols+1];
	}
	for (int i = 2; i < numCols + 2; i++){//mirrors sidereel rows
		mirrorFramedAry[0][i] = mirrorFramedAry[2][i];
		mirrorFramedAry[1][i] = mirrorFramedAry[2][i];
		mirrorFramedAry[numRows + 2][i] = mirrorFramedAry[numRows + 1][i];
		mirrorFramedAry[numRows + 3][i] = mirrorFramedAry[numRows + 1][i];
	}
	/*cout << "testing mirroring works correctly!" << endl;
	for(int i = 0; i < numRows + 4; i++){
		for(int j = 0; j < numCols + 4; j++){
			cout << mirrorFramedAry [i][j] << " ";
		}
		cout <<endl;
	}*/
}

void imgReformat (int** inAry, ofstream& outFile, int numRows, int numCols, int minVal, int maxVal){//good
	outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl; //step 1
	string str = to_string(maxVal); //step 2
	int width = str.length();
	int r;
	int c;
	int WW;
	r = 2; //step 3
	while (r < numRows + 2){ //step 12
		c = 2; //step 4
		while (c < numCols + 2){ //step 10
			outFile << inAry[r][c]; //step 5
			str = to_string(inAry[r][c]); //step 6
			WW = str.length();
			while (WW < width + 1){ //step 8 might have to do width +1
				outFile << " "; //step 7
				WW++;
			}
			c++; //step 9
		}
		outFile << endl;
		r++; //step 11
	}
}

void loadNeighborAry (int** mirrorFramedAry, int row, int col, int* neighborAry){//good
	int k = 0;
	for (int i = row - 2; i < row + 3; i++){
		for (int j = col - 2; j < col + 3; j++){
			neighborAry[k] = mirrorFramedAry[i][j];
			k++;
		}
	}
}

void sort (int* nieghborAry){//good
	for(int i = 0; i < 25; i++){		
		for(int j= i + 1; j < 25; j++){
			if(nieghborAry[i] > nieghborAry[j]){
				int temp = nieghborAry[i];
				nieghborAry[i] = nieghborAry[j];
				nieghborAry[j] = temp;
			}
		}
	}
}

int convolution(int* nieghborAry, int* maskAry, int weight){//looks good
	int result = 0; //step 1
	for (int i = 0; i < 25; i++){ //step 3/4
		result += nieghborAry[i] * maskAry[i]; //step 2
	}
	return (result / weight); //step 5
}

void computeAvg(int** mirrorFramedAry, int** avgAry, int numRows, int numCols, int* neighborAry){//good
	int avg = 0;
	for (int i = 2; i < numRows + 2; i++){
		for (int j = 2; j < numCols + 2; j++){
			loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
			int sum = 0;
			for (int k = 0; k < 25; k++){
				sum += neighborAry[k];
			}
			avgAry[i][j] = sum/25;
		}
	}
}

void computeMedian (int** mirrorFramedAry, int** medianAry, int numRows, int numCols, int* neighborAry){//good
	int i = 2; //step 1
	int j;
	while (i < (numRows + 2)){ //step 9
		j = 2; //step 2
		while (j < (numCols + 2)){ //step 7
			loadNeighborAry(mirrorFramedAry, i, j, neighborAry); //step 3
			sort(neighborAry); //step 4
			medianAry[i][j] = neighborAry[12]; //step 5
			j++; //step 6
		}
		i++; //step 8
	}
}

void computeGauss (int** mirrorFramedAry, int** gaussAry, int numRows, int numCols, int maskWeight, int* neighborAry, int* maskAry){//img1 works, img2 looks good
	int i = 2; //step 1
	while (i < numRows + 2){ //step 8
		int j = 2; //step 2
		while (j < numCols + 2){ //step 6
			loadNeighborAry(mirrorFramedAry, i , j, neighborAry); //step 3
			gaussAry[i][j] = convolution(neighborAry, maskAry, maskWeight); //step 4
			j++; //step 5
		}
		i++; //step 7
	}
}

void binaryThreshold (int** inAry, int** outAry, int numRows, int numCols, int thrVal){//good
	for(int i = 0; i < numRows + 4; i++){
		for (int j = 0; j < numCols + 4; j++){
			if (inAry[i][j] >= thrVal){
				outAry[i][j] = 1;
			}
			else{
				outAry[i][j] = 0;
			}
		}
	}
	/*for(int i = 0; i < numRows + 4; i++){
		for (int j = 0; j < numCols + 4; j++){
			cout << outAry[i][j];
		}
		cout << endl;
	}*/
}

void prettyPrint (int** thrAry, ofstream& outFile, int numRows, int numCols){//good
	for(int i = 0; i < numRows + 4; i++){
		for (int j = 0; j < numCols + 4; j++){
			if (thrAry[i][j] > 0){
				outFile << thrAry[i][j] << " ";
			}
			else{
				outFile << "  ";
			}
		}
		outFile << endl;
	}
}

int main (int argc, char **argv) {
	ifstream inFile1; //variables
	ifstream inFile2;
	ofstream avgOutFile;
	ofstream medOutFile;
	ofstream gaussOutFile;
	ofstream debugFile;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int maskRows;
	int maskCols;
	int maskMin;
	int maskMax;
	int thrVal;
	int maskWeight = 0;
	inFile1.open(argv[1]); //step 0
	inFile2.open(argv[2]);
	thrVal = atoi(argv[3]);
	avgOutFile.open(argv[4]);
	medOutFile.open(argv[5]);
	gaussOutFile.open(argv[6]);
	debugFile.open(argv[7]);
	if (inFile1.is_open()){ //step 1
		inFile1 >> numRows;
		inFile1 >> numCols;
		inFile1 >> minVal;
		inFile1 >> maxVal;
	}
	if (inFile2.is_open()){ //step 1
		inFile2 >> maskRows;
		inFile2 >> maskCols;
		inFile2 >> maskMin;
		inFile2 >> maskMax;
	}
	else cout << "Error: Unable to open a file and/or Header read error";
	int **mirrorFramedAry = new int*[numRows + 4];//creates 1d array of size numRows+4
    for(int i = 0; i < numRows + 4; ++i) {
        mirrorFramedAry[i] = new int[numCols + 4];//makes 1d array into 2d array
    }
    for(int i = 0; i < numRows + 4; ++i) {
    	for (int j  = 0; j < numCols + 4; j++){
    		mirrorFramedAry[i][j] = -1; //set array to -1
		}
    }
	int **avgAry = new int*[numRows + 4];//creates 1d array of size numRows+4
    for(int i = 0; i < numRows + 4; ++i) {
        avgAry[i] = new int[numCols + 4]();//makes 1d array into 2d array initilized to 0
    }
	int **medianAry = new int*[numRows+ 4];//creates 1d array of size numRows+4
    for(int i = 0; i < numRows + 4; ++i) {
        medianAry[i] = new int[numCols + 4]();//makes 1d array into 2d array initilized to 0
    }
	int **gaussAry = new int*[numRows+ 4];//creates 1d array of size numRows+4
    for(int i = 0; i < numRows + 4; ++i) {
        gaussAry[i] = new int[numCols + 4]();//makes 1d array into 2d array initilized to 0
    }
	int **thrAry = new int*[numRows+ 4];//creates 1d array of size numRows+4
    for(int i = 0; i < numRows + 4; ++i) {
        thrAry[i] = new int[numCols + 4]();//makes 1d array into 2d array initilized to 0
    }
	int* neighborAry = new int[25](); //initilizes to 0
	int* maskAry = new int[25](); //initilizes to 0
	maskWeight = loadMaskAry(inFile2, maskAry); //step 3
	loadImage(inFile1, numRows, numCols, mirrorFramedAry); //step 4	
	mirrorFraming(mirrorFramedAry, numRows, numCols); // step 5
	imgReformat(mirrorFramedAry, debugFile, numRows, numCols, minVal, maxVal); //step 6
	
	computeAvg (mirrorFramedAry, avgAry, numRows, numCols, neighborAry); //step 7
	imgReformat (avgAry, debugFile, numRows, numCols, minVal, maxVal);
	binaryThreshold (avgAry, thrAry, numRows, numCols, thrVal);
	prettyPrint (thrAry, avgOutFile, numRows, numCols);
	
	computeMedian (mirrorFramedAry, medianAry,numRows, numCols, neighborAry); //step 8
	imgReformat (medianAry, debugFile, numRows, numCols, minVal, maxVal);
	binaryThreshold (medianAry, thrAry, numRows, numCols, thrVal);
	prettyPrint (thrAry, medOutFile, numRows, numCols);
	
	computeGauss (mirrorFramedAry, gaussAry, numRows, numCols, maskWeight, neighborAry, maskAry); //step 9
	imgReformat (gaussAry, debugFile, numRows, numCols, minVal, maxVal);
	binaryThreshold (gaussAry, thrAry, numRows, numCols, thrVal);
	prettyPrint (thrAry, gaussOutFile, numRows, numCols);
	
	inFile1.close(); //step 10
	inFile2.close();
	avgOutFile.close();
	medOutFile.close();
	gaussOutFile.close();
	debugFile.close();
	return 0;
}

