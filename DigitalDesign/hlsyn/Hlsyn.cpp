/**************************************************************************************************/

/*
* File: Hlsyn.cpp
* Author: Miguel A. Garcia & Rubi Ballesteros
* NetID: magarcia1
* Assignment #: 3
* Date: December/10/2014
*
* Description: Hlsyn contains the required definitions of the Hlsyin class to successfully parse the content
* provided as an argument at the beginning of the execution of our program. In particular, we are interested in
* extracting the information about the Components and Dependencies existing within the file provided.
*
*/

/**************************************************************************************************/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <sstream>

/***********************************************************************/

#include "Hlsyn.h"
#include "Component.h"
#include "Inoutput.h"
//#include "graph.h"

using namespace std;

/***********************************************************************/

//Parse Components and Dependencies.
//If unable to read file, the vector will be of size 0 
bool ReadFromFile(Graph& myGraph, std::string filename){

	Inoutput* iov;
	istringstream inSS;
	fstream input;                     //Input file stream
	string lineString = "";
	string words[18];
	int myId = 0;
	int size = 0;
	int startId = 0, endId = 0;
	bool flag = true;

	bool InIFflag = false;

	Inoutput* currIFcondition;

	input.open(filename.c_str());

	if (input.is_open()) {
		while (!input.eof()){
			
			for (int i = 0; i < 18; i++) {
				words[i].clear();
			}

			getline(input, lineString);
			istringstream inStringS(lineString);

			//parse line
			inStringS >> words[0] >> words[1] >> words[2] >> words[3] >> words[4] >> words[5] >> words[6]
				>> words[7] >> words[8] >> words[9] >> words[10] >> words[11] >> words[12] >> words[13]
				>> words [14] >> words[15] >> words[16] >> words [17];
				
			//Ignore Empty Lines DONE----------------------------------------------------------------------
			if (words[0] == "") {
				continue;
			}
			//Ignore // (comments) DONE---------------------------------------------------------------------
			else if (words[0] == "//") {
				continue;
			}

			//Handle if statement---------------------------------------------------------------------------------
			else if(words[0]=="if"|| words[0] == "}"){
				//starting the if statement
				if (words[0] == "if") {
					//SAVE the if condition as an INOUTPUT currIFcondition
					currIFcondition = myGraph.getInput(words[2]);
					if (currIFcondition == NULL) {
						currIFcondition = myGraph.getOutput(words[2]);
					}

					if (currIFcondition == NULL) {
						cout << "Incorrect IF usage" << endl;
						return false;
					}

					//Check condition of parenthesis
					InIFflag = true;
					continue;
				}
				//The closing bracket
				else if (words[0] == "}") {
					InIFflag = false;
					continue;
				}
}
			
			//Detect Input, Output, or Variable Statement DONE-----------------------------------------------------
			else if ((words[0] == "input") || (words[0] == "output") || (words[0] == "variable")) {
				bool signedNum = true;

				//Set width length for signed------------------------
				if (words[1] == "Int2") {
					words[1] = "[1:0]";
					size = 2;
				}
				else if (words[1] == "Int1") {
					words[1] = "";
					size = 1;
				}
				else if (words[1] == "Int8") {
					words[1] = "[7:0]";
					size = 8;
				}
				else if (words[1] == "Int16") {
					words[1] = "[15:0]";
					size = 16;
				}
				else if (words[1] == "Int32") {
					words[1] = "[31:0]";
					size = 32;
				}
				else if (words[1] == "Int64") {
					words[1] = "[63:0]";
					size = 64;
				}

				//Set width length for unsigned------------------------
				if (words[1] == "UInt2") {
					words[1] = "[1:0]";
					size = 2;
					signedNum = false;
				}
				else if (words[1] == "UInt1") {
					words[1] = "";
					size = 1;
					signedNum = false;
				}
				else if (words[1] == "UInt8") {
					words[1] = "[7:0]";
					size = 8;
					signedNum = false;
				}
				else if (words[1] == "UInt16") {
					words[1] = "[15:0]";
					size = 16;
					signedNum = false;
				}
				else if (words[1] == "UInt32") {
					words[1] = "[31:0]";
					size = 32;
					signedNum = false;
				}
				else if (words[1] == "UInt64") {
					words[1] = "[63:0]";
					size = 64;
					signedNum = false;
				}
				//for loop to finish reading an definite (16) amount of inputs/outputs listed in a lines.
				bool stop = false;

				for (int j = 0; !stop && (j <= 17); j++){

					if (words[j] == words[0] || words[j] == words[1]) { //ignore the first two words we've seen
						continue;
					}
					else if (words[j] == "") {
						stop = true;
					}
					else {
						//remove the comma if there is one in "input a, b,".
						if (words[j].at(words[j].length() - 1) == ',') {
							words[j] = words[j].erase(words[j].size() - 1);
						}

						//Create class object and insert to DP*****
						iov = new Inoutput(words[j], words[1], words[0], size);
						iov->setSign(signedNum);
						myGraph.insertPut(iov);
					}
				}
			}	
			//TODO: Detect components & create corresponding Scheduling Graph---------------------------------------
			// ADD, SUB, MUL, REG, SHR, SHL, COMP
			else if (words[1] == "=") {

				int Vnumber = myGraph.getCompSize() + 1;

				// ADDER/SUBTRACTOR *************************************************************
				if (words[3] == "+" || words[3] == "-") {
					Component* adder;
					Inoutput* a;
					Inoutput* b;
					Inoutput* c;

					stringstream CName;

					CName << "V" << Vnumber; //guarantees unique name
					adder = new Component(Vnumber, "adder/subtractor", CName.str());

					a = myGraph.getInput(words[2]);
					b = myGraph.getInput(words[4]);
					c = myGraph.getOutput(words[0]);

					if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
						cout << "input/output/variable of an adder/subtractor componet does not exist" << endl;
						return false;
					}

					adder->insertInput(a);
					adder->insertInput(b);
					adder->setOutput(c);
					adder->setLatency(1); //One cycle latency specified
					adder->setToFullSchedule(1);
					
					adder->setOperation(lineString); 


					if (InIFflag == true) {
						adder->setIfCond(currIFcondition);
						myGraph.insertIF(adder); //it has an extra \t in there
					}
					else {
						myGraph.insertComponent(adder);
					}

				}
				// MULTIPLIER ********************************************************************
				if (words[3] == "*") {
					Component* mult;
					Inoutput* a;
					Inoutput* b;
					Inoutput* c;

					stringstream CName;

					CName << "V" << Vnumber; //guarantees unique name
					mult = new Component(Vnumber, "multiplier", CName.str());

					a = myGraph.getInput(words[2]);
					b = myGraph.getInput(words[4]);
					c = myGraph.getOutput(words[0]);

					if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
						cout << "input/output/variable of a multiplier componet does not exist" << endl;
						return false;
					}

					mult->insertInput(a);
					mult->insertInput(b);
					mult->setOutput(c);
					mult->setLatency(2); //One cycle latency specified
					mult->setToFullSchedule(2);
					mult->setOperation(lineString);

					if (InIFflag == true) {
						mult->setIfCond(currIFcondition);
						myGraph.insertIF(mult); //it has an extra \t in there
					}
					else {
						myGraph.insertComponent(mult);
					}

				}
				// DIVIDER/MODULO ****************************************************************
				if (words[3] == "/" || words[3] == "%") {
					Component* div;
					Inoutput* a;
					//Inoutput* b;
					Inoutput* c;

					stringstream CName;

					CName << "V" << Vnumber; //guarantees unique name
					div = new Component(Vnumber, "divider/modulo", CName.str());

					a = myGraph.getInput(words[2]);
					//b = myGraph.searchforEdge(words[4]);
					c = myGraph.getOutput(words[0]);

					if (a == NULL || c == NULL) { //if the input/outputs/wire not found
						cout << "input/output/variable of a multiplier componet does not exist" << endl;
						return false;
					}

					div->insertInput(a);
					//mult->insertInput(b);
					div->setOutput(c);
					div->setLatency(3); //One cycle latency specified
					div->setToFullSchedule(3);
					div->setOperation(lineString);
					if (InIFflag == true) {
						div->setIfCond(currIFcondition);
						myGraph.insertIF(div); //it has an extra \t in there
					}
					else {
						myGraph.insertComponent(div);
					}

				}
				// LOGICAL ***********************************************************************
				if (words[3] == ">" || words[3] == "<" || words[3] == "<<" || words[3] == ">>"
					|| words[3] == "==" || words[3] == "?") {
					Component* logic;
					Inoutput* a;
					Inoutput* b;
					Inoutput* c;
					Inoutput* d;

					stringstream CName;

					CName << "V" << Vnumber; //guarantees unique name
					logic = new Component(Vnumber, "logic", CName.str());
					if (words[3] == "?") {
						a = myGraph.getInput(words[2]);
						b = myGraph.getInput(words[4]);
						c = myGraph.getInput(words[6]);
						d = myGraph.getOutput(words[0]);

						if (a == NULL || b == NULL || c == NULL || d == NULL) { //if the input/outputs/wire not found
							cout << "input/output/variable of a multiplier componet does not exist" << endl;
							return false;
						}

						logic->insertInput(a);
						logic->insertInput(b);
						logic->insertInput(c);
						logic->setOutput(d);
						logic->setLatency(1); //One cycle latency specified
						logic->setOperation(lineString);

						if (InIFflag == true) {
							myGraph.insertIF(logic); //it has an extra \t in there
						}
						else {
							myGraph.insertComponent(logic);
						}
					}
					else {
						a = myGraph.getInput(words[2]);
						b = myGraph.getInput(words[4]);
						c = myGraph.getOutput(words[0]);

						if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
							cout << "input/output/variable of a multiplier componet does not exist" << endl;
							return false;
						}

						logic->insertInput(a);
						logic->insertInput(b);
						logic->setOutput(c);
						logic->setLatency(1); //One cycle latency specified
						logic->setToFullSchedule(1);
						logic->setOperation(lineString);

						if (InIFflag == true) {
							logic->setIfCond(currIFcondition);
							myGraph.insertIF(logic); //it has an extra \t in there
						}
						else {
							myGraph.insertComponent(logic);
						}
					}

					

				}
			}

		}
	}
	else {
		cout << "Sorry, I was not able to open the file for you." << endl;
		flag = false;
	}

	return flag;
}

bool ConnectGraph(Graph& myGraph) {
	Component* NOPhead;
	NOPhead = new Component(0, "head", "NOP");

	Component* NOPtail;
	NOPtail = new Component(0, "tail", "NOP");
	

	//Set predecessor and successor
	for (int i = 0; i < myGraph.getCompSize(); i++) {
		Component* currComp1;
		currComp1 = myGraph.getComponent(i);
		for (int j = 1; j < myGraph.getCompSize(); j++) {
			Component* currComp2;
			currComp2 = myGraph.getComponent(j);
			for (int k=0; k < currComp2->getInputSize(); k++) {
				Inoutput* currInput;
				currInput = currComp2->getInput(k);
				if (currComp1->getOutput() == currInput) {
					currComp1->addSuccessor(currComp2);
					currComp2->addPredecessor(currComp1);
				}
			}
		}
	}
	//Connect input type Inoutput objects to NULL
	for (int i = 0; i < myGraph.getCompSize(); i++) {
		Component* currComp;
		currComp = myGraph.getComponent(i);

		if (currComp->getPredecessorSize() == 0) {
			NOPhead->addSuccessor(currComp);
			currComp->addPredecessor(NOPhead);
		}

		if (currComp->getSuccessorSize() == 0) {
			NOPtail->addPredecessor(currComp);
			currComp->addSuccessor(NOPtail);
		}


	}
	myGraph.insertComponent(NOPtail);
	myGraph.insertComponentAt(NOPhead, 0);
	
	return true;
}

bool ALAPSchedule(Graph& myGraph, int alatency) {
	vector<Component*> queue;
	Component* tail = NULL;
	Component* head = NULL;
	Component* currComp = NULL;
	unsigned int i = 0;
	tail = myGraph.getComponent(myGraph.getCompSize() - 1);
	head = myGraph.getComponent(0);
	tail->setScheduled(alatency + 1);
	
	for (i = myGraph.getCompSize() - 1; i > 0; i--){
		currComp = myGraph.getComponent(i); 
		if (CheckSuccScheduled(currComp) == true) {
			if (currComp->getSuccessorSize() != 0) {
				currComp->setScheduled(lowestLatency(currComp) - currComp->getLatency());
			}
		}
		else{
			queue.push_back(currComp);
		}
	}
	for (i = 0; i < queue.size(); i++){
		currComp = queue.at(i);
		if (CheckSuccScheduled(currComp) == true) {
			if (currComp->getSuccessorSize() != 0) {
				currComp->setScheduled(lowestLatency(currComp) - currComp->getLatency());
			}
		}
	}
	head->setScheduled(lowestLatency(head) - 1);
	if (head->getScheduled() < 0){
		return false;
	}
	else {
		return true;
	}
}

bool CheckSuccScheduled(Component* acomponent) {
	Component* currComp;
	int i = 0;
	for (i = 0; i < acomponent->getSuccessorSize(); i++) {
		currComp = acomponent->getSuccessor(i);
		if (currComp->getScheduled() == -1) {
			return false;
		}
	}

	return true;
}

int lowestLatency(Component* aComp){
	int late = aComp->getSuccessor(0)->getScheduled();
	unsigned int i = 0;
	for (int i = 0; i < aComp->getSuccessorSize(); i++){
		if (aComp->getSuccessor(i)->getScheduled() < late){
			late = aComp->getSuccessor(i)->getScheduled();
		}
	}

	return late;
}

void setIFlatency(Graph& myGraph) {
	//Set predecessor and successor
	for (int i = 0; i < myGraph.getIfSize(); i++) {
		Component* currComp1;
		currComp1 = myGraph.getIFComponent(i);
		for (int j = 1; j < myGraph.getCompSize(); j++) {
			Component* currComp2;
			currComp2 = myGraph.getComponent(j);
			
			if (currComp2->getOutput() == currComp1->getIfCond()) {

				currComp1->addPredecessor(currComp2);

				currComp2->setIfPred();
				currComp2->addSuccessor(currComp1);

				if (currComp2->getType() == "multiplier") {
					currComp1->setScheduled(currComp2->getScheduled() + 2);//adjust for latency
					currComp2->setUntilIf(2);
				}
				else if (currComp2->getType() == "divider/modulo") {
					currComp1->setScheduled(currComp2->getScheduled() + 3);
					currComp2->setUntilIf(3);
				}
				else {
					currComp1->setScheduled(currComp2->getScheduled() + 1);
					currComp2->setUntilIf(1);
				}
			} //set the current Inouptut IF to the components
		}
	}
}

bool WritetoFile(Graph& GP, char* FileName, int latency) {
	int j = 0;
	string operationparts[7];

	ofstream myfile(FileName);

	

	if (myfile.is_open()) {

		//Print the module declaration *****************************************
		myfile << "`timescale 1ns / 1ps" << endl << "module hlsm(";

		istringstream inputs;
		istringstream outputs;
		myfile << "Clk,Rst,Start,";
		for (int i = 0; i < GP.getInpSize(); i++) {
			myfile << GP.getInputat(i)->getName() << ",";
		}

		for (int i = 0; i < GP.getOutSize(); i++) {
			if (i + 1 == GP.getOutSize()) {
				myfile << GP.getOutputat(i)->getName() << ");";
			}
			else {
				myfile << GP.getOutputat(i)->getName() << ",";
			}
		}

		//Print inputs, outputs, and wires*********************************************

		//inputs------
		int currentsizePrint;
		int prevsizePrint = GP.getInputat(0)->getSizeInt();
		int futuresizePrint;

		// CLK and RST
		myfile << endl << "\tinput Clk, Rst, Start;";

		bool currSign;
		bool futureSign;
		bool prevSign = GP.getInputat(0)->getSigned();

		//--check if the first input is signed-------
		if (GP.getInputat(0)->getSigned() == true) {
			myfile << endl << "\tinput signed " << GP.getInputat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\tinput " << GP.getInputat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < GP.getInpSize(); i++) {

			currentsizePrint = GP.getInputat(i)->getSizeInt();
			currSign = GP.getInputat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << GP.getInputat(i)->getName();

				if ((i + 1) < GP.getInpSize()) {
					futuresizePrint = GP.getInputat(i + 1)->getSizeInt();
					futureSign = GP.getInputat(i + 1)->getSigned();

					if (futuresizePrint == currentsizePrint && futureSign == currSign) {
						myfile << ", ";
					}
					else {
						myfile << ";";
					}

				}
				else {
					myfile << ";";
				}

			}
			else {
				if (currSign == true) {
					myfile << endl << "\tinput signed" << GP.getInputat(i)->getSizeSpec() << " "
						<< GP.getInputat(i)->getName();
				}
				else {
					myfile << endl << "\tinput " << GP.getInputat(i)->getSizeSpec() << " "
						<< GP.getInputat(i)->getName();
				}

				if ((i + 1) != GP.getInpSize()) {
					myfile << ", ";
				}
				else {
					myfile << ";";
				}
			}
			prevsizePrint = currentsizePrint;
			prevSign = currSign;
		}


		//OUTPUTS------
		prevsizePrint = GP.getOutputat(0)->getSizeInt();
		prevSign = GP.getInputat(0)->getSigned();
		//--check if the first input is signed-------
		if (GP.getOutputat(0)->getSigned() == true) {
			myfile << endl << "\toutput signed reg " << GP.getOutputat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\toutput reg " << GP.getOutputat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < GP.getOutSize(); i++) {

			currentsizePrint = GP.getOutputat(i)->getSizeInt();
			currSign = GP.getOutputat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << GP.getOutputat(i)->getName();

				if ((i + 1) < GP.getOutSize()) {
					futuresizePrint = GP.getOutputat(i + 1)->getSizeInt();
					futureSign = GP.getOutputat(i + 1)->getSigned();

					if (futuresizePrint == currentsizePrint && futureSign == currSign) {
						myfile << ", ";
					}
					else {
						myfile << ";";
					}

				}
				else {
					myfile << ";";
				}

			}
			else {
				if (currSign == true) {
					myfile << endl << "\toutput signed reg " << GP.getOutputat(i)->getSizeSpec() << " "
						<< GP.getOutputat(i)->getName();
				}
				else {
					myfile << endl << "\toutput reg " << GP.getOutputat(i)->getSizeSpec() << " "
						<< GP.getOutputat(i)->getName();
				}

				if ((i + 1) != GP.getOutSize()) {
					myfile << ", ";
				}
				else {
					myfile << ";";
				}
			}
			prevsizePrint = currentsizePrint;
			prevSign = currSign;
		}

		//wire------
		prevsizePrint = GP.getVarat(0)->getSizeInt();
		prevSign = GP.getVarat(0)->getSigned();
		//--check if the first input is signed-------
		if (GP.getVarat(0)->getSigned() == true) {
			myfile << endl << "\treg signed " << GP.getVarat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\treg " << GP.getVarat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < GP.getVarSize(); i++) {

			currentsizePrint = GP.getVarat(i)->getSizeInt();
			currSign = GP.getVarat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << GP.getVarat(i)->getName();

				if ((i + 1) < GP.getVarSize()) {
					futuresizePrint = GP.getVarat(i + 1)->getSizeInt();
					futureSign = GP.getVarat(i + 1)->getSigned();

					if (futuresizePrint == currentsizePrint && futureSign == currSign) {
						myfile << ", ";
					}
					else {
						myfile << ";";
					}

				}
				else {
					myfile << ";";
				}

			}
			else {
				if (currSign == true) {
					myfile << endl << "\treg signed " << GP.getVarat(i)->getSizeSpec() << " "
						<< GP.getVarat(i)->getName();
				}
				else {
					myfile << endl << "\treg " << GP.getVarat(i)->getSizeSpec() << " "
						<< GP.getVarat(i)->getName();
				}

				if ((i + 1) != GP.getVarSize()) {
					myfile << ", ";
				}
				else {
					myfile << ";";
				}
			}
			prevsizePrint = currentsizePrint;
			prevSign = currSign;
		}


		myfile << endl << endl << "\treg[2:0] State;" << endl;


		//Print states-----------------------------------------------

		//---if RST
		myfile << "\talways @(posedge Clk) begin" << "\n\t\tif (Rst) begin" << endl;
		myfile << "\t\t\tState<=Wait" << endl;
		for ( int k = 0; k < GP.getOutSize(); k++) {
			myfile << "\t\t\t" << GP.getOutputat(k)->getName() << "<=0;" << endl;
		}
		for (int k = 0; k < GP.getVarSize(); k++) {
			myfile << "\t\t\t" << GP.getVarat(k)->getName() << "<=0;" << endl;
		}

		//---wait state
		myfile << "\t\tend" << endl << "\t\telse begin" << endl;
		myfile << "\t\t\t case (State) begin" << endl;
		myfile << "\t\t\t\tWait: begin" << endl;
		myfile << "\t\t\t\t\t" << "if (Start == 1)begin" << endl
			<< "\t\t\t\t\t\tState<=S1;" << endl;
		myfile << "\t\t\t\t\tend" << endl;
		myfile << "\t\t\t\t\t" << "else begin" << endl
			<< "\t\t\t\t\t\tState<=Wait;" << endl;
		myfile << "\t\t\t\t\tend" << endl;

	//---rest of states

		//FIGURE OUT WHAT THE LAST COMPONENT SCHEDULED IS-----------------------------------------------
		int lastSchedTime = 0;
		for (int j = 0; j < GP.getCompSize(); j++) {
			if ((GP.getComponent(j)->getScheduled() > lastSchedTime)) {
				if (GP.getComponent(j)->getName() != "NOP") {
					lastSchedTime = GP.getComponent(j)->getScheduled();
				}
			}

		}

		//----------------------------------------------------------------------------------------------
		
		for (int i = 1; i < latency + 1; i++) {
			bool pred = false;
			bool goon = false;
			string condname = "";
			int SIF = 0;
			int schedTime=0;
			int count = 0;

			for (int j = 0; j < GP.getCompSize(); j++) {

				//clear prev parsing operation
				for (int i = 0; i < 7; i++) {
					operationparts[i].clear();
				}
				Component* currComp;
				currComp = GP.getComponent(j);
				//if the component was scheduled
				if ((currComp->getScheduled()  == i) && (currComp->getName() != "NOP")) { //if the component was scheduled at this time
					if (count == 0) {
						myfile << "\t\t\t\tend" << endl;
						myfile << "\t\t\t\tS" << i << ": begin" << endl;
					}


				//print the operation "a<=b+c" (add <= instead of just =
					istringstream operationParse(currComp->getOperation());

					//parse line
					operationParse >> operationparts[0] >> operationparts[1] >> operationparts[2]
						>> operationparts[3] >> operationparts[4] >> operationparts[5] >> operationparts[6];

					myfile << "\t\t\t\t\t" << operationparts[0] <<"<"<< operationparts[1] << operationparts[2]
						<< operationparts[3] << operationparts[4] << operationparts[5] << operationparts[6] 
						<< ";" << endl;

					count = count + 1;
				}
				if (currComp->getIfPred() == true) {
					if (currComp->getUntilIf() == 0) {
						pred = true;
						condname = currComp->getOutput()->getName();
						SIF = currComp->getSuccessor(currComp->getSuccessorSize() - 1)->getScheduled();
						schedTime = i + 1;
						currComp->setIfPred();
					}
					else {
						currComp->setUntilIf(currComp->getUntilIf() - 1);
					}
				}

				if (i+1 < lastSchedTime && j == GP.getCompSize()-1 && j!=0) {
					if (pred==true) {

						myfile << "\t\t\t\t\tif(" << condname
							<< "==1)begin" << endl;
						myfile << "\t\t\t\t\t\tState<=SIF"
							<< SIF
							<< ";" << endl;
						myfile << "\t\t\t\t\tend" << endl << "\t\t\t\t\telse begin" << endl << "\t\t\t\t\t\tState<="
							<< "S" <<schedTime << ";" << endl
							<< "\t\t\t\t\tend" << endl;

						
						pred = false;

					}
					else {
						myfile << "\t\t\t\t\tState<=S" << i + 1 << ";" << endl;
					}

				}
			}
		}

		//--final state
		myfile << "\t\t\t\t\tState<=Final;" << endl;

		//---IF statement states---------------------------------------------------------------------
		for (int i = 1; i < latency + 1; i++) {
			int count = 0;
			bool inLast = false;
			for (int j = 0; j < GP.getIfSize(); j++) {

				//clear prev parsing operation
				for (int i = 0; i < 7; i++) {
					operationparts[i].clear();
				}
				Component* currComp;
				currComp = GP.getIFComponent(j);
				//if the component was scheduled
				if ((currComp->getScheduled() == i) && (currComp->getName() != "NOP")) { //if the component was scheduled at this time
					if (count == 0) {
						myfile << "\t\t\t\tend" << endl;
						myfile << "\t\t\t\tSIF" << i << ": begin" << endl;
					}


					//print the operation "a<=b+c" (add <= instead of just =
					istringstream operationParse(currComp->getOperation());

					//parse line
					operationParse >> operationparts[0] >> operationparts[1] >> operationparts[2]
						>> operationparts[3] >> operationparts[4] >> operationparts[5] >> operationparts[6];

					myfile << "\t\t\t\t\t" << operationparts[0] << "<" << operationparts[1] << operationparts[2]
						<< operationparts[3] << operationparts[4] << operationparts[5] << operationparts[6]
						<< ";" << endl;

					count = count + 1;

					if (i + 1 < lastSchedTime && j == GP.getIfSize() - 1 && !inLast) {
						myfile << "\t\t\t\t\tState<=S" << i + 1 << ";" << endl;
						inLast = true;
				}



				}
			}
		}



		//------------------------------------------------------------------------
	
		myfile << "\t\t\t\tend" << endl;
		myfile << "\t\t\t\tFinal: begin" << endl;
		myfile << "\t\t\t\t\t" << "Done<=1;" << endl;
		myfile << "\t\t\t\t\t" << "State<=Wait;" << endl;
		myfile << "\t\t\t\tend" << endl;
		myfile << "\t\t\tendcase" << endl;
		myfile << "\t\tend" << endl;
		myfile << "\tend" << endl;

		//EOF
		myfile << "endmodule\n";
		myfile.close();
		cout << "Output generation successful to: " << FileName << endl;
		return true;
	}
	return true;
}