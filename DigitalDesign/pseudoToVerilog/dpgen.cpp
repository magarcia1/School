//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: 13 November 2013
//
// Description: General code for a binary search tree that does not handle the case of duplicate key insertions.
//				This tree is sorted based on the order of the ASCII representaion of words "varName"
//------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include "dpgen.h"
#include "Inoutput.h"

//TODO: Datapath Class & header to hold vector of all components. DONE
//TODO: Component Class & header to hold--type,name,inputs,outputs,size of output, size string. DONE
//TODO: Input/Output/wire Class & header containing name string and size of input/output (includes wires). DONE

bool ReadfromFile(Datapath &DP, char* FileName) {

	string command;
	string  lineString;
	ifstream input;
	int temp = 0;
	int size; // in bits

	input.open(FileName);

	if (!input.is_open()) {
		cout << "Could not open file " << FileName << "\n";
		return false;
	}

	while (!input.eof()) {
		//Note: all words are separated by spaces.
		string words[18];

		getline(input, lineString);
		istringstream inStringS(lineString);

		//parse line
		inStringS >> words[0] >> words[1] >> words[2] >> words[3] >> words[4] >> words[5] >> words[6]
			>> words[7] >> words[8] >> words[9] >> words[10] >> words[11] >> words[12] >> words[13]
			>> words [14] >> words[15] >> words[16] >> words [17];


		//TODO: Ignore Empty Lines DONE----------------------------------------------------------------------
		if (words[0] == "") {
			continue;
		}
		//TODO: Ignore // (comments) DONE---------------------------------------------------------------------
		else if (words[0] == "//") {
			continue;
		}

		//TODO: Detect Input Output Wire Register Statements DONE-----------------------------------------------------
		else if ((words[0] == "input") || (words[0] == "output") || (words[0] == "wire") || (words[0] == "register")) {
			Inoutput* iow;
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
			//while loop to finish reading an definite (16) amount of inputs/outputs listed in a lines.
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
					if (words[j].at(words[j].length()-1) == ',') {
						words[j] = words[j].erase(words[j].size() - 1);
					}
					/*if (words[j].back() == ','){
						words[j].pop_back();
					} */

					//Create class object and insert to DP*****
					iow = new Inoutput(words[j], words[1], words[0], size);
					iow->setSign(signedNum);
					DP.insertPut(iow);
				}


			}

 		}

		//TODO: Detect components & create corresponding DP components---------------------------------------
		// DONE: ADD, SUB, MUL, REG, SHR, SHL, COMP
		else if (words[1] == "=") {

			int nameNumber = DP.getCompSize() + 1;

			// ADDER ****************************************************************************
			if (words[3] == "+") {
				Component* adder;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;

				stringstream CName;

				CName << "add" << nameNumber; //guarantees unique name

				adder = new Component("ADD", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
				
				adder->insertInput(a);
				adder->insertInput(b);
				adder->setOutput(c);
				adder->setSize(c->getSizeInt());
				DP.insertComponent(adder);
				
			}

			// SUBTRACTER ***********************************************************************
			else if (words[3] == "-") {
				Component* sub;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				stringstream CName;
				CName << "sub" << nameNumber; //guarantees unique name


				sub = new Component("SUB", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
				
				sub->insertInput(a);
				sub->insertInput(b);
				sub->setOutput(c);
				sub->setSize(c->getSizeInt());
				DP.insertComponent(sub);
				


			}

			// MULTIPLIER ***********************************************************************
			else if (words[3] == "*") {
				Component* mult;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				stringstream CName;

				CName << "mul" << nameNumber; //guarantees unique name

				mult = new Component("MUL", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
				if (a->getSigned() || b->getSigned() || c->getSigned()) { //if the input/outputs/wire signed
					mult->setType("SMUL");
				}

				mult->insertInput(a);
				mult->insertInput(b);
				mult->setOutput(c);
				mult->setSize(c->getSizeInt());
				DP.insertComponent(mult);
				

			}

			// COMPARATOR ***********************************************************************
			else if (words[3] == "<" || words[3] == ">" || words[3] == "==") {
				Component* comp;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				stringstream CName;

				CName << "comp" << nameNumber; //guarantees unique name

				comp = new Component("COMP", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
			
				if (a->getSigned() || b->getSigned() || c->getSigned()) { //if the input/outputs/wire signed
					comp->setType("SCOMP");
				}

				comp->insertInput(a);
				comp->insertInput(b);
				comp->setOutput(c);

				if (a->Inoutput::getSizeInt() >= b->Inoutput::getSizeInt() ){ 
					temp = a->Inoutput::getSizeInt();
				}
				else if (a->Inoutput::getSizeInt() < b->Inoutput::getSizeInt() ){ 
					temp = b->Inoutput::getSizeInt();
				}

				comp->setSize(temp);
				DP.insertComponent(comp);
			}

			// 2X1MULTIPLEXER *******************************************************************
			//g = dLTe ? d : e                                                                  *
			//MUX2x1 #(64) mux1(e, d, dLTe, g);                                                 *
			//***********************************************************************************
			else if (words[3] == "?" && words[5] == ":") {
				Component* mux;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				Inoutput* d;
				stringstream CName;

				CName << "mux" << nameNumber; //guarantees unique name

				mux = new Component("MUX2X1", CName.str());

				a = DP.getInput(words[6]);
				b = DP.getInput(words[4]);
				c = DP.getInput(words[2]);
				d = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL || d == NULL) { //if the input/outputs/wire not found
					return false;
				}

				if (a->getSigned() || b->getSigned() || c->getSigned() || d->getSigned()) { //if the input/outputs/wire signed
					mux->setType("SMUX2X1");
				}

				mux->insertInput(a);
				mux->insertInput(b);
				mux->insertInput(c);
				mux->setOutput(d);
				mux->setSize(d->getSizeInt());
				DP.insertComponent(mux);
				


			}

			// REGISTER***************************************************************************
			else if (words[3] == "") {
				Component* reg = NULL;
				Inoutput* a = NULL;
				Inoutput* b = NULL;
				Inoutput* clk = new Inoutput("clk", "", "input", 1);
				Inoutput* rst = new Inoutput("rst", "", "input", 1);

				stringstream CName;

				CName << "reg" << nameNumber; //guarantees unique name

				reg = new Component("REG", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL) { //if the input/outputs/wire not found
					return false;
				}
				

				reg->insertInput(clk);
				reg->insertInput(rst);
				reg->insertInput(a);
				reg->setOutput(b);
				reg->setSize(b->getSizeInt());
				DP.insertComponent(reg);
				
			}

			// RIGHT SHIFTER *********************************************************************
			//zrin = greg >> dEQe                                                                *
			//SHR #(64) shr1(greg, dEQe, zrin);                                                  *
			//************************************************************************************
			else if (words[3] == ">>") {
				Component* shr;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				stringstream CName;

				CName << "shr" << nameNumber; //guarantees unique name

				shr = new Component("SHR", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
				
				if (a->getSigned() || b->getSigned() || c->getSigned()) { //if the input/outputs/wire signed
					shr->setType("SSHR");
				}

				shr->insertInput(a);
				shr->insertInput(b);
				shr->setOutput(c);
				shr->setSize(c->getSizeInt());
				DP.insertComponent(shr);
				


			}

			// LEFT SHIFTER **********************************************************************
			////xrin = hreg << dLTe                                                              *
			//SHL #(64) shl1(hreg,dLTe,xrin);                                                    *
			//************************************************************************************
			else if (words[3] == "<<") {
				Component* shl;
				Inoutput* a;
				Inoutput* b;
				Inoutput* c;
				stringstream CName;

				CName << "shl" << nameNumber; //guarantees unique name

				shl = new Component("SHL", CName.str());

				a = DP.getInput(words[2]);
				b = DP.getInput(words[4]);
				c = DP.getOutputWire(words[0]);

				if (a == NULL || b == NULL || c == NULL) { //if the input/outputs/wire not found
					return false;
				}
				
				if (a->getSigned() || b->getSigned() || c->getSigned()) { //if the input/outputs/wire signed
					shl->setType("SSHL");
				}
				shl->insertInput(a);
				shl->insertInput(b);
				shl->setOutput(c);
				shl->setSize(c->getSizeInt());
				DP.insertComponent(shl);
			}

			//ERROR*******************************************************************************
			/*else if ((words[3] != "+") || (words[3] != "-") || (words[3] != "*") || (words[3] != "<<") ||
				(words[3] != ">>") || (words[3] != "?") || (words[3] != "") ){
				cout << "Incorrect operator" << words[3] << " used.";
				return false;
			}*/
			else {
				cout << "Error on Computation.\n";
				return false;
			}
}
		

		

	}
	input.close();
	return true;

}

bool AdjustInputs(Datapath &DP) {
	Inoutput* newPut;
	Inoutput* a = NULL;
	Inoutput* b = NULL;

	Component* currComp;

	int outputSize = 0;
	int inputSize = 0;
	unsigned int i = 0;

	if (DP.getCompSize() == 0) {
		cout << "You entered an empty file." << endl;
		return false;
	}

	for (i=0; i < DP.getCompSize(); i++) {
		
		currComp = DP.getComponent(i);
		for (int j=0; j < currComp->getInputSize(); j++) {
			a = currComp->getInput(j);
			b = currComp->getOutput();
			inputSize = a->getSizeInt();
			outputSize = b->getSizeInt();
			
			//Check that it is not the select line of the MUX too.
			//Check that it is not a Comparator output is always smaller.
			if (inputSize < outputSize && (currComp->getType().at(currComp->getType().length()-1) !='1' && j!=2)
				&& (currComp->getType()!="COMP" && currComp->getType() != "SCOMP")
				&& (currComp->getType() != "ADD" && currComp->getType() != "SUB")
				&& (currComp->getType() != "REG" && (j!=0 || j!=1))
				&& ((currComp->getType() != "SHL" || currComp->getType() != "SSHL") || j != 1 )
				&& ((currComp->getType() != "SHR" || currComp->getType() != "SSHR") || j != 1)) {
				if (a->getSigned()) {

					if (currComp->getType() == "SHL" || currComp->getType() == "SSHL" ||
						currComp->getType() == "SHR" || currComp->getType() == "SSHR") {

						if (j == 1) {
							//nothing here
						}
					
						else {
							stringstream newName;
							newName << "{{" << (outputSize - inputSize) << "{" << a->getName() << "[" << a->getSizeInt() - 1 << "]}}"
								<< ", " << a->getName() << "}";


							newPut = new Inoutput(newName.str(), a->getSizeSpec(), a->getType(), a->getSizeInt());
							currComp->replaceInput(j, newPut);
						}
					}
					else {
						stringstream newName;
						newName << "{{" << (outputSize - inputSize) << "{" << a->getName() << "[" << a->getSizeInt() - 1 << "]}}"
							<< ", " << a->getName() << "}";


						newPut = new Inoutput(newName.str(), a->getSizeSpec(), a->getType(), a->getSizeInt());
						currComp->replaceInput(j, newPut);
					}
				}
				else {
					stringstream newName;
					newName << "{{" << (outputSize - inputSize) << "{0}}" << ", " << a->getName() << "}";

					newPut = new Inoutput(newName.str(), a->getSizeSpec(), a->getType(), a->getSizeInt());
					currComp->replaceInput(j, newPut);
				}
				
			}
			else if (inputSize > outputSize
				&& (currComp->getType() != "COMP" && currComp->getType() != "SCOMP")
				&& (currComp->getType() != "ADD" && currComp->getType() != "SUB")) {
			
				stringstream newName;
				newName << "[" << (inputSize - outputSize) << ":0]" << a->getName();


				newPut = new Inoutput(newName.str(), a->getSizeSpec(), a->getType(), a->getSizeInt());
				currComp->replaceInput(j, newPut);

			}
		}
	
	}


	return true;
}

bool WritetoFile(Datapath &DP, char* FileName) {
	int j = 0;
	ofstream myfile(FileName);

	if (myfile.is_open()) {

		//Print the module declaration *****************************************
		myfile << "`timescale 1ns / 1ps" << endl << "module circuit(";

		istringstream inputs;
		istringstream outputs;

		for (int i = 0; i < DP.getInpSize(); i++) {
			myfile << DP.getInputat(i)->getName() << ",";
		}

		for (int i = 0; i < DP.getOutSize(); i++) {
			if (i + 1 == DP.getOutSize()) {
				myfile << DP.getOutputat(i)->getName() << ");";
			}
			else {
				myfile << DP.getOutputat(i)->getName() << ",";
			}
		}

		//Print inputs, outputs, and wires*********************************************

		//inputs------
		int currentsizePrint;
		int prevsizePrint = DP.getInputat(0)->getSizeInt();
		int futuresizePrint;

		bool currSign;
		bool futureSign;
		bool prevSign = DP.getInputat(0)->getSigned();
		//--check if the first input is signed-------
		if (DP.getInputat(0)->getSigned() == true) {
			myfile << endl << "\tinput signed " << DP.getInputat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\tinput " << DP.getInputat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < DP.getInpSize(); i++) {

			currentsizePrint = DP.getInputat(i)->getSizeInt();
			currSign = DP.getInputat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << DP.getInputat(i)->getName();

				if ((i + 1) < DP.getInpSize()) {
					futuresizePrint = DP.getInputat(i + 1)->getSizeInt();
					futureSign = DP.getInputat(i + 1)->getSigned();

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
					myfile << endl << "\tinput signed" << DP.getInputat(i)->getSizeSpec() << " "
						<< DP.getInputat(i)->getName();
				}
				else {
					myfile << endl << "\tinput " << DP.getInputat(i)->getSizeSpec() << " "
						<< DP.getInputat(i)->getName();
				}

				if ((i + 1) != DP.getInpSize()) {
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
		prevsizePrint = DP.getOutputat(0)->getSizeInt();
		prevSign = DP.getInputat(0)->getSigned();
		//--check if the first input is signed-------
		if (DP.getOutputat(0)->getSigned() == true) {
			myfile << endl << "\toutput signed " << DP.getOutputat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\toutput " << DP.getOutputat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < DP.getOutSize(); i++) {

			currentsizePrint = DP.getOutputat(i)->getSizeInt();
			currSign = DP.getOutputat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << DP.getOutputat(i)->getName();

				if ((i + 1) < DP.getOutSize()) {
					futuresizePrint = DP.getOutputat(i + 1)->getSizeInt();
					futureSign = DP.getOutputat(i + 1)->getSigned();

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
					myfile << endl << "\toutput signed " << DP.getOutputat(i)->getSizeSpec() << " "
						<< DP.getOutputat(i)->getName();
				}
				else {
					myfile << endl << "\toutput " << DP.getOutputat(i)->getSizeSpec() << " "
						<< DP.getOutputat(i)->getName();
				}

				if ((i + 1) != DP.getOutSize()) {
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
		prevsizePrint = DP.getWireat(0)->getSizeInt();
		prevSign = DP.getWireat(0)->getSigned();
		//--check if the first input is signed-------
		if (DP.getWireat(0)->getSigned() == true) {
			myfile << endl << "\twire signed " << DP.getWireat(0)->getSizeSpec() << " ";
		}
		else {
			myfile << endl << "\twire " << DP.getWireat(0)->getSizeSpec() << " ";
		}
		//--------------------------------------------
		for (int i = 0; i < DP.getWireSize(); i++) {

			currentsizePrint = DP.getWireat(i)->getSizeInt();
			currSign = DP.getWireat(i)->getSigned();

			if (currentsizePrint == prevsizePrint && currSign == prevSign) {
				myfile << DP.getWireat(i)->getName();

				if ((i + 1) < DP.getWireSize()) {
					futuresizePrint = DP.getWireat(i + 1)->getSizeInt();
					futureSign = DP.getWireat(i + 1)->getSigned();

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
					myfile << endl << "\twire signed " << DP.getWireat(i)->getSizeSpec() << " "
						<< DP.getWireat(i)->getName();
				}
				else {
					myfile << endl << "\twire " << DP.getWireat(i)->getSizeSpec() << " "
						<< DP.getWireat(i)->getName();
				}

				if ((i + 1) != DP.getWireSize()) {
					myfile << ", ";
				}
				else {
					myfile << ";";
				}
			}
			prevsizePrint = currentsizePrint;
			prevSign = currSign;
		}

		myfile << "\n\n";
		for (int i = 0; i < DP.getCompSize(); i++){
			myfile << "	" << DP.getComponent(i)->getType() << " #(" << DP.getComponent(i)->getSize() << ") " << DP.getComponent(i)->getName() << "(";
			for (j = 0; j < DP.getComponent(i)->getInputSize(); j++){
				myfile << DP.getComponent(i)->getInput(j)->getName() << ", ";
			}
			for (j = 0; j < 1; j++){
				myfile << DP.getComponent(i)->getOutput()->getName() << ");";
			}
			myfile << "\n";
		}
		myfile << "endmodule\n";
		myfile.close();
		cout << "Output generation successful to: "<< FileName << endl;
		return true;
	}
	//(&DP)->~Datapath();
	return true;
		
}