#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

int loadHist(int maxV, int* histAry, ifstream& inFile1){
	int max = 0;
	if (inFile1.is_open()){
		int read;
		int index;
		while(inFile1 >> read){//read left column
    		index = read;//set index of array to be left column
    		inFile1 >> read;//read right column
    		if(index <= maxV){	
				histAry[index] = read;//set value of array in possiton of the left colum to be the right column
			}
    		else break;
    		if (read > max){//if right column value is higher than current max
    			max = read;//overwirte max with right column value
			}
		}	
	}
	else cout << "Error: Unable to open a file"; 
	return max;
}

void dispHist(int* histAry, int numRows, int numCols, int minVal, int maxVal, ofstream& outFile){
	outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
	for (int i = 0; i < (maxVal + 1); i++){//for every entry in hisAry
		outFile << i << " (" << histAry[i] << "):";//print its position and the value in parenthasis
		for (int j = 0; j < histAry[i]; j++){//then for the total value
			outFile << "+";// print a +
		}
		outFile << endl;
	}
}

int deepestConcavity(int x1, int y1, int x2, int y2, int* histAry, ofstream& debugFile){
	debugFile << "Entering deepestConcavity method" << endl; //step 0
	double m = (double)((double) (y2 - y1)/((double)(x2 - x1)));
	double b = (double)(y1 - (m * (double) x1));
	int maxGap = 0;
	int first = x1;
	int second = x2;
	int x = first;
	int thr = first;
	int y;
	int gap;
	while (x <= second){ //step 6
		y = (int) (m * x + b); //step 1
		gap = abs((histAry[x] - y)); //step 3?
		if (gap > maxGap){ //step 4?
			maxGap = gap;
			thr = x;
		}
		x++; //step 5?
	}
	debugFile << "leaving deepestConcavity method, maxGap is " << maxGap << " and thr is " << thr << ";" << endl;
	return thr; //step 8
}

double computeMean(int leftIndex, int rightIndex, int* histAry, ofstream& debugFile){
	debugFile << "Entering computeMean method" << endl; //step 0
	int maxHeight = 0;
	int sum = 0;
	int numPixels = 0;
	int index = leftIndex; //step 1
	while (index < rightIndex){ //step 5
		sum += (histAry[index] * index); //step 2
		numPixels += histAry[index];
		if (histAry[index] > maxHeight){ //step 3
			maxHeight = histAry[index];
		}
		index++; //step 4
	}
	double result = (double) (((double)sum) / ((double) numPixels)); //step 6
	debugFile << "Leaving computeMean method, maxHeight is: " << maxHeight << " and result is: " << result << endl; //step 7
	return result; //step 8
}

double computeVar(int leftIndex, int rightIndex, double mean, int* histAry, ofstream& debugFile){
	debugFile << "Entering computeVar method" << endl; //step 0
	double sum = 0.0;
	int numPixels = 0;
	int index = leftIndex; //step 1
	while (index < rightIndex){ //step 4
		sum += (double)((double) histAry[index] * (((double) index - mean) * ((double) index - mean))); //step 2
		numPixels += histAry[index];
		index++; //step 3
	}
	double result = sum / ((double) numPixels); //step 5
	debugFile << "Leaving computeVar method returning result: " << result << endl; //step 6
	return result; //step 7
}

double modifiedGauss(int x, double mean, double var, int maxHeight){
	double square = (((double) x - mean) * ((double) x - mean));
	double forExp = square / (2 * var);
	double ret = maxHeight * exp(-forExp);
	return ret;
}

double fitGauss(int leftIndex, int rightIndex, int maxHeight, int* histAry, int* GaussAry, ofstream& debugFile){
	debugFile << "Entering fitGauss method" << endl; //step 0
	double mean;
	double var;
	double sum = 0.0;
	double Gval;
	double maxGval;
	mean = computeMean(leftIndex, rightIndex, histAry, debugFile); //step 1
	var = computeVar(leftIndex, rightIndex, mean, histAry, debugFile);
	int index = leftIndex; // step 2
	while (index <= rightIndex){ //step 7
		Gval = modifiedGauss(index, mean, var, maxHeight); //step 3
		sum += abs(Gval - (double)(histAry[index])); // step 4
		GaussAry[index] = (int) Gval; //step 5
		index++; //step 6
	}
	debugFile << "leaving fitGauss method, sum is " << sum << endl; //step 8
	return sum; //step 9
}

int biGaussian(int* histAry, int* GaussAry, int maxHeight, int minVal, int maxVal, ofstream& debugFile){
	debugFile << "Entering biGaussian method" << endl; //step 0
	double sum1;
	double sum2;
	double total;
	double minSumDiff;
	int offSet = (int) ((maxVal - minVal) / 10);
	int dividePt = offSet;
	int bestThr = dividePt;
	minSumDiff = 999999.0;
	while (dividePt < (maxVal - offSet)){ //step 8
		for(int i = 0; i < (maxVal + 1); i++){ //step 1
			GaussAry[i] = 0;
		}
		sum1 = fitGauss(0, dividePt, maxHeight, histAry, GaussAry, debugFile); //step 2
		sum2 = fitGauss(dividePt, maxVal, maxHeight, histAry, GaussAry, debugFile); //step 3
		total = sum1 + sum2; //step 4
		if (total < minSumDiff){ //step 5
			minSumDiff = total;
			bestThr = dividePt;
		}
		debugFile << "dividePt, sum1, sum2, total, minSumDiff, bestThr: " << dividePt << ", " << sum1 << ", " << sum2 << ", " << total << ", " << minSumDiff << ", " << bestThr << endl; //step 6
		dividePt++; //step 7
	}
	debugFile << "leaving biGaussian method, minSumDiff is: " << minSumDiff << ", bestThr is:" << bestThr << endl; //step 9
	return bestThr;
}

int main (int argc, char **argv) {
	ifstream inFile1; //variables
	ifstream inFile2; //variables
	ofstream outFile;
	ofstream debugFile;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int x1;
	int x2;
	int y1;
	int y2;
	int maxHeight;
	int deepestThrVal;
	int BiGaussThrVal;
	inFile1.open(argv[1]); //step 0
	inFile2.open(argv[2]);
	outFile.open(argv[3]);
	debugFile.open(argv[4]);
	if (inFile1.is_open()){ //step 1
		inFile1 >> numRows;
		inFile1 >> numCols;
		inFile1 >> minVal;
		inFile1 >> maxVal;
	}
	else cout << "Error: Unable to open a file and/or Header read error";
	if (inFile2.is_open()){ //step 1
		inFile2 >> x1;
		inFile2 >> y1;
		inFile2 >> x2;
		inFile2 >> y2;
	}
	else cout << "Error: Unable to open a file and/or read error";
	int* histAry = new int[maxVal+1](); //initilizes to 0
	maxHeight = loadHist(maxVal, histAry, inFile1);
	int* GaussAry = new int[maxVal+1](); //initilizes to 0
	dispHist(histAry, numRows, numCols, minVal, maxVal, outFile);
	outFile << "Peak one has x,y values of: " << x1 << "," << y1 << " and Peak two has x,y values of: " << x2 << "," << y2 << endl;
	deepestThrVal = deepestConcavity(x1, y1, x2, y2,histAry, debugFile);//step 3
	outFile << "The threshold for Deepest Concavity is: " << deepestThrVal << endl;
	BiGaussThrVal = biGaussian(histAry, GaussAry, maxHeight, minVal, maxVal, debugFile);//step 4
	outFile << "The threshold for BiGaussian Fit is: " << BiGaussThrVal << endl;
	
	inFile1.close(); //step 5
	inFile2.close();
	outFile.close();
	debugFile.close();
	return 0;
}

