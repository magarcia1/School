//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: September 2013
//
// Description: General code for a datapath.
//------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include "dpgen.h"
#include "Datapath.h"


Datapath::Datapath() {

}
Datapath::~Datapath() {
	unsigned int i = 0;

	//for (i = 0; i < this->inputs.size(); ++i){
	//	delete(inputs[i]);
	//}
	//for (i = 0; i < outputs.size(); ++i){
	//	delete(outputs[i]);
	//}
	for (i = 0; i < components.size(); ++i){
		delete(components[i]);
	}
	for (i = 0; i < wires.size(); ++i){
		delete(wires[i]);
	}
	for (i = 0; i < registers.size(); ++i){
		delete(registers[i]);
	}

	inputs.clear();
	outputs.clear();
	wires.clear();
	registers.clear();
	//components.clear();
}


void Datapath::insertPut(Inoutput* iow) {
	if (iow->getType() == "input") {
		inputs.push_back(iow);
	}
	else if (iow->getType() == "output") {
		outputs.push_back(iow);
	}
	else if (iow->getType() == "wire") {
		wires.push_back(iow);
	}
	else if (iow->getType() == "register") {
		registers.push_back(iow);
	}
}

void Datapath::insertComponent(Component* dpcomp) {
	this->components.push_back(dpcomp);
}

Inoutput* Datapath::getInput(string name) {
	Inoutput* current;
	for (int unsigned i = 0; i < inputs.size(); i++) {
		current = inputs.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	for (int unsigned i = 0; i < wires.size(); i++) {
		current = wires.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	for (int unsigned i = 0; i < registers.size(); i++) {
		current = registers.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	cout << "The input/wire " << name << " specified does not match the inputs declared.\n Program terminated.\n";
	return NULL;
}

Inoutput* Datapath::getOutputWire(string name) {
	Inoutput* current;
	for (int unsigned i = 0; i < wires.size(); i++) {
		current = wires.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	for (int unsigned i = 0; i < outputs.size(); i++) {
		current = outputs.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	for (int unsigned i = 0; i < registers.size(); i++) {
		current = registers.at(i);
		if (name == current->getName()) {
			return current;
		}
	}
	cout << "The wire/output " << name << " specified does not match the inputs declared.\n Program terminated.\n";
	return NULL;

}

int Datapath::getCompSize() {
	return components.size();
}

int Datapath::getInpSize() {
	return this->inputs.size();
}
int Datapath::getOutSize() {
	return this->outputs.size();
}
int Datapath::getWireSize() {
	return this->wires.size();
}

Component* Datapath::getComponent(int i) {
	return this->components.at(i);
}

Inoutput* Datapath::getInputat(int i) {
	return this->inputs.at(i);
}
Inoutput* Datapath::getOutputat(int i) {
	return this->outputs.at(i);
}
Inoutput* Datapath::getWireat(int i) {
	return this->wires.at(i);
}