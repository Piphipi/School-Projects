#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class treeNode{
	public :string chrStr;
	public :int count;
	public :string code;
	public :treeNode *left;
	public :treeNode *right;
    public :treeNode *next;
    public :treeNode(string& chrs, int cnt, string cd, treeNode* lft, treeNode* rgt, treeNode* nxt){
        chrStr = chrs;
        count = cnt;
        code = cd;
        left = lft;
        right = rgt;
        next = nxt;
    }

    void printNode (treeNode* node, ofstream& outFile){
        outFile << ("(" + node->chrStr + ", ") << (node->count) << (", ") << (node->code) << (", ");
		if (node->left == nullptr){
			outFile << "NULL, ";
		}
		else{
			outFile << ((node->left)->chrStr) << ", ";
		}
		if(node->right == nullptr){
			outFile << "NULL, ";
		}
		else{
			outFile << ((node->right)->chrStr) << ", ";
		}
		if(node->next == nullptr){
			outFile << ("NULL)");
    	}
    	else{
    		outFile << ((node->next)->chrStr) << (")");
		}
    }
};
class LinkedList{
    public :treeNode* listHead;
    public :LinkedList(){
    	string def = "dummy";
        treeNode* dummy = new treeNode(def,0,"",nullptr,nullptr,nullptr);
        listHead = dummy;
    }
    
    bool isEmpty(){
        if(listHead->next == nullptr){
            return true;
        }
        else{
            return false;
        }
    }
    
    treeNode* findSpot(treeNode* head, treeNode* key){
        treeNode* spot = head;
        while(spot->next != nullptr && ((spot->next)->count < key->count)){
            spot = spot->next;
        }
        return spot;
    }
    
	void insertOneNode(treeNode* spot, treeNode* newNode){
        newNode->next = spot->next;
        spot->next = newNode;
	}
	
    void printList(ofstream& outFile){
    	treeNode* temp;
    	temp = listHead;
    	while(temp != nullptr){
    		temp->printNode(temp, outFile);
    		temp = temp->next;
    		if (temp == nullptr){
    			outFile << " --> NULL" << endl;
			}
			else{
				outFile << " -->" << endl;
			}
		}
	}
};
class Huffman{
	public :treeNode* Root;
	public :string codeTable[128];
	public :Huffman(){
		for(int i = 0; i < 128; i++){
			codeTable[i] = "";
		}
	}
	
	bool isLeaf(treeNode* node){
		if(node->left == nullptr && node->right == nullptr){
			return true;
		}
		else{
			return false;
		}
	}
	
	LinkedList constructHuffmanLList(ifstream& inFile, ofstream& outFile){
		string chr;
		int count;
		string empty = "";
		outFile << "Enter constructHuffmanLList() method" << endl;
		LinkedList L;
		if (inFile.is_open()){
			while(inFile >> chr) {
				inFile >> count;
				treeNode* newNode = new treeNode(chr, count, empty, NULL, NULL, NULL);
				treeNode* spot = L.findSpot(L.listHead, newNode);
				L.insertOneNode(spot, newNode);
				L.printList(outFile);
			}
		}
		else cout << "Error: Unable to open a file";
		outFile << "leaving constructHuffmanLList() method!" << endl;
		return L;
	}
	
	void constructHuffmanBinTree(LinkedList& list, ofstream& outFile){
		treeNode* spot;
		string conctString;
		outFile << "Enter constructHuffmanBinTree () method! " << endl;
		while((list.listHead->next)->next != nullptr){
			conctString = (list.listHead->next->chrStr + ((list.listHead->next)->next)->chrStr);
			treeNode* newNode = new treeNode(conctString, ((list.listHead->next)->count + ((list.listHead->next)->next)->count), "", list.listHead->next, (list.listHead->next)->next, nullptr);
			spot = list.findSpot(list.listHead, newNode);
			list.insertOneNode(spot,newNode);
			list.listHead->next = (((list.listHead->next)->next)->next);
			list.printList(outFile);
		}
		Root = list.listHead->next;
		outFile << "Leaving constructHuffmanBinTree () method!" << endl;
	}
	
	void constructCharCode(treeNode* T, string code, ofstream& outFile){
		int index;
		outFile << "In recursion of constructCodeTable(), code = " << code << endl;
		if(isLeaf(T)){
			T->code = code;
			string chr = T->chrStr;
			index = (int) chr.at(0);
			codeTable[index] = code;
		}
		else{
			constructCharCode(T->left, code + "0", outFile);
			constructCharCode(T->right, code + "1", outFile);
		}
	}
	
	void printCodeTable(ofstream& outFile){
		for(int i = 0; i < 128; i++){
			if(codeTable[i] != ""){
				outFile << (char) i << ", " << codeTable[i] << endl;
			}
		}
	}
	
	void preOrderTraversal(treeNode* T, ofstream& outFile){
		if (T != NULL){	
			T->printNode (T, outFile);
			outFile << endl;
			preOrderTraversal (T->left, outFile);
			preOrderTraversal (T->right, outFile);
		}
	}
	
	void inOrderTraversal(treeNode* T, ofstream& outFile){
		if (T != NULL){	
			inOrderTraversal (T->left, outFile);
			T->printNode (T, outFile);
			outFile << endl;
			inOrderTraversal (T->right, outFile);
		}
	}
	
	void postOrderTraversal(treeNode* T, ofstream& outFile){
		if(T != NULL){
			postOrderTraversal (T->left, outFile);
			postOrderTraversal (T->right, outFile);
			T->printNode(T, outFile);
			outFile << endl;
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
	Huffman H;
	LinkedList l = H.constructHuffmanLList(inFile, outFile1);
	H.constructHuffmanBinTree(l, outFile1);
	H.constructCharCode(H.Root, "", outFile1);
	H.printCodeTable(outFile2);
	H.preOrderTraversal(H.Root, outFile2);
	H.inOrderTraversal(H.Root, outFile2);
	H.postOrderTraversal(H.Root, outFile2);
	inFile.close();
	outFile1.close();
	outFile2.close();
	return 0;
}
