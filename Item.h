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
struct Turnon{
	string print;
	vector <string> actions;
};
class Item
{
public:
	Item(xml_node<> * node);
	//Container();

	string name;
	string status;
	string description;
	string writing;
	Turnon turnon;
	vector <Trigger> triggers;


};