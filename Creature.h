#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include "rapidxml.hpp"
#include "Trigger.h"


using namespace std;
using namespace rapidxml;
struct Attack{
	Condition condition;
	string print;
	vector <string> actions;
};
class Creature
{
public:
	Creature(xml_node<> * node);

	string name;
	string status;
	string description;
	vector <string> vulnerabilities;
	Attack attack;
	vector <Trigger> triggers;


};