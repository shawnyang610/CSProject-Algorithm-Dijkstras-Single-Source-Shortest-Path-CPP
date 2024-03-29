//CSS700-34
//Shawn Yang

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class DijkstraSSS {
public:
	int numNodes, sourceNode, minNode, currentNode, newCost;
	int ** costMatrix;
	int *fatherAry;
	int *toDoAry;
	int *bestCostAry;
	ofstream *outfile1;
	ofstream *outfile2;
	DijkstraSSS(int inNumNodes, int inSourceNode, ofstream *inOutfile1, ofstream *inOutfile2) {
		outfile1 = inOutfile1;
		outfile2 = inOutfile2;
		numNodes = inNumNodes;
		sourceNode = inSourceNode;
		costMatrix = new int*[numNodes + 1];
		for (int i = 0; i < numNodes+1; i++) {
			costMatrix[i] = new int[numNodes + 1];
			for (int j = 0; j < numNodes + 1; j++) {
				if (i == j)
					costMatrix[i][j] = 0;
				else
					costMatrix[i][j] = 99999;
			}
		}
		fatherAry = new int [numNodes + 1];
		for (int i = 0; i < numNodes + 1; i++) {
			fatherAry[i] = i;
		}
		toDoAry = new int[numNodes + 1];
		for (int i = 0; i < numNodes + 1;  i++) {
			toDoAry[i] = 1;
		}
		bestCostAry = new int[numNodes + 1];
		for (int i = 0; i < numNodes+1; i++) {
			bestCostAry[i] = 99999;
		}
	}
	void dijkstras() {
		//step 2
		setBestCostAry(sourceNode);
		setFatherAry(sourceNode);
		setToDoAry(sourceNode);
		while (!isAllFinishedInToDoAry()) {
			//step 3
			minNode = findMinNode();
			markMinNode(minNode); //update toDoAry
			debugPrint();
			//step 4, 5, 6, 7
			compareAndUpdateLowestCost();
		} //step 8 while loop
		//step 9, 10		
		printAllShortestPaths();		
	}

	//function for step 4, 5, 6, 7
	void compareAndUpdateLowestCost() {
		for (int i = 1; i < numNodes + 1; i++) {
			if (toDoAry[i] == 1) {
				currentNode = i;
				newCost = computeCost(minNode, currentNode);
				if (newCost < bestCostAry[currentNode]) {
					changeCost(currentNode, newCost);
					changeFather(currentNode, minNode);
					debugPrint();
				}
			}

		}
	}
	void loadCostMatrix(int i, int j, int Cost) {
		costMatrix[i][j] = Cost;
	}
	void debugPrint() {
		*outfile2 << "\n-----------------------------------debug log-----------------------------------";
		*outfile2 << "\nsourceNode: " << sourceNode;

		*outfile2 << "\nfatherAry: ";
		for (int i = 1; i < numNodes + 1; i++) {
			*outfile2 << fatherAry[i] << " ";
		}
		
		*outfile2 << "\nbestCostAry: ";
		for (int i = 1; i < numNodes + 1; i++) {
			*outfile2 << bestCostAry[i] << " ";	
		}
		
		*outfile2 << "\ntoDoAry: ";
		for (int i = 1; i < numNodes + 1; i++) {
			*outfile2 << toDoAry[i] << " ";	
		}
		*outfile2 << endl;
	}
	void printCostMatrix() {
		*outfile2 << "costMatrix: \n-------------------------------------------------------------------------------\n";
		for (int i = 0; i < numNodes + 1; i++) {
			for (int j = 0; j < numNodes + 1; j++) {
				*outfile2 << costMatrix[i][j] << "      ";
			}
			*outfile2 << endl;
		}
	}
	void setBestCostAry(int inSourceNode) {
		for (int i = 1; i < numNodes + 1; i++) {
			bestCostAry[i] = costMatrix[inSourceNode][i];
		}
	}
	void setFatherAry(int inSourceNode) {
		for (int i = 1; i < numNodes+1; i++)
			fatherAry[i] = inSourceNode;
	}
	void setToDoAry(int inSourceNode) {
		toDoAry[inSourceNode] = 0;
	}
	int findMinNode() {
		int minIndex = -1;
		for (int i = 1; i < numNodes + 1; i++) {
			if (toDoAry[i] == 1) {
				minIndex = i;
				break;
			}
			
		}
		for (int i = 1; i < numNodes + 1; i++) {
			if (toDoAry[i] == 1) {
				if (bestCostAry[i] < bestCostAry[minIndex]) {
					minIndex = i;
				}
			}
		}
		if (minIndex == -1) {
			cout << "Error, no more node in toDoAry[i] has value 1" << endl;
			exit(0);
		}
		else
			return minIndex;
	}
	int computeCost(int inMinNode, int inCurrentNode) {
		return (bestCostAry[inMinNode] + costMatrix[inMinNode][inCurrentNode]);
	}
	void markMinNode(int inMinNode) {
		toDoAry[inMinNode] = 0;
	}
	void changeFather(int inCurrentNode, int inMinNode) {
		fatherAry[inCurrentNode] = inMinNode;
	}
	void changeCost(int inCurrentNode, int inNewCost) {
		bestCostAry[inCurrentNode] = inNewCost;
	}
	bool isAllFinishedInToDoAry() {
		bool status = true;
		for (int i = 1; i < numNodes + 1; i++) {
			if (toDoAry[i] == 1) {
				status = false;
			}
		}
		return status;
	}
	void printAllShortestPaths() {
		*outfile1 << "\n===========================================================\nThere are " 
			<< numNodes 
			<< " nodes in the input graph.\n===========================================================\n";
		*outfile1 << "Source node =" << sourceNode << endl;
		*outfile1 << "The shortest paths are:" << endl;
		int temp;
		for (int i = 1; i < numNodes + 1; i++) {
			temp = i;
			*outfile1 << "The path from " << sourceNode << " to " << i << " is: ";
			printShortestPath1(i);
			*outfile1 << endl;
		}
	}
	//traverse from target to source
	void printShortestPath1(int inNode) {
		int temp=inNode;
		*outfile1 <<inNode;
		while (temp != sourceNode) {
			temp = fatherAry[temp];
			*outfile1 << " <-- "<<temp;
		}
		*outfile1 << " cost = " << bestCostAry[inNode] << endl;
	}

	//traverse from source to target using backtrack
	void printShortestPath2(int inNode) {
		if (inNode == sourceNode){}
		else {
			printShortestPath2(fatherAry[inNode]);
			*outfile1 << inNode << " ";
		}
	}

};

int main(int argc, char *argv[])
{
	ifstream infile;
	ofstream outfile1;
	ofstream outfile2;
	int numNodes, sourceNode, i, j, cost;
	if (argc < 4) {
		cout << "Please retry and supply required file paths in the format of: input.txt output.txt output2.txt" << endl;
		return 1;
	}
	try {
		infile.open(argv[1]);
		outfile1.open(argv[2]);
		outfile2.open(argv[3]);
	} catch (const std::exception& e) {
	}
	cout << "input file opened: " << argv[1] << "\noutput file opened: " << argv[2] << "\noutput file opened: " << argv[3] << endl;
	//step 0, load numNodes and specify/load SourceNode(s). in this case, try all sourceNodes
	
	infile >> numNodes;
	infile >> sourceNode;
	//start new round of game
	DijkstraSSS *dijkstraSSS = new DijkstraSSS(numNodes, sourceNode, &outfile1, &outfile2);
	//step 1, loadMatrix
	while (infile >> i && infile >> j && infile >> cost) {
		dijkstraSSS->loadCostMatrix(i, j, cost);
	}

	//start dijks algorithm
	dijkstraSSS->dijkstras();

	infile.close();
	outfile1.close();
	outfile2.close();
    return 0;
}

