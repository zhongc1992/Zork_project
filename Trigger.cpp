#include "Trigger.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

Trigger::Trigger(xml_node<> * node)
{
	triggered = false;
	for(xml_node<> *s = node->first_node(); s; s=s->next_sibling()){
		string s_name = s->name();
		
		if(s_name.compare("type") == 0){
			type= s->value();
			//cout<<"Trigger_type="<<type<<endl;
		}
		else if(s_name.compare("command") == 0){
			command = s->value();
			//cout<<"Trigger_command="<<command<<endl;
		}
		else if(s_name.compare("condition") == 0){
			//cdt(s);
			Condition condition;
			for (xml_node<> *p=s->first_node();p; p=p->next_sibling()){
				string c_name = p->name();
				string c_value = p->value();
				if(c_name.compare("has") == 0){
					condition.has = c_value;
				}
				else if(c_name.compare("object") == 0){
					condition.object = c_value;
				}
				else if(c_name.compare("owner") == 0){
					condition.owner = c_value;
				}
				else if(c_name.compare("status") == 0){
					condition.status = c_value;
				}

			}
			conditions.push_back(condition);
			//cout<<"Trigger_condition.objidect="<<condition.object<<endl;
		}
		else if(s_name.compare("print") == 0){
			//accept = s->value();
			print = s->value();
			//cout<<"Trigger_print="<<print<<endl;
		}
		else if(s_name.compare("action") == 0){
			actions.push_back(s->value());
		}

	}
}