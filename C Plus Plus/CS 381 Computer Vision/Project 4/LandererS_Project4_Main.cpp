#include <iostream>
#include <fstream>
#include <string>
using namespace std;
	int numRows;//globals
	int numCols;
	int minVal;
	int maxVal;
	int trueNumCC;
	int newMin = 0;
	int newMax;
	int* nonZeroNeighborAry = new int[5](); //initilizes to 0
	
class property{
	public :int label;
    public :int numPixel;
    public :int minR;
    public :int minC;
    public :int maxR;
    public :int maxC;
};

void zero2D (int** Ary, int numRows, int numCols){//zeros 2D Array
	for (int i = 0; i < numRows; i++){
		for (int j = 0; j < numCols; j++){
			Ary[i][j] = 0;
		}
	}
}

void loadImage (ifstream& inFile1, int** zeroFramedAry){
	if (inFile1.is_open()){
		int read;
		for(int i = 0; i < numRows; i++){
			for(int j = 0; j < numCols; j++){
				inFile1 >> read;
				zeroFramedAry [i + 1][j + 1] = read;
			}
		}
	}
	else cout << "Error: Unable to open a file"; 
}

void conversion(int** inAry){
	for (int i = 0; i < numRows; i++){
		for (int j = 0; j < numCols; j++){
			if (inAry[i+1][j+1] == 0){
				inAry[i+1][j+1] = 1;
			}
			else if (inAry[i+1][j+1] == 1){
				inAry[i+1][j+1] = 0;
			}
		}
	}
}

void imgReformat (int** inAry, ofstream& outFile){
	//outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl; //step 1
	string str = to_string(maxVal); //step 2
	int width = str.length();
	int r;
	int c;
	int WW;
	r = 1; //step 3
	while (r < numRows + 1){ //step 12
		c = 1; //step 4
		while (c < numCols + 1){ //step 10
			if(inAry[r][c] == 0){
				outFile << ".";
			}
			else{
				outFile << inAry[r][c]; //step 5
			}
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

void printEQAry(int* EQAry, int label, ofstream& RFPP){
	RFPP << "The EQ array up to the current label (starting at 1) is:\n";
	for (int i = 1; i < label +1; i++){
		RFPP << EQAry[i] << " ";
	}
	RFPP << endl;
}

int manageEQAry(int* EQAry, int newLabel){
	int readLabel = 0; //step 0
	int index = 1; //step 1
	while (index <= newLabel){
		if (index != EQAry[index]){ //step 2
			EQAry[index] = EQAry[EQAry[index]];
		}
		else{
			readLabel++;
			EQAry[index] = readLabel;
		}
		index++;
	}
	return readLabel;
}

void connected4Pass1(int** framedAry, int &newLabel, int* EQAry){//test if this works
	for (int i = 1; i < numRows + 1; i++){
		for (int j = 1; j < numCols + 1; j++){
			if (framedAry[i][j] > 0){ //step 2
				if(framedAry[i-1][j] == 0 && framedAry[i][j-1] == 0){ //case 1
					newLabel++;
					framedAry[i][j] = newLabel;
				}
				else if (framedAry[i-1][j] > 0 || framedAry[i][j-1] > 0){ //case 2/3 at lease 1 neighbor is > 0
					if (framedAry[i-1][j] > 0){//above is > 0
						if (framedAry[i-1][j] == framedAry[i][j-1] || framedAry[i][j-1] == 0){ //and equal to left pixel or left pixel is 0 case 2
							framedAry[i][j] = framedAry[i-1][j]; //set current pixel to value of above pixel
						}
						else{//case 3
							if(framedAry[i-1][j] < framedAry[i][j-1]){ //min is above pixel
								framedAry[i][j] = framedAry[i-1][j]; //set current pixel to value of above pixel
								EQAry[framedAry[i][j-1]] = framedAry[i-1][j]; // set index of max neighbor to value of min neighbor
							}
							else{//min is left pixel
								framedAry[i][j] = framedAry[i][j-1]; //set current pixel to value of left pixel
								EQAry[framedAry[i-1][j]] = framedAry[i][j-1]; // set index of max neighbor to value of min neighbor
							}
						}
					}
					else{ //above is 0, left is > 0 case 2
						framedAry[i][j] = framedAry[i][j-1]; //set current pixel to value of left pixel
					}
				}
			}
		}
	}
}

void connected4Pass2(int** framedAry, int* EQAry){//test if this works
	int NNZN;
	bool SameNZN = true; //default
	int NZNZ;	
	for (int i = numRows; i > 0; i--){//right to left
		for (int j = numCols; j > 0; j--){//bottom to top
			NNZN = 0;
			if (framedAry[i][j+1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i][j+1];
				NNZN++;
			}
			if (framedAry[i+1][j] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i+1][j];
				NNZN++;
			}
			if (framedAry[i][j] > 0){ //step 2
				nonZeroNeighborAry[NNZN]= framedAry[i][j];
				NNZN++;
				if(NNZN == 1){ //case 1(all neighbors are zero)
					//do nothing
				}
				NZNZ = nonZeroNeighborAry[0]; //get value of first non zero neighbor
				if(NNZN >= 2){//at least 2 NZN
					for (int i = 1; i < NNZN; i++){ //check other non zero neighbors
						if(nonZeroNeighborAry[i] != NZNZ){//if any arent the same set the boolean to false
							SameNZN = false;
						}
					}
				}
				if (SameNZN){ //case 2
					//do nothing
				}
				else{ //case 3
					int minLabel = 9999;
					for (int i = 0; i < NNZN; i++){
						if (nonZeroNeighborAry[i] < minLabel){
							minLabel = nonZeroNeighborAry[i];
						}
					}
					if(framedAry[i][j] > minLabel){
						EQAry[framedAry[i][j]] = minLabel;
						framedAry[i][j] = minLabel;
					}
				}
			}
		}
	}
}

void connected8Pass1(int** framedAry, int &newLabel, int* EQAry){//test if this works(it does)
	int NNZN;
	bool SameNZN = true; //default
	int NZNZ;
	for (int i = 1; i < numRows + 1; i++){
		for (int j = 1; j < numCols + 1; j++){
			NNZN = 0;
			if (framedAry[i-1][j-1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i-1][j-1];
				NNZN++;
			}
			if (framedAry[i-1][j] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i-1][j];
				NNZN++;
			}
			if (framedAry[i-1][j+1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i-1][j+1];
				NNZN++;
			}
			if (framedAry[i][j-1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i][j-1];
				NNZN++;
			}
			if (framedAry[i][j] > 0){ //step 2
				if(NNZN == 0){ //case 1(all neighbors are zero)
					newLabel++;
					framedAry[i][j] = newLabel;
				}
				else{//case 2 or 3
					NZNZ = nonZeroNeighborAry[0]; //get value of first non zero neighbor
					if(NNZN >= 2){//at least 2 NZN
						for (int i = 1; i < NNZN; i++){ //check other non zero neighbors
							if(nonZeroNeighborAry[i] != NZNZ){//if any arent the same set the boolean to false
								SameNZN = false;
							}
						}
					}
					if (SameNZN){ //case 2
						framedAry[i][j] = NZNZ; //if all niehbors have the same label, set the selected pixel to that value(label of 0th non zero neighbor
					}
					else{ //case 3
						int minLabel = 9999;
						for (int i = 0; i < NNZN; i++){
							if (nonZeroNeighborAry[i] < minLabel){
								minLabel = nonZeroNeighborAry[i];
							}
						}
						framedAry[i][j] = minLabel;
						for (int i = 0; i < NNZN; i++){
							if (EQAry[nonZeroNeighborAry[i]] > minLabel){//if one of the nonZeroNeighbors is bigger than the min
								EQAry[nonZeroNeighborAry[i]] = minLabel;//set the EQ to the min
							}
						}
					}
				}
			}
		}
	}
}

void connected8Pass2(int** framedAry, int* EQAry){//test if this works(it does)
	int NNZN;
	bool SameNZN = true; //default
	int NZNZ;	
	for (int i = numRows; i > 0; i--){//right to left
		for (int j = numCols; j > 0; j--){//bottom to top
			NNZN = 0;
			if (framedAry[i][j+1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i][j+1];
				NNZN++;
			}
			if (framedAry[i+1][j-1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i+1][j-1];
				NNZN++;
			}
			if (framedAry[i+1][j] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i+1][j];
				NNZN++;
			}
			if (framedAry[i+1][j+1] > 0){ //filling nonZeroNeighborAry
				nonZeroNeighborAry[NNZN] = framedAry[i+1][j+1];
				NNZN++;
			}
			if (framedAry[i][j] > 0){ //step 2
				nonZeroNeighborAry[NNZN]= framedAry[i][j];
				NNZN++;
				if(NNZN == 1){ //case 1(all neighbors are zero)
					//do nothing
				}
				NZNZ = nonZeroNeighborAry[0]; //get value of first non zero neighbor
				if(NNZN >= 2){//at least 2 NZN
					for (int i = 1; i < NNZN; i++){ //check other non zero neighbors
						if(nonZeroNeighborAry[i] != NZNZ){//if any arent the same set the boolean to false
							SameNZN = false;
						}
					}
				}
				if (SameNZN){ //case 2
					//do nothing
				}
				else{ //case 3
					int minLabel = 9999;
					for (int i = 0; i < NNZN; i++){
						if (nonZeroNeighborAry[i] < minLabel){
							minLabel = nonZeroNeighborAry[i];
						}
					}
					if(framedAry[i][j] > minLabel){
						EQAry[framedAry[i][j]] = minLabel;
						framedAry[i][j] = minLabel;
					}
				}
			}
		}
	}
}

void connectPass3(int** zeroFramedAry, int* EQAry, property* &CCproperty, int trueNum, ofstream& debug){
	debug << "Entering connectPass3 method.\n"; //step 0
	for (int i = 1; i < trueNumCC + 1; i++){ //step 
		CCproperty[i].label = i;
		//cout << "Label:" << CCproperty[i].label << endl;
		CCproperty[i].numPixel = 0;
		//cout << "numPixel:"<<CCproperty[i].numPixel << endl;
		CCproperty[i].minR = numRows;
		//cout << "minR:"<<CCproperty[i].minR<< endl;
		CCproperty[i].maxR = 0;
		//cout << CCproperty[i].maxR << endl;
		CCproperty[i].minC = numCols;
		//cout << CCproperty[i].minC << endl;
		CCproperty[i].maxC = 0;
		//cout << CCproperty[i].maxR << endl;
	}
	for(int r = 0; r < numRows + 1; r++){ //step 2/4
		for (int c = 0; c < numCols + 1; c++){
			if(zeroFramedAry[r][c] > 0){ //step 3
				zeroFramedAry[r][c] = EQAry[zeroFramedAry[r][c]];//relabeling
				int k = zeroFramedAry[r][c];
				CCproperty[k].numPixel++;
				//cout << "numPixel for " << k << " is " << CCproperty[k].numPixel << endl;
				if (r < CCproperty[k].minR){
					CCproperty[k].minR = r;
					//cout << "new minR of" << k << " = " <<CCproperty[k].minR << endl;
				}
				if (r > CCproperty[k].maxR){
					CCproperty[k].maxR = r;
					//cout << "new maxR of" << k << " = " <<CCproperty[k].maxR << endl;
				}
				if (c < CCproperty[k].minC){
					CCproperty[k].minC = c;
					//cout << "new minC of" << k << " = " <<CCproperty[k].minC << endl;
				}
				if (c > CCproperty[k].maxC){
					CCproperty[k].maxC = c;
					//cout << "new maxC of" << k << " = " <<CCproperty[k].maxC << endl;
				}
			}
		}
	}
	debug << "leaving connectPass3 method.\n";
}

void connected4(int** inAry, int label, int* EQAry, property* &CCproperty, ofstream& RFPP, ofstream& debug){
	debug << "entering connected4 method.\n";
	connected4Pass1(inAry, label, EQAry);//step 1
	debug << "After connected4 pass1, newLabel =" << label << endl;
	RFPP << "Result of Pass 1\n";
	imgReformat(inAry, RFPP);
	RFPP << endl;
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	connected4Pass2(inAry, EQAry);//step2
	debug << "After connected4 pass2, newLabel =" << label << endl;
	RFPP << "Result of Pass 2\n";
	imgReformat(inAry, RFPP);
	RFPP << endl;
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	trueNumCC = manageEQAry(EQAry, label);//step 3
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	newMin = 0;
	newMax = trueNumCC;
	CCproperty = new property[trueNumCC+1];
	debug << "In connected4, after manage EQAry, trueNumCC =" << trueNumCC << endl;
	connectPass3(inAry, EQAry, CCproperty, trueNumCC, debug); //step 4
	RFPP << "Result of Pass 3\n";
	imgReformat(inAry, RFPP); //step 5
	RFPP << endl;
	printEQAry(EQAry, label, RFPP); //step 6
	RFPP << endl;
	debug << "Leaving connected4 method.\n"; //step 7
}

void connected8(int** inAry, int label, int* EQAry, property* &CCproperty, ofstream& RFPP, ofstream& debug){
	debug << "entering connected8 method.\n";
	connected8Pass1(inAry, label, EQAry);//step 1
	debug << "After connected8 pass1, newLabel =" << label << endl;
	RFPP << "Result of Pass 1\n";
	imgReformat(inAry, RFPP);
	RFPP << endl;
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	connected8Pass2(inAry, EQAry);//step2
	debug << "After connected8 pass2, newLabel =" << label << endl;
	RFPP << "Result of Pass 2\n";
	imgReformat(inAry, RFPP);
	RFPP << endl;
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	trueNumCC = manageEQAry(EQAry, label);//step 3
	RFPP << "EQ Table after management\n";
	printEQAry(EQAry, label, RFPP);
	RFPP << endl;
	newMin = 0;
	newMax = trueNumCC;
	CCproperty = new property[trueNumCC+1];
	debug << "In connected8, after manage EQAry, trueNumCC =" << trueNumCC << endl;
	connectPass3(inAry, EQAry, CCproperty, trueNumCC, debug); //step 4
	RFPP << "Result of Pass 3\n";
	imgReformat(inAry, RFPP); //step 5
	RFPP << endl;
	printEQAry(EQAry, label, RFPP); //step 6
	RFPP << endl;
	debug << "Leaving connected8 method." << endl; //step 7	
}

void printImg(int** inAry, ofstream& labelFile){
	string str = to_string(maxVal); //step 2
	int width = str.length();
	int r;
	int c;
	int WW;
	r = 1; //step 3
	while (r < numRows + 1){ //step 12
		c = 1; //step 4
		while (c < numCols + 1){ //step 10
			if(inAry[r][c] == 0){
				labelFile << ".";
			}
			else{
				labelFile << inAry[r][c]; //step 5
			}
			str = to_string(inAry[r][c]); //step 6
			WW = str.length();
			while (WW < width + 1){ //step 8 might have to do width +1
				labelFile << " "; //step 7
				WW++;
			}
			c++; //step 9
		}
		labelFile << endl;
		r++; //step 11
	}
}

void drawBoxes(int** framedAry, property* CCproperty, int trueNum){
	int index = 1; //step 1
	while (index <= trueNum){
		int minRow = CCproperty[index].minR; //step 2
		int minCol = CCproperty[index].minC;
		int maxRow = CCproperty[index].maxR;
		int maxCol = CCproperty[index].maxC;
		int label = CCproperty[index].label;
		for(int i = minCol; i < maxCol + 1; i++){ //step 3
			framedAry[minRow][i] = label;
		}
		for(int i = minCol; i < maxCol + 1; i++){
			framedAry[maxRow][i] = label;
		}
		for(int i = minRow; i < maxRow + 1; i++){
			framedAry[i][minCol] = label;
		}
		for(int i = minRow; i < maxRow + 1; i++){
			framedAry[i][maxCol] = label;
		}
		index++; //step 4
	}
}

void printCCProperty(property* CCproperty, ofstream& propertyFile){
	propertyFile << numRows << " " << numCols << " " << minVal << " " << trueNumCC << endl;
	propertyFile << trueNumCC << endl;
	for (int i = 1; i <= trueNumCC; i++){
		propertyFile << CCproperty[i].label << endl; //print the label
		propertyFile << CCproperty[i].numPixel << endl; //print the number of pixels
		propertyFile << CCproperty[i].minR - 1<< ", " << CCproperty[i].minC - 1<< endl; //print upper left corner
		propertyFile << CCproperty[i].maxR - 1<< ", " << CCproperty[i].maxC - 1<< endl; //print bottom right corner
	}
}

int main (int argc, char **argv) {
	char option;
	property* CCproperty;
	ifstream inFile; //variables
	int connectedness = 0;
	ofstream RFprettyPrintFile;
	ofstream labelFile;
	ofstream propertyFile;
	ofstream debugFile;
	inFile.open(argv[1]); //step 0
	connectedness = atoi(argv[2]);
	option = argv[3][0];
	RFprettyPrintFile.open(argv[4]);
	labelFile.open(argv[5]);
	propertyFile.open(argv[6]);
	debugFile.open(argv[7]);
	if (inFile.is_open()){
		inFile >> numRows;
		inFile >> numCols;
		inFile >> minVal;
		inFile >> maxVal;
	}
	int **zeroFramedAry = new int*[numRows + 2];//creates 1d array of size numRows+2
    for(int i = 0; i < numRows + 2; ++i) {
        zeroFramedAry[i] = new int[numCols + 2];//makes 1d array into 2d array
    }
    int* EQAry = new int[(numRows * numCols)/4](); //initilizes to 0
    for (int i = 0; i < (numRows * numCols)/4; i++){
    	EQAry[i] = i; // set each value to its index
	}
	int newLabel = 0;
	zero2D(zeroFramedAry, numRows + 2, numCols + 2);
	loadImage(inFile, zeroFramedAry);
	if(option == 'y' || option == 'Y'){
		conversion(zeroFramedAry);//make method
	}
	if (connectedness == 4){
		connected4(zeroFramedAry, newLabel, EQAry, CCproperty, RFprettyPrintFile, debugFile);//make method
	}
	else if (connectedness == 8){
		connected8(zeroFramedAry, newLabel, EQAry, CCproperty, RFprettyPrintFile, debugFile);//make method
	}
	else{
		cout << "Error: Incorect Arg in positon 2, connectedness.\n";
		return -1;
	}
	labelFile << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
	printImg(zeroFramedAry, labelFile);//make method
	printCCProperty(CCproperty, propertyFile);//make method. Prints cc properties to file !wrong values
	drawBoxes(zeroFramedAry, CCproperty, trueNumCC);
	imgReformat(zeroFramedAry, RFprettyPrintFile);
	RFprettyPrintFile << endl;
	RFprettyPrintFile << "The trueNumCC is: " << trueNumCC << endl;
	inFile.close(); //step 12
	RFprettyPrintFile.close();
	labelFile.close();
	propertyFile.close();
	debugFile.close();	
}
