//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: September 2015
//
// Description: .
//
//------------------------------------------------------------------------------------------------

#ifndef DATAPATH_H
#define DATAPATH_H

//------------------------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "Inoutput.h"
#include "Component.h"

using namespace std;
//------------------------------------------------------------------------------------------------

class Datapath{
private:
	vector<Inoutput*> inputs;
	vector<Inoutput*> outputs;
	vector<Inoutput*> wires;
	vector<Inoutput*> registers;
	
	vector<Component*> components;



public:
	Datapath();
	~Datapath();

	void insertPut(Inoutput* iow);
	void insertComponent(Component* dpcomp);
	Inoutput* getInput(string name);
	Inoutput* getInputat(int i);
	Inoutput* getOutputat(int i);
	Inoutput* getWireat(int i);
	Inoutput* getOutputWire(string name);
	Component* getComponent(int i);
	int getCompSize();
	int getInpSize();
	int getOutSize();
	int getWireSize();
	
	
	

};

//------------------------------------------------------------------------------------------------

#endif 
