/**************************************************************************************************/

/*
* File: graph.cpp
* Author: Miguel A. Garcia & Rubi Ballesteros
* NetID: magarcia1
* Assignment #: 3
* Date: November/10/2015
*
* Description: Graph.h contains the definition of a class named Graph that consists mainly of
* two sections: Components (nodes) and Inputs/outputs/variables (edges).

*
*/

/**************************************************************************************************/

#include "Graph.h"
#include <iostream>

using namespace std;

/**************************************************************************************************/

Graph::Graph() {

}

void Graph::insertPut(Inoutput* iow) {
	if (iow->getType() == "input") {
		inputs.push_back(iow);
	}
	else if (iow->getType() == "output") {
		outputs.push_back(iow);
	}
	else if (iow->getType() == "variable") {
		variables.push_back(iow);
	}
}

void Graph::insertComponent(Component* dpcomp) {
	this->components.push_back(dpcomp);
}

void Graph::insertIF(Component* aComponent) {
	this->IfStatements.push_back(aComponent);
}

Inoutput* Graph::getInput(string name) {
	Inoutput* current;
	for (int unsigned i = 0; i < inputs.size(); i++) {
		current = inputs.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	for (int unsigned i = 0; i < variables.size(); i++) {
		current = variables.at(i);
		if (name == current->getName()) {
			return current;
		}
	}

	cout << "The input/wire " << name << " specified does not match the inputs declared.\n Program terminated.\n";
	return NULL;
}

Inoutput* Graph::getOutput(string name) {
	Inoutput* current;
	for (int unsigned i = 0; i < variables.size(); i++) {
		current = variables.at(i);
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

	cout << "The wire/output " << name << " specified does not match the inputs declared.\n Program terminated.\n";
	return NULL;

}

int Graph::getCompSize() {
	return components.size();
}

int Graph::getInpSize() {
	return this->inputs.size();
}

int Graph::getOutSize() {
	return this->outputs.size();
}

int Graph::getVarSize() {
	return this->variables.size();
}

int Graph::getIfSize() {
	return this->IfStatements.size();
}

Component* Graph::getComponent(int i) {
	return this->components.at(i);
}

Inoutput* Graph::getInputat(int i) {
	return this->inputs.at(i);
}

Inoutput* Graph::getOutputat(int i) {
	return this->outputs.at(i);
}

Inoutput* Graph::getVarat(int i) {
	return this->variables.at(i);
}

Component* Graph::getIFComponent(int i){
	return this->IfStatements.at(i);
}

/**************************************************************************************************/