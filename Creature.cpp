#include "Creature.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

Creature::Creature(xml_node<> * node)
{
	for(xml_node<> *s = node->first_node(); s; s=s->next_sibling()){
		string s_name = s->name();
		string s_value = s->value();
		
		if(s_name.compare("name") == 0){
			name = s_value;
			//cout<<"Room_name="<<name<<endl;
		}
		else if(s_name.compare("status") == 0){
			status = s_value;
			//cout<<"Room_status="<<status<<endl;
		}
		else if(s_name.compare("description") == 0){
			description = s_value;
			//cout<<"Room_description="<<status<<endl;
		}
		else if(s_name.compare("vulnerability") == 0){
			//accept = s->value();
			vulnerabilities.push_back(s_value);
			//cout<<"Room_items="<<vulnerabilities[0]<<endl;
		}
		else if(s_name.compare("attack") == 0){
			//accept = s->value();
			//Attack attack;
			for (xml_node<> *p=s->first_node();p; p=p->next_sibling()){
				string a_name = p->name();
				string a_value = p->value();
				if(a_name.compare("condition") == 0){
					//attack.condition;
					for (xml_node<> *c=p->first_node();c; c=c->next_sibling()){
						string c_name = c->name();
						string c_value = c->value();
						if(c_name.compare("has") == 0){
							attack.condition.has = c_value;
						}
						else if(c_name.compare("object") == 0){
							attack.condition.object = c_value;
						}
						else if(c_name.compare("owner") == 0){
							attack.condition.owner = c_value;
						}
						else if(c_name.compare("status") == 0){
							attack.condition.status = c_value;
						}

					}
					//cout<<"Creature_.attack.condition.has="<<attack.condition.has<<endl;
					//cout<<"Creature_.attack.condition.object="<<attack.condition.object<<endl;
					//cout<<"Creature_.attack.condition.owner="<<attack.condition.owner<<endl;
					//cout<<"Creature_.attack.condition.status="<<attack.condition.status<<endl;
				}
				else if(a_name.compare("print") == 0){
					attack.print = a_value;
				}
				else if(a_name.compare("action") == 0){
					attack.actions.push_back(a_value);
				}
				

			}
			
		}
		
		
		else if(s_name.compare("trigger") == 0){
			//accept = s->value();
			Trigger trigger(s);
			triggers.push_back(trigger);
			//cout<<"Creature_trigger.object="<<triggers[0].conditions[0].object<<endl;
			//cout<<"Creature_trigger.owner="<<triggers[0].conditions[0].owner<<endl;
		}
	}

}