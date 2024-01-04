#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class listNode{
	public :string data;
    public :listNode *next;
    public :listNode(string& input){
        data = input;
        next = nullptr;
    }
    public :listNode(){;
    }
	friend class LLStack;
	friend class LLQueue;
	friend class RSort;
    void printNode (listNode* node, ofstream& outFile){
        if(node->next == nullptr){
            outFile << ("(" + node->data + ", NULL) --> NULL") << endl;
    	}
        else{
            outFile << ("(" + node->data + ", " + (node->next)->data + ") --> ");
        }
    }
};
class LLStack{
    public :listNode* top;
    public :LLStack(){
    	string def = "dummyNode";
        listNode* dummy = new listNode(def);
        top = dummy;
    }
    friend class LLQueue;
	friend class RSort;
	
    void push(string& input){
        listNode* newest = new listNode(input);
        newest->next = top;
		top = newest;
    }

    listNode* pop(){
        if(isEmpty()){
            exit(1);
        }
        else{
            listNode* del = top;
			top = top->next;
            del->next = nullptr;
            return del;
        }
    }

    bool isEmpty(){
        if(top->next == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    void print(ofstream& outFile){
    	listNode * temp;
    	temp = top;
    	outFile << ("Printing Stack S") << endl;
    	while(temp != nullptr){
    		temp->printNode(temp, outFile);
    		temp = temp->next;
		}
	}
};
class LLQueue{
    public :listNode* head;
	public :listNode* tail;
    public :LLQueue(){
    	string def = "dummyNode";
        listNode* dummy = new listNode(def);
        head = dummy;
        tail = dummy;
    }
	friend class RSort;

    void insert(string& input){
    	listNode* newest = new listNode(input);
		if (isEmpty()){
    		head->next = newest;
    		tail = newest;
		}
		else{
			tail->next = newest;
			tail = newest;
		}
        
    }

    listNode* deleteNode(){
        if(isEmpty()){
            exit(1);
        }
        else{
            listNode* ret;
            ret = head->next;
            head->next = ret->next;
            if (head->next == nullptr){
            	tail = head;
			}
            return ret;
        }
    }
 
    bool isEmpty(){
        if(tail->data == "dummyNode"){
            return true;
        }
        else{
            return false;
        }
    }

    void printQ(int table, int index, ofstream& outFile){
        outFile << ("Table [") << table << ("] [")<< index << ("]:");
		if (isEmpty()){
            head->printNode(head, outFile);
        }
        else{
            listNode* newest = head;
            while(newest->next != nullptr){
                newest->printNode(newest, outFile);
                newest = newest->next;
            }
            newest->printNode(newest, outFile);   
    	}
    }
};
class RSort{
	public :LLQueue** hashTable;
	public :string data;
	public :int currentTable, nextTable, longestStringLength, currentPosition;
	public :RSort(){
		hashTable = new LLQueue*[2];
		for (int i = 0; i < 2; i++){
			hashTable[i] = new LLQueue [256];
			for (int j = 0; j <256; j++){
				LLQueue newest;
				hashTable [i][j] = newest;
			}
		}
	}
	void firstReading(ifstream& inFile, ofstream& outFile){
		outFile << ("Preforming first Reading") << endl;
		longestStringLength = 0;
		if (inFile.is_open()){
    		string data;
			while(inFile >> data) {
				if(data.length() > longestStringLength){
					longestStringLength = data.length();
				}
			}
		}
		else cout << "Error: Unable to open a file"; 
	}
	string padString(string data){
		if (data.length() < longestStringLength){
			data.append((longestStringLength-data.length()),' ');
		}
		return data;
	}
	LLStack loadStack(ifstream& inFile, ofstream& outFile){
		outFile << ("Preforming loadStack!") << endl;
		LLStack S;
		if (inFile.is_open()){
    		string data;
    		string paddedData;
			while(inFile >> data) {
				paddedData = padString(data);
				S.push(paddedData);
				}
			}
		else cout << "Error: Unable to open a file"; 
		return S;
	}
	void moveStack(LLStack S, ofstream& outFile){
		outFile << ("Preforming moveStack") << endl;
		int index;
		char chr;
		string parse;
		while (!S.isEmpty()){
			listNode* node;
			node = S.pop();
			chr = getChar(node->data);
			index = (int) chr;
			hashTable[currentTable][index].insert(node->data);
		}
	}
	char getChar(string data){
		return data.at(currentPosition);
	}
	void printTable(int table, ofstream& outFile){
		LLQueue test;
		for(int i = 0; i < 256; i++){
			test = hashTable [table][i];
			if (!test.isEmpty()){
				test.printQ(table, i, outFile);
			}
		}
	}
	void printSortedData(int table, ofstream& outFile){
		LLQueue toPrint;
		for(int i = 0; i < 256; i++){
			toPrint = hashTable [table][i];
			toPrint.printQ(table, i, outFile);
		}
	}	
};
int main (int argc, char **argv) {
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	RSort R;
	R.firstReading(inFile, outFile2);
	inFile.close();
	inFile.open(argv[1]);
	LLStack S = R.loadStack(inFile, outFile2);
	S.print(outFile2);
	R.currentPosition = R.longestStringLength - 1;
	R.currentTable = 0;
	R.moveStack(S, outFile2);
	char chr;
	int index;
	while(R.currentPosition > 0){
		R.currentPosition--;
		R.nextTable = ((R.currentTable + 1) % 2);
		int currentQueue = 0;
		while(currentQueue < 256){		
			while (!(R.hashTable [R.currentTable][currentQueue].isEmpty())){
				listNode* node;
				node = R.hashTable[R.currentTable][currentQueue].deleteNode();
				chr = R.getChar(node->data);
				index = (int) chr;
				R.hashTable[R.nextTable][index].insert(node->data);
			}
			currentQueue++;
		}
		R.printTable(R.nextTable, outFile2);
		R.currentTable = R.nextTable;
	}
	R.printSortedData(R.nextTable, outFile1);
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}
