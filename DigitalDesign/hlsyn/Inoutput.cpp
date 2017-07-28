
//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: September 2015
//
// Description:  General input/output/wire
//------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include "Inoutput.h"


//------------------------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------------------------

Inoutput::Inoutput() {
	this->size = 0;
}
Inoutput::Inoutput(string nameString, string sizeSpecString, string type, int sizeInteger) {
	this->name = nameString;
	this->sizeSpec = sizeSpecString;
	this->size = sizeInteger;
	this->type = type;
	this->signedNum = true;

}
Inoutput::~Inoutput() {
	//done in datapath.h/.c
}

void Inoutput::setSign(bool sign) {
	this->signedNum = sign;
}

bool Inoutput::getSigned() {
	return this->signedNum;
}

string Inoutput::getName(){
	return this->name;

}
string Inoutput::getSizeSpec() {
	return this->sizeSpec;

}
int Inoutput::getSizeInt() {
	return this->size;
}
string Inoutput::getType() {
	return this->type;
}