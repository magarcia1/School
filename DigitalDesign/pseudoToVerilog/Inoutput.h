//------------------------------------------------------------------------------------------------
//
// Author: Rubi Ballesteros and Miguel Garcia
// Date Created: September 2015
//
// Description: Data type for inputs outputs and wires
//
//------------------------------------------------------------------------------------------------

#ifndef INOUTPUT_H
#define INOUTPUT_H

//------------------------------------------------------------------------------------------------
#include <string>

using namespace std;
//------------------------------------------------------------------------------------------------

class Inoutput {
private:
	string name;
	string sizeSpec;
	string type;
	int size;
	bool signedNum;
	

public:
	Inoutput();
	Inoutput(string nameString,string sizeSpecString, string type, int sizeInteger);
	~Inoutput();

	void setSign(bool sign);
	bool getSigned();
	string getName();
	string getSizeSpec();
	int getSizeInt();
	string getType();

};

//------------------------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------------------------