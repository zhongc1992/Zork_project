#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "rapidxml.hpp"


using namespace std;
using namespace rapidxml;
struct Condition{
	string has;
	string object;
	string owner;
	string status;
};
class Trigger
{

//private: 
public:
	Trigger(xml_node<> * node);
	//Container();
	string type;
	string command;
	string print;
	vector <Condition> conditions;
	vector <string> actions;
	

	bool triggered;
};

#endif /*TRIGGER_H_*/
	

