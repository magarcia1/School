//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: September 2015
//
// Description: Data type for inputs outputs and wires
//
//------------------------------------------------------------------------------------------------

#ifndef COMPONENT_H
#define COMPONENT_H

//------------------------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "Inoutput.h"

using namespace std;
//------------------------------------------------------------------------------------------------

class Component {
private:
	string type;
	string name;
	vector<Inoutput*> inputs;
	Inoutput* output;
	int sizeoOutput;


public:
	Component();
	Component(string atype, string aname);
	~Component();

	void setType(string type);
	void setName(string name);
	void insertInput(Inoutput* anInput);
	void setOutput(Inoutput* anOutput);
	void setSize(int theSize);

	string getType();
	string getName();
	Inoutput* getInput(int i);
	Inoutput* getOutput();
	int getSize();
	int getInputSize();

	void replaceInput(int i, Inoutput* newPut);

};
//------------------------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------------------------
