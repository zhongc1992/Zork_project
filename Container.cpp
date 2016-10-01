#include "Container.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

Container::Container(xml_node<> * node)
{
	open = false;
	for(xml_node<> *s = node->first_node(); s; s=s->next_sibling()){
		string s_name = s->name();
		
		if(s_name.compare("name") == 0){
			name = s->value();
			//cout<<"Container_name="<<name<<endl;
		}
		else if(s_name.compare("status") == 0){
			status = s->value();
			//cout<<"Container_status="<<status<<endl;
		}
		else if(s_name.compare("description") == 0){
			description = s->value();
			//cout<<"Container_description"<<status<<endl;
		}
		else if(s_name.compare("accept") == 0){
			//accept = s->value();
			accepts.push_back(s->value());
			//cout<<"Container_accept="<<accepts[0]<<endl;
		}
		else if(s_name.compare("item") == 0){
			//accept = s->value();
			items.push_back(s->value());
			//cout<<"Container_items="<<items[0]<<endl;
		}
		else if(s_name.compare("trigger") == 0){
			//accept = s->value();
			Trigger trigger(s);
			triggers.push_back(trigger);
			//cout<<"Container_trigger.condition.object="<<triggers[0].conditions[0].object<<endl;
			//cout<<"Container_trigger.condition.owner="<<triggers[0].conditions[0].owner<<endl;
			}
	}

}


