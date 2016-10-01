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

class Container
{
public:
	Container(xml_node<> * node);
	//Container();

	string name;
	string status;
	string description;
	vector <string> accepts;
	vector <string> items;
	vector <Trigger> triggers;

	bool open;


};