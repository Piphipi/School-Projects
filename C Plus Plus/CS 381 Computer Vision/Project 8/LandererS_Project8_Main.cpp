#include <iostream>
#include <fstream>
#include <string>
using namespace std;
	int numRows;//globals
	int numCols;
	int minVal;
	int maxVal;
	int numStructRows;
	int numStructCols;
	int structMin;
	int structMax;
	int rowOrigin;
	int colOrigin;
	int* HPP;
	int* VPP;
	int* binHPP;
	int* binVPP;
	int* morphHPP;
	int* morphVPP;
	int* structElem = new int[3];
	
	
class boxNode{
	public :int boxType;
    public :int minR;
    public :int minC;
    public :int maxR;
    public :int maxC;
    public :boxNode* next;
};

void loadImage (ifstream& inFile1, int** imgAry){
	if (inFile1.is_open()){
		int read;
		for(int i = 0; i < numRows; i++){
			for(int j = 0; j < numCols; j++){
				inFile1 >> read;
				imgAry [i + 1][j + 1] = read;
			}
		}
	}
	else cout << "Error: Unable to open a file"; 
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

void computePP (int** inAry){
	for (int i = 0; i < numRows + 2; i++){
		for (int j = 0; j < numCols + 2; j++){
			if (inAry[i][j] > 0){
				HPP[i]++;
				VPP[j]++;
			}
		}
	}
}

void binaryThreshold (int* inAry, int* outAry, int rc, int thrVal){//good
	for(int i = 0; i < rc + 2; i++){
		if (inAry[i] >= thrVal){
			outAry[i] = 1;
		}
		else{
			outAry[i] = 0;
		}	
	}
}

void printPP (int* PP, int rc, ofstream& outFile){
	for (int i = 0; i < rc + 2; i++){
		outFile << PP[i];
		outFile << " ";
	}
	outFile << "\n";
}

boxNode* computeZoneBox (int* bHPP, int* bVPP){
	int minR = 1; //step 0
	int minC = 1;
	int maxR = numRows;
	int maxC = numCols;
	while (bHPP[minR] == 0 && minR <= numRows){ //step 2
		minR++; //step 1
	}
	while (bHPP[maxR] == 0 && maxR >= 1){ //step 4
		maxR--; //step 3
	}
	while (bVPP[minC] == 0 && minC <= numCols){ //step 6
		minC++; //step 5
	}
	while (bVPP[maxC] == 0 && maxC >= 1){ //step 8
		maxC--; //step 7
	}
	boxNode* B = new boxNode(); //step 9
	B->boxType = 1;
	B->minR = minR;
	B->maxR = maxR;
	B->minC = minC;
	B->maxC = maxC;
	B->next = NULL;
	return B; //step 10
}

void listInsert (boxNode* head, boxNode* nextN){
	nextN->next = head->next; //set curent first node to be after new node
	head->next = nextN; //put new node at the begging of the list
}

void printBox (boxNode* head, ofstream& outFile){
	boxNode* print = head;
	while (print->next != NULL){
		print = print->next;
		outFile << print->boxType << "\n" << print->minR << " " << print->minC;
		outFile << " " << print->maxR << " " << print->maxC << "\n";
	}
}

void dilation(int j, int* inAry, int* outAry, int* structElem){
    int jOffset = (j - colOrigin);
    int cIndex = 0;
    while(cIndex < numStructCols){
        if(structElem[cIndex] > 0){
            outAry[jOffset + cIndex] = 1;
        }
        cIndex++;
    }
}

void erosion(int j, int* inAry, int* outAry, int* structElem){
    int jOffset = (j - colOrigin);
    bool matchFlag = true;
    int cIndex = 0;
    while(cIndex < numStructCols && matchFlag == true){
        if(structElem[cIndex] > 0 && inAry[jOffset + cIndex] <= 0){
            matchFlag = false;
        }
        cIndex++;
    }
    if(matchFlag == true){
        outAry[j] = 1;
    }
    else{
        outAry[j] = 0;
    }
}

void morphClosing (int* PP, int* structElem, int* morph, int rc){ //slightly modified from Proj3
	int* temp = new int[rc + 2](); //vars
	int rowFrameSize = numStructRows/2;
	int colFrameSize = numStructRows/2;
	int rowSize = numRows + (rowFrameSize * 2);
	int colSize = numCols + (colFrameSize * 2);
	//compute errosion
    int j = colFrameSize;
    while(j < rc){
        if(PP[j] > 0){
            erosion(j, PP, temp, structElem);
        }
        j++;
    }
	//compute dilation
    j = colFrameSize;
    while(j < rc){
        if(temp[j] > 0){
            dilation(j, temp, morph, structElem);
        }
        j++;
    }
}

int computePPruns (int* PP, int lastIndex){
	int numRuns = 0; //step 0
	int index = 0;
	while (index <= lastIndex){ //step 7
		while (PP[index] == 0 && index <= lastIndex){ //step 2
			index++; //step 1
		}
		if (index > lastIndex){
			break;
		}
		while (PP[index] > 0 && index <= lastIndex){ //step 3+5
			index++; //setp 4
		}
		numRuns++; //step 6
	}
	return numRuns; //step 8
}

int computeReadingDirection (int runsHPP, int runsVPP, ofstream& outFile){
	int factor = 2; //step 0
	int direction = 0;
	if (runsHPP <= factor && runsVPP <= factor){ //step 1
		outFile << "the zone may be a non text zone\n";
	}
	else if (runsHPP >= factor * runsVPP){
		outFile << "the document reading direction is horizontal\n";
		direction = 1;
	}
	else if (runsVPP >= factor * runsHPP){
		outFile << "the document reading direction is vertical\n";
		direction = 2;
	}
	else{
		outFile << "the zone may be a non text zone\n";
	}
	return direction;//step 2
}

void computeHorizontalTextBox (boxNode* listHead, boxNode* zBox, int* PP, int lastIndex){
	int minR = zBox->minR; //step 0
	int minC = zBox->minC;
	int maxR = minR;
	int maxC = zBox->maxC;
	while (PP[maxR] == 0 && maxR <= lastIndex){ //step 2
		maxR++; //step 1
	}
	minR = maxR; //step 2
	while (minR <= lastIndex) { //step 10
		while (PP[maxR] > 0 && maxR <= lastIndex){ //step 4
			maxR++; //step 3
		}
		boxNode* B = new boxNode(); //step 5
		B->boxType = 2;
		B->minR = minR;
		B->minC = minC;
		B->maxR = maxR;
		B->maxC = maxC;
		listInsert(listHead, B);
		minR = maxR; //step 6
		while (PP[minR] == 0 && minR <= lastIndex){ //step 8
			minR++; //step 7
		}
		maxR = minR; //step 9
	}
}

void computeVerticalTextBox (boxNode* listHead, boxNode* zBox, int* PP, int lastIndex){
	int minR = zBox->minR; //step 0
	int minC = zBox->minC;
	int maxR = zBox->maxR;
	int maxC = minC;
	while (PP[maxC] == 0 && maxC <= lastIndex){ //step 2
		maxC++; //step 1
	}
	minC = maxC; //step 2
	while (minC <= lastIndex) { //step 10
		while (PP[maxC] > 0 && maxC <= lastIndex){ //step 4
			maxC++; //step 3
		}
		boxNode* B = new boxNode(); //step 5
		B->boxType = 2;
		B->minR = minR;
		B->minC = minC;
		B->maxR = maxR;
		B->maxC = maxC;
		listInsert(listHead, B);
		minC = maxC; //step 6
		while (PP[minC] == 0 && minC <= lastIndex){ //step 8
			minC++; //step 7
		}
		maxC = minC; //step 9
	}
}

void overlayBox (int** imgAry, boxNode* listHead){
	boxNode* overlay = listHead;
	while (overlay->next != NULL){//possibly add && overlay->next->boxType == 2
		overlay = overlay->next;
		int minRow = overlay->minR; //step 2
		int minCol = overlay->minC ;
		int maxRow = overlay->maxR;
		int maxCol = overlay->maxC;
		for(int i = minCol; i < maxCol + 1; i++){ //step 3
			imgAry[minRow][i] = 9;
		}
		for(int i = minCol; i < maxCol + 1; i++){
			imgAry[maxRow][i] = 9;
		}
		for(int i = minRow; i < maxRow + 1; i++){
			imgAry[i][minCol] = 9;
		}
		for(int i = minRow; i < maxRow + 1; i++){
			imgAry[i][maxCol] = 9;
		}
	}
}

int main (int argc, char **argv){
	boxNode* listHead;
	int thrVal;
	int runsHPP;
	int runsVPP;
	int readingDirection;
	ifstream inFile; //variables
	ifstream structFile;
	ofstream outFile1;
	ofstream outFile2;
	inFile.open(argv[1]); //step 0
	thrVal = atoi(argv[2]);
	structFile.open(argv[3]);
	outFile1.open(argv[4]);
	outFile2.open(argv[5]);
	if (inFile.is_open()){
		inFile >> numRows;
		inFile >> numCols;
		inFile >> minVal;
		inFile >> maxVal;
	}
	if (structFile.is_open()){
		structFile >> numStructRows;
		structFile >> numStructCols;
		structFile >> structMin;
		structFile >> structMax;
		structFile >> rowOrigin;
		structFile >> colOrigin;
		structFile >> structElem[0];
		structFile >> structElem[1];
		structFile >> structElem[2];
	}
	int **imgAry = new int*[numRows + 2];//creates 1d array of size numRows+2
    for(int i = 0; i < numRows + 2; ++i) {
        imgAry[i] = new int[numCols + 2]();//makes 1d array into 2d array initilized to 0
    }
    HPP = new int[numRows + 2]();//makes 1d array initilized to 0
    VPP = new int[numCols + 2]();//makes 1d array initilized to 0
    binHPP = new int[numRows + 2]();//makes 1d array initilized to 0
    binVPP = new int[numCols + 2]();//makes 1d array initilized to 0
    morphHPP = new int[numRows + 2]();//makes 1d array initilized to 0
    morphVPP = new int[numCols + 2]();//makes 1d array initilized to 0
    loadImage(inFile, imgAry); //step 1
    outFile1 << "Below is the input image\n";
    imgReformat(imgAry, outFile1);
    computePP(imgAry); //step 2
    outFile2 << "Below is HPP\n";
    printPP(HPP, numRows, outFile2);
    outFile2 << "Below is VPP\n";
    printPP(VPP, numCols, outFile2);
    binaryThreshold(HPP, binHPP, numRows, thrVal); //step 3
    binaryThreshold(VPP, binVPP, numCols, thrVal);
    outFile2 << "Below is binHPP\n";
    printPP(binHPP, numRows, outFile2);
    outFile2 << "Below is binVPP\n";
    printPP(binVPP, numCols, outFile2);
    listHead = new boxNode(); //step 4
    boxNode* zBox = computeZoneBox(binHPP, binVPP);
    listInsert(listHead, zBox);
    outFile2 << "Below is the linked list after input zone box\n";
    printBox(listHead, outFile2);
    morphClosing(binHPP, structElem, morphHPP, numRows); //step 5
    morphClosing(binVPP, structElem, morphVPP, numCols);
    outFile2 << "Below is morphHPP after performing morphClosing on HPP\n";
    printPP(morphHPP, numRows, outFile2);
    outFile2 << "Below is morphVPP after performing morphClosing on VPP\n";
    printPP(morphVPP, numCols, outFile2);
    runsHPP = computePPruns(morphHPP, numRows); //step 6
    runsVPP = computePPruns(morphVPP, numCols);
    outFile2 << "The number of runs in morphHPP - runs HPP is: " << runsHPP << "\n";
    outFile2 << "The number of runs in morphVPP - runs VPP is: " << runsVPP << "\n";
    readingDirection = computeReadingDirection(runsHPP, runsVPP, outFile1); //step 7
    outFile2 << "reading Direction is: " << readingDirection << "\n";
    if (readingDirection == 1){ //step 8
    	computeHorizontalTextBox(listHead, zBox, morphHPP, numRows);
	}
	else if (readingDirection == 2){ //step 8
    	computeVerticalTextBox(listHead, zBox, morphVPP, numCols);
	}
	overlayBox(imgAry, listHead); //step 9
	outFile1 << "Below is the input image overlay with bounding boxes\n"; //step 10
	imgReformat(imgAry, outFile1);
	outFile1 << "Output the boxNode in the list\n"; //step 11
	printBox(listHead, outFile1);
	inFile.close(); //step 12
	structFile.close();
	outFile1.close();
	outFile2.close();
}
