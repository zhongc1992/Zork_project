#include "Room.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

Room::Room(xml_node<> * node) {
  for (xml_node<> *s = node->first_node();s;s = s->next_sibling()) {
        string s_name = s->name();
	string s_value = s->value();
    
	if (s_name == "name") {
	  name = s_value;
	  // cout<<"First  as......" <<name<<endl;
	} else if (s_name == "status") {
	  status = s_value;
	  // cout<<"Room name is "<<status<<endl;
	} else if (s_name == "type") {
	  type = s_value;
	  //cout <<"Type recorded as"<<type<<endl;
	} else if (s_name == "description") {
	  description = s_value;
	  //cout<<"Description as...."<<description<<endl;
	} else if (s_name == "border") {
	    Border border;
	    for (xml_node<> *p = s->first_node();p;p = p->next_sibling()) {
	              string b_name = p->name();
		      string b_value = p->value();
		      if (b_name == "direction") {
			border.direction = b_value;
			//cout <<"Border direcion as....." <<border.direction<<endl;
		      } else if (b_name == "name") {
			border.name = b_value;
			//cout <<"Border name as....." <<border.name<<endl;
		      }	      
	    }
	    borders.push_back(border);
	} else if (s_name == "container") {
	    containers.push_back(s_value);
	    // cout<<"container as...."<<s_value<<endl;
	} else if (s_name == "item") {
	    items.push_back(s_value);
	    //cout<<"items as ....."<<s_value<<endl;
	} else if (s_name == "creature") {
	    creatures.push_back(s_value);
	    //cout<<"Creatures as ....."<<s_value<<endl;
	} else if (s_name == "trigger") {
	    Trigger trigger(s);
	    triggers.push_back(trigger);
	    }

  }
}
