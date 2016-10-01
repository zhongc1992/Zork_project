#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <algorithm>
#include "rapidxml.hpp"

#include "Container.h"
#include "Trigger.h"
#include "Room.h"
#include "Creature.h"
#include "Item.h"


#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"


using namespace std;
using namespace rapidxml;

bool Exit = false;

map <string, Room> all_rooms;
map <string, Item> all_items;
map <string, Container> all_containers;
map <string, Creature> all_creatures;
map <string, Trigger> all_triggers;

map <string, Room> aall_rooms;
map <string, Item> aall_items;
map <string, Container> aall_containers;
map <string, Creature> aall_creatures;
map <string, Trigger> aall_triggers;

void checkInput(string input);
void goToRoom(string input);
void updateItems();
void SplitInput(const std::string& str, std::vector<std::string>& v);
void dealItems(string input);
void removeItemFromContainer(string input); 
void removeItemFromInventory(string input);
void behindScenesCommands(string input);
void checkTriggersOverride(string input);//check each trigger with command and excute them if override
void checkTriggersOverride2();//check each trigger without command
void actTrigger(Trigger *this_trigger); //excute this_trigger
int scoreTrigger(Trigger *this_trigger);//give a trigger its override score by checking through all its conditions(don't check command)

Room *cur_room;
Room *start_room;
Container *cur_container;
vector<string> inventory; //curent inventory
vector<string> cur_items;
vector<string> cur_containers;
vector<string> cur_creatures;
vector<Trigger> cur_triggers;

Room* getRoomFromMap(string name){ //get a Room from all_rooms
  map <string, Room>::iterator p;
  p = all_rooms.find(name);
  if (p != all_rooms.end()) {
       return &(p->second);//return as a pointer to the room
  } else {
      //cout<<"system error! Couldn't find Roon from Map..."<<endl;
      return NULL;
  }
}

Room* ggetRoomFromMap(string name){ //a back up command for behind scene "Add"
  map <string, Room>::iterator p;
  p = aall_rooms.find(name);
  if (p != aall_rooms.end()) {
       return &(p->second);//return as a pointer to the room
  } else {
      //cout<<"system error! Couldn't find Roon from Map..."<<endl;
      return NULL;
  }
}

Container* getContainerFromMap(string input) {
  map <string,Container>::iterator p;
  p = all_containers.find(input);
  if (p != all_containers.end()) {
    return &(p->second);
  } else {
    //cout <<"system error! Couldn't find container from the Map..." <<endl;
    return NULL;
  }
}

Container* ggetContainerFromMap(string input) {
  map <string,Container>::iterator p;
  p = aall_containers.find(input);
  if (p != aall_containers.end()) {
    return &(p->second);
  } else {
    //cout <<"system error! Couldn't find container from the Map..." <<endl;
    return NULL;
  }
}

Item * getItemFromMap (string input) {
  map <string, Item>::iterator p;
  p = all_items.find(input);
  if (p != all_items.end()) {
    return &(p->second);
  } else {
    //cout <<"System error! Couldn't find item from the Map...."<<endl;
    return NULL;
  }
}

Item * ggetItemFromMap (string input) {
  map <string, Item>::iterator p;
  p = aall_items.find(input);
  if (p != aall_items.end()) {
    return &(p->second);
  } else {
    //cout <<"System error! Couldn't find item from the Map...."<<endl;
    return NULL;
  }
}

Creature *getCreatureFromMap (string input) {
  map <string, Creature>::iterator p;
  p = all_creatures.find(input);
  if (p != all_creatures.end()) {
    return &(p->second);
  } else {
    return NULL;
  }
}

Creature *ggetCreatureFromMap (string input) {
  map <string, Creature>::iterator p;
  p = aall_creatures.find(input);
  if (p != aall_creatures.end()) {
    return &(p->second);
  } else {
    return NULL;
  }
}

Trigger *getTriggerFromMap (Trigger *input) {
  //cout <<"getting triggers!!!!"<<endl;
  Trigger *p = NULL;
  for(int i=0; i < cur_items.size(); i++){
    Item * cur_item = getItemFromMap(cur_items[i]);
    //vector <Trigger>::iterator p;

    //cout <<"one one"<<endl;
    //cout <<"size of ....."<<cur_item->triggers.size()<<endl;
    //for (p = cur_item->triggers.begin();p != cur_item->triggers.end();p++) {
    for (int k = 0; k < cur_item->triggers.size();k++) {
        int score  = 0;
        //cout << "in item...."<<endl;
        Trigger *p = &(cur_item->triggers[k]);
        if (p->conditions.size() == input->conditions.size()) {
          for (int j = 0;j < p->conditions.size();j++) {
              if (p->conditions[j].has.empty() && input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].status == input->conditions[j].status) {
                    score +=1;
                  }
                  else {
                    score -= 1000;
                  }
              }
              else if (!p->conditions[j].has.empty() && !input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].has == input->conditions[j].has && p->conditions[j].owner == input->conditions[j].owner) {
                    score +=1;
                  }
                  else {
                    score -= 1000;
                  }
              }
              else {
                score -= 1000;
              }
          }

        }
        else {
          score -= 1000;//if conditions size not equal
        }

        if (score > 0) {
          return p;
        }
      }
    }
    //cout <<"size of cont....."<<cur_containers.size()<<endl;

    for(int i=0; i < cur_containers.size(); i++){
    Container * cur_cont = getContainerFromMap(cur_containers[i]);
    //vector <Trigger>::iterator p;
    //Trigger *p = NULL;
    //cout<<"COntainer name "<<cur_cont->name<<endl;
   // cout<<"two two"<<endl;
   // for (p = cur_cont->triggers.begin();p != cur_cont->triggers.end();p++) {
    for (int k = 0; k < cur_cont->triggers.size();k++) {
        int score  = 0;
        p = &(cur_cont->triggers[k]);
        //cout<<"wwww111"<<endl;
        if (p->conditions.size() == input->conditions.size()) {
          for (int j = 0;j < p->conditions.size();j++) {
            //cout<<"www222"<<endl;
              if (p->conditions[j].has.empty() && input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].status == input->conditions[j].status) {
                    //cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"11111"<<endl;
                    score +=1;
                  }
                  else {
                      //                 cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"wrong 1111"<<endl;
                    score -= 1000;
                  }
              }
              else if (!p->conditions[j].has.empty() && !input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].has == input->conditions[j].has && p->conditions[j].owner == input->conditions[j].owner) {
                    //cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"2222"<<endl;
                    score +=1;
                  }
                  else {
                      //                                     cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"wrong 2222"<<endl;
                    score -= 1000;
                  }
              }
              else {
                score -= 1000;
              }
          }

        }
        else {
          score -= 1000;//if conditions size not equal
        }

        if (score > 0) {
          return p;
        }
      }
    }

//cout <<"size of creat....."<<cur_creatures.size()<<endl;
   for(int i=0; i < cur_creatures.size(); i++){
    Creature * cur_creature = getCreatureFromMap(cur_creatures[i]);
    //vector <Trigger>::iterator p;
   // cout<<"three three" <<endl;
    //Trigger *p = NULL;
    //cout <<"cur_creature_trigger size" <<cur_creature->triggers.size()<<endl;
    //for (p = cur_creature->triggers.begin();p != cur_creature->triggers.end();p++) {
     for (int k = 0; k < cur_creature->triggers.size();k++) { 
        int score  = 0;
        p = &(cur_creature->triggers[k]);
        //cout<<"www1111"<<endl;
        if (p->conditions.size() == input->conditions.size()) {
          for (int j = 0;j < p->conditions.size();j++) {
            //cout<<"www222"<<endl;
              if (p->conditions[j].has.empty() && input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].status == input->conditions[j].status) {
                    //cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"11111"<<endl;
                    score +=1;
                  }
                  else {
                    //cout<<p->conditions[j].object<<"....."<<input->conditions[j].object<<endl;
                    //cout<<"wrong 1111"<<endl;
                    score -= 1000;
                  }
              }
              else if (!p->conditions[j].has.empty() && !input->conditions[j].has.empty()) {
                  if (p->conditions[j].object == input->conditions[j].object && p->conditions[j].has == input->conditions[j].has && p->conditions[j].owner == input->conditions[j].owner) {
                    //cout<<"22222"<<endl;
                    score +=1;
                  }
                  else {
                    //cout<<"wrong 2222"<<endl;
                    score -= 1000;
                  }
              }
              else {
                score -= 1000;
              }
          }

        }
        else {
          score -= 1000;//if conditions size not equal
        }
        //cout<<"the score equals to ..."<<score<<endl;
        if (score > 0) {
          //cout<<"returned result....."<<endl;
          return p;
   
        }
      }
    }

    return NULL;

  }


  //cout<<"  ww10"<<endl;
  /*for(int i=0; i < cur_containers.size(); i++){
    Container * cur_container = getContainerFromMap(cur_containers[i]);
    cur_triggers.insert(cur_triggers.end(), cur_container->triggers.begin(), cur_container->triggers.end());
  }
  //cout<<"  ww11"<<endl;
  //cout<<"cur_creature.size="<<cur_creatures.size()<<endl;
  if(cur_creatures.size()>0){
    for(int i=0; i < cur_creatures.size(); i++){
      if(all_creatures.find(cur_creatures[i]) != all_creatures.end()){
        Creature * cur_creature = getCreatureFromMap(cur_creatures[i]);
        cur_triggers.insert(cur_triggers.end(), cur_creature->triggers.begin(), cur_creature->triggers.end());
      }
    }
  }*/


void removeItemFromContainer(string input) {
  for(int i = 0; i < cur_containers.size(); i++) {
    Container* target = getContainerFromMap(cur_containers[i]);
    if (find(target->items.begin(),target->items.end(),input) != target->items.end()) {
      vector<string>::iterator it;
      for(it = target->items.begin();it != target->items.end();it++) {
        string name = *it;
        if (name == input) {
          target->items.erase(it);//erase by iterator
          break;
        }
      }
    }
  }
}

void removeItemFromInventory(string input) {
  vector<string>::iterator it;
  for(it = inventory.begin(); it != inventory.end(); it++) {
    string item = *it;
    if (item == input) {
      inventory.erase(it);
      break;
    }
  }
}

void SplitInput(const std::string& str, std::vector<std::string>& v)
{
  std::stringstream ss(str);
    ss >> std::noskipws;
    std::string field;
    char ws_delim;
    while(1) {
      if( ss >> field )
          v.push_back(field);
      else if (ss.eof())
          break;
      else
          v.push_back(std::string());

      ss.clear();
      ss >> ws_delim;
  }
}


void behindScenesCommands(string input) {


  if (input.find("Update") != -1 ) {//update a new status to the object, for example Update axe to sharp
      vector<string> words;
      SplitInput(input,words);

      if (words.size() == 4) {
          if (getItemFromMap(words[1]) != NULL) {
            Item *this_item = getItemFromMap(words[1]);
            this_item->status = words[3]; 
          } 
          else if (getContainerFromMap(words[1]) != NULL) {
            Container * this_container = getContainerFromMap(words[1]);
            this_container->status = words[3];
          }
          else if (getCreatureFromMap(words[1]) != NULL) {
            Creature * this_creature = getCreatureFromMap(words[1]);
            this_creature->status = words[3];
          }
          else if (getRoomFromMap(words[1]) != NULL) {
            Room *this_room = getRoomFromMap(words[1]);
            this_room->status = words[3];
          }
      } 

      else {
         cout<<"System error information on item action!!"<<endl;
      }

  } //end of update object

  else if (input.find("Add") != -1) {//Add object to room/ container
      vector<string> words;
      SplitInput(input,words);

      int ob_type;
      int tar_type;

      if (ggetItemFromMap(words[1]) != NULL) { //find everything from the backed up item list
          ob_type = 1;
          //Item* this_item = ggetItemFromMap(words[1]);
      } 
      if (ggetContainerFromMap(words[1]) != NULL) {
          ob_type = 2;
          //Container* this_cont = ggetContainerFromMap(words[1]); 
      }
      if (ggetCreatureFromMap(words[1]) != NULL) {
          ob_type = 3;
          //Creature* this_creature = ggetCreatureFromMap(words[1]);
      }
      if (ggetContainerFromMap(words[3]) != NULL) {
          tar_type = 1;
          //Container* this_cont = ggetContainerFromMap(words[3]); 
      }      
      if (ggetRoomFromMap(words[3]) != NULL) {
          tar_type = 2;
          //Room* this_room = ggetRoomFromMap(words[3]); 
      }

      if (ob_type == 1 && tar_type == 1) {//be careful with these steps!!!!!!!!!!!!!!!!!!!!!!!!
        Item* this_item = ggetItemFromMap(words[1]);//get things need from back up!!!!!!!!!!!!!!!!!!1
        Container* this_cont = getContainerFromMap(words[3]); //add it into current room or container!!!!!!!!!!!!!!!!!!!
        this_cont->items.push_back(words[1]);//add item to the list in container
        all_items.insert(make_pair(words[1],*this_item));//add item back to map
      }

      if (ob_type == 1 && tar_type == 2) {
        Item* this_item = ggetItemFromMap(words[1]);
        Room* this_room = getRoomFromMap(words[3]); 
        this_room->items.push_back(words[1]);//add item to the list in room
        all_items.insert(make_pair(words[1],*this_item));//add item back to map
      }

      if (ob_type == 2 && tar_type == 2) {
        Container* this_cont = ggetContainerFromMap(words[1]);
        Room* this_room = getRoomFromMap(words[3]); 
        this_room->containers.push_back(words[1]);//add container to the list in room
        all_containers.insert(make_pair(words[1],*this_cont));//add item back to map
      }

      if (ob_type == 3 && tar_type == 2) {
        Creature* this_creature = ggetCreatureFromMap(words[1]);
        Room* this_room = getRoomFromMap(words[3]); 
        this_room->creatures.push_back(words[1]);//add creature to the list in room
        //cout<<this_room->name<<endl;
        /*for(int i=0;i<this_room->creatures.size();i++) {
          cout<<"Creatures are"<<this_room->creatures[i]<<endl;
        }*/
        all_creatures.insert(make_pair(words[1],*this_creature));//add item back to map
        //cout<<"Add creature to ROOM!!!!!"<<endl;
      }

  }//end of add

  else if (input.find("Delete") != -1) {//Delete
      vector<string> words;
      SplitInput(input,words);

      if (getItemFromMap(words[1]) != NULL) {
        map <string, Item>::iterator target;
        target = all_items.find(words[1]);
        all_items.erase(target);//erase item from map

        map <string, Container>::iterator this_cont;//if the item also belongs to a container, remove it from the container
        for (this_cont = all_containers.begin();this_cont !=all_containers.end();this_cont++) {
          Container *my_cont = &(this_cont->second);
          if (find(my_cont->items.begin(),my_cont->items.end(),words[1]) != my_cont->items.end()) {//search for item in this contaienr
              vector<string>::iterator it;
              for(it = my_cont->items.begin();it != my_cont->items.end();it++) {
                  string name = *it;
                  if (name == words[1]) {
                    my_cont->items.erase(it);//remove the container's iterm
                    break;
                  }
              }
          }
          
        }


        map <string, Room>::iterator this_room;//if item also belongs to a room, remove it from the room item list;
        for (this_room = all_rooms.begin();this_room != all_rooms.end();this_room++) {
          Room *my_room = &(this_room->second);
          if (find (my_room->items.begin(),my_room->items.end(),words[1]) != my_room->items.end()) {
            vector<string>::iterator it;
            for (it = my_room->items.begin();it != my_room->items.end();it++) {
              string name = *it;
              if (name == words[1]) {
                my_room->items.erase(it);
                break;
              }
            }
          }
        }

      }

      if (getContainerFromMap(words[1]) != NULL) {
        map <string, Container>::iterator target;
        target = all_containers.find(words[1]);
        all_containers.erase(target);//erase the container from map

        map <string, Room>::iterator this_room;//if container also belongs to a room, remove it from the room container list;
        for (this_room = all_rooms.begin();this_room != all_rooms.end();this_room++) {
          Room *my_room = &(this_room->second);
          if (find (my_room->containers.begin(),my_room->containers.end(),words[1]) != my_room->containers.end()) {
            vector<string>::iterator it;
            for (it = my_room->containers.begin();it != my_room->containers.end();it++) {
              string name = *it;
              if (name == words[1]) {
                my_room->containers.erase(it);
                break;
              }
            }
          }
        }
      }
 
      if (getRoomFromMap(words[1]) != NULL) {
        map <string, Room>::iterator target;
        target = all_rooms.find(words[1]);
        all_rooms.erase(target);//erase room from all room list, doesn't deal with border cases
      }

      if (getCreatureFromMap(words[1]) != NULL) {
        map <string, Creature>::iterator target;
        target = all_creatures.find(words[1]);
        all_creatures.erase(target);//erase creatrue from map

        map <string, Room>::iterator this_room;//if container also belongs to a room, remove it from the room container list;
        for (this_room = all_rooms.begin();this_room != all_rooms.end();this_room++) {
          Room *my_room = &(this_room->second);
          if (find (my_room->creatures.begin(),my_room->creatures.end(),words[1]) != my_room->creatures.end()) {
            vector<string>::iterator it;
            for (it = my_room->creatures.begin();it != my_room->creatures.end();it++) {
              string name = *it;
              if (name == words[1]) {
                my_room->creatures.erase(it);
                //cout<<"Creature erased from room!!!!"<<endl;
                break;
              }
            }
          }
        }


      }

  }//end of delete

  else if(input == "Game Over"){
    cout<<"Victory!"<<endl;
    Exit = true;
  }
  else{
    //cout<<"   check1"<<endl;
    checkInput(input);
  }
}



int scoreTrigger(Trigger *this_trigger){//give a trigger its override score by checking through all its conditions(don't check command)
  int override = 0;

  for (int j = 0; j < this_trigger->conditions.size();j++) {//check each condition individually
                  if (this_trigger->conditions[j].has.empty()) {//if this condition doesn't contain a "has" element
                      string object_name = this_trigger->conditions[j].object;
                      string object_stat = this_trigger->conditions[j].status;

                      if (getRoomFromMap(object_name) != NULL) {//room is the object
                          Room * this_room = getRoomFromMap(object_name);
                          if (this_room->status == object_stat) {
                            override += 1; //one condition passed, increase by a small number
                            //cout <<"Room trigger 1st override"<<endl;
                          }
                          else {
                            override -= 1000;//the condition doesn't match, deduct by a large number
                            //cout <<"Room trigger NOT 1st override"<<endl;
                          }
                      }

                      else if (getContainerFromMap(object_name) != NULL) {//container is the object
                          Container * this_cont = getContainerFromMap(object_name);
                          if (this_cont->status == object_stat) {
                            override += 1; 
                            //cout<<"Container trigger first override" <<endl;
                          }
                          else {
                            override -= 1000;
                            //cout<<"Container trigger NOT first override" <<endl;
                          }
                      }

                      else if (getItemFromMap(object_name) != NULL) {//item is the object
                          Item * this_item = getItemFromMap(object_name);
                          if (this_item->status == object_stat) {
                            override += 1;
                            //cout<<"Item trigger first override" <<endl;
                          }
                          else {
                            override -= 1000;
                            //cout<<"Item trigger NOT first override" <<endl;
                          }
                      }

                      else if (getCreatureFromMap(object_name) != NULL) {//creature is the object
                          Creature * this_creat = getCreatureFromMap(object_name);
                          if (this_creat->status == object_stat) {
                            override += 1;
                            //cout<<"Creature trigger first override" <<endl;
                          }
                          else {
                            override -= 1000;
                            //cout<<"Creature trigger NOT first override" <<endl;
                          }
                      }
                  } 

                  else {// if the condition does have a "has" element
                    string object_has = this_trigger->conditions[j].has;
                    string object_owner = this_trigger->conditions[j].owner;
                    string object_name = this_trigger->conditions[j].object;

                    if (object_owner == "inventory") {
                      //cout<<"owner is inventory"<<endl;
                      if (find(inventory.begin(),inventory.end(),object_name) != inventory.end() && object_has == "yes") {
                        override += 1;
                        //cout<<"owner is inventory 1st overide"<<endl;
                      } 
                      else if (find(inventory.begin(),inventory.end(),object_name) == inventory.end() && object_has == "no") {
                        override += 1;
                        //cout<<"owner is inventory 1st overide"<<endl;
                      }
                      else {
                        override -= 1000;
                        //cout<<"owner is inventory NOT 1st overide"<<endl;
                      }
                    }

                    else if (getContainerFromMap(object_owner) != NULL) {
                      //cout<<"owner is container"<<endl;
                      Container *this_cont = getContainerFromMap(object_owner);
                      if (find(this_cont->items.begin(),this_cont->items.end(),object_name) != this_cont->items.end() && object_has == "yes") {
                        override += 1;
                        //cout<<"owner is container 1st override"<<endl;
                      }
                      else if (find(this_cont->items.begin(),this_cont->items.end(),object_name) == this_cont->items.end() && object_has == "no") {
                        override += 1;
                        //cout<<"owner is container 1st override"<<endl;
                      }
                      else {
                        override -= 1000;
                        //cout<<"owner is container NOT 1st override"<<endl;
                      }
                    }

                    else if (getRoomFromMap(object_owner) != NULL) {
                      //cout<<"owner is room"<<endl;
                      Room *this_room = getRoomFromMap(object_owner);
                      if (find(this_room->items.begin(),this_room->items.end(),object_name) != this_room->items.end() && object_has == "yes") {
                        override += 1;
                        //cout<<"owner is room 1st override"<<endl;

                      }
                      else if (find(this_room->items.begin(),this_room->items.end(),object_name) == this_room->items.end() && object_has == "no") {
                        override += 1;
                        //cout<<"owner is room 1st override"<<endl;

                      }
                      else {
                        override -= 1000;
                        //cout<<"owner is room NOT 1st override"<<endl;

                      }
                    }

                  }
               }
return override;
}//end of scoreTrigger


void actTrigger(Trigger *this_trigger) {//excute a trigger
  /*if (this_trigger.triggered == true) {//if the trigger has been activated already as a single
      return;
  }*/
  
      if (this_trigger->type == "single") {
       // cout << "trigger type changed......."<<endl;
        this_trigger->triggered = true;//only single trigger's status will update to true, permanent trigger won't

      }
      cout<<this_trigger->print<<endl;
      for (int i = 0;i < this_trigger->actions.size();i++) {
        behindScenesCommands(this_trigger->actions[i]);
      }
  

}




void checkTriggersOverride2() {
 
  for (int i = 0; i < cur_triggers.size(); i++) {//current trigger is a copy, it won't be afftected the real trigger if you make any changes
      int override = 0;
      Trigger *this_trigger = &cur_triggers[i];//make it pointer to change its status
      //cout<<"before this sign..............."<<endl;
      //cout<<"NAME TRIGGER "<<this_trigger->conditions[i].object<<endl;
      Trigger *real_trigger = getTriggerFromMap(this_trigger);
     // cout<<"After this sign............."<<endl;
      if (real_trigger == NULL) {
        real_trigger = this_trigger;
      }
      if (this_trigger->command.empty()) {//it contains not command
        override = scoreTrigger(this_trigger);
      }
      if (override > 0 && real_trigger->triggered == false) {//this trigger passed all the conditions checking, ready to be excuted
        //cout<<"Before activated, it is .."<<real_trigger->triggered<<endl;
        //cout<<"check 2 activated"<<endl;
        actTrigger(real_trigger);//active the target trigger
        //cout<<"check 2 activated"<<endl;
              //checkInput(input);
        //checkTriggersOverride2();
      } 
  }
}




void checkTriggersOverride(string input) {//start of the first trigger check,check those triggers with a command. Thus we can judge whether a command overide occurs or not.

    
    for (int i = 0; i < cur_triggers.size(); i++) {
            int override = 0; //the flag of override
            Trigger *this_trigger = &cur_triggers[i];
            Trigger *real_trigger = getTriggerFromMap(this_trigger);
            if (real_trigger == NULL) {
          //      cout<<"DONT HAVE THE TRIGGER>>>>>"<<endl;
                real_trigger = this_trigger;
            }
            if (this_trigger->command == input) { //if trigger has a command and the command matches the input, we continue to check the other conditions
               override = scoreTrigger(this_trigger);
            }//end of command check
        //    cout <<"the actual trigger is .."<<real_trigger->triggered<<endl;
            if (override > 0 && real_trigger->triggered == false) {//this trigger passed all the conditions checking, ready to be excuted
              //cout<<"check 1 checkInput"<<endl;
      //        cout<<"check 1 activated..."<<endl;
              actTrigger(real_trigger);//active the target trigger
              //checkInput(input);
              //cout<<"check 1 activated..."<<endl;
              checkTriggersOverride2();
              return;//once trigger is activated, end the function
            } 
            /*else {//if not trigger actived by the command, excute the input, and then check corresponding trigger actived 
              cout<<"check 1 checkInput"<<endl;
              checkInput(input);
              checkTriggersOverride2();
            }*/
    }//end of checking each trigger
    //cout<<"check 1 checkInput"<<endl;//if no trigger activated, check the input
    checkInput(input);
   // cout<<"go in to check trigger 2"<<endl;
    updateItems();
    checkTriggersOverride2();
    //return override;

}//end of the first trigger check














void dealItems(string input) {
    vector<string> words;
    SplitInput(input,words);
    if (words.size() == 1) {
      if (words[0] == "i") {//check the inventory
        if(inventory.empty()) {
          cout <<"Inventory: empty "<<endl;
        } else {
          cout<< "Inventory: ";
          for(int i=0; i < inventory.size(); i++){
              cout<<inventory[i]<<" ";
            }
          cout<<""<<endl;
        }
      } else {
        cout <<"Invalid command deal with inventory....." << endl;
      }
    }

    else if (words.size() == 2) {
      if (words[0] == "take") { //take item from room
        if (find(inventory.begin(),inventory.end(),words[1]) != inventory.end()) { //if item is already in the inventory
          cout << "Item is already in the invetory!" <<endl;
        } else if (find(cur_items.begin(),cur_items.end(),words[1]) != cur_items.end()) {
          inventory.push_back(words[1]);
          cout <<"Item "<<words[1]<<" added to inventory" <<endl;
          removeItemFromContainer(words[1]); //even if its taken from the room, we can still call this func and do nothing. Because item in Room won't be displayed, no need to remove.
        } else {
          cout<<"Item doesn't exist in this room"<<endl;
        }
      }

      else if (words[0] == "drop") { //drop item to room
        if(find(inventory.begin(),inventory.end(),words[1]) != inventory.end()) {// if item is in the inventory
          removeItemFromInventory(words[1]);
          cout<< "Item "<<words[1]<<" dropped"<<endl;
          cur_room->items.push_back(words[1]);
        } else {
          cout <<"Item "<<words[1]<<" is not in your inventory, cannot drop!"<<endl;
        }

      }

      else if (words[0] == "open") {//open container or exit
        if (words[1] == "exit") {// if command is "open exit"
          if (cur_room->type == "exit") {// the current room has the exit
            cout << "Game over" <<endl;
            Exit = true; //change flag
          } else {
            cout <<"This room is not the exit!!!"<<endl;
          }
        } else{
          if(find(cur_containers.begin(),cur_containers.end(),words[1]) != cur_containers.end()) {
             Container *this_container = getContainerFromMap(words[1]);
             this_container->open = true; //set the container as open, cur_item will updated with items in it
             if (this_container->items.size() == 0) {
                  cout <<this_container->name<<" is empty!"<<endl;
             } else{
                 cout <<this_container->name<<" contains ";
                 for (int i = 0; i < this_container->items.size(); i++) {
                    cout <<this_container->items[i]<<" ";
                 }
                 cout<<" "<<endl;
             }

          } else {
            cout <<"This container doesn't exit in current room!"<<endl;
          }
      }
      
    }

    else if (words[0] == "read") {//read item description
      if (find(inventory.begin(),inventory.end(),words[1]) != inventory.end()) {
          Item *this_item = getItemFromMap(words[1]);
          if (this_item->writing.empty()) {
            cout <<"Nothing written!"<<endl;
          } else {
            cout <<this_item->writing<<endl;  
          }
      } else {
        cout <<"Cannot read! Item is not in your inventory!!"<<endl;
      }

    }

    else {
      cout <<"invalid command with size 2!!!"<<endl;
    }



  }//end of words.size() == 2

  else if (words.size() == 3) {
       if (words[0] == "turn" && words[1] == "on") {//the case of turn on
            if (find(inventory.begin(),inventory.end(),words[2]) != inventory.end()) {
                Item *this_item = getItemFromMap(words[2]);
                if (!this_item->turnon.print.empty()) {
                    cout<<this_item->turnon.print<<endl;
                    if (this_item->turnon.actions.size() > 0) {
                       for (int i = 0; i < this_item->turnon.actions.size(); i++) {
                          behindScenesCommands(this_item->turnon.actions[i]);
                       }
                    }
                }
            } else {
                cout<<"item is not in your inventory!"<<endl;
            }
       } //end of the case turn on

       else {
        cout<<"Invalid command with word size 3!!"<<endl;
       }
  }//end of words.size() == 3




  else if (words.size() == 4) {

    if (words[0] == "put") {//the case to put an item into container
        if (find(inventory.begin(),inventory.end(),words[1]) != inventory.end()) {
          if(find(cur_containers.begin(),cur_containers.end(),words[3]) != cur_containers.end()) {
              Container * this_cont = getContainerFromMap(words[3]);
              if (this_cont->open == true) {
                  removeItemFromInventory(words[1]);
                  this_cont->items.push_back(words[1]);
                  cout<<"Item "<<words[1]<<" added to container "<<words[3]<<endl;
              } else {
                cout <<"You need to open the chest first!!!!"<<endl;
              }
                            
          } else {
              cout <<"Container is not in your current room" <<endl;
          }

        } else {
            cout <<"The item " <<words[1]<<" is not in your inventory" << endl;
        }

    }// end of the case "Put"

    else if (words[0] == "attack") {

      if (words[2] == "with") {
        if (getCreatureFromMap(words[1]) == 0) {
          cout <<"Creature doesn't exit!" <<endl;
        }
        else if (find(cur_creatures.begin(),cur_creatures.end(),words[1]) == cur_creatures.end()) {
          cout<<"Creature is not in current room!"<<endl;
        }
        else if (getItemFromMap(words[3]) == 0) {
          cout<<"Item doesn't exist!"<<endl;
        }
        else if (find(inventory.begin(),inventory.end(),words[3]) == inventory.end()) {
          cout<<"Item is not in your inventory!"<<endl;
        }
        else {//both creature and item valid, continue
          Creature *this_creature = getCreatureFromMap(words[1]);
          Item *this_item = getItemFromMap(words[3]);
          string object_stat = this_creature->attack.condition.status;

          if (find(this_creature->vulnerabilities.begin(),this_creature->vulnerabilities.end(),words[3]) != this_creature->vulnerabilities.end()) {//able to attack
            if (this_creature->attack.condition.object.empty()) {//the creature doesn't have condtion information,see sample.txt.xml
              cout<< "Assault the "<<words[1]<<" with "<<words[3]<<endl;
              cout<<this_creature->attack.print<<endl;
              for (int i = 0;i < this_creature->attack.actions.size();i++) {
                behindScenesCommands(this_creature->attack.actions[i]);
              }
            }

            else {//the creature has attack conditions
              string ob_name = this_creature->attack.condition.object;//see sample.txt.xml
              string ob_stat = this_creature->attack.condition.status;
              if (getItemFromMap(ob_name) != NULL) {
                Item* this_item = getItemFromMap(ob_name);
                if (this_item->status == ob_stat) {
                    cout<< "Assault the "<<words[1]<<" with "<<words[3]<<endl;
                    cout<<this_creature->attack.print<<endl;
                    for (int i = 0;i < this_creature->attack.actions.size();i++) {
                      behindScenesCommands(this_creature->attack.actions[i]);
                    }
                }
                else {
                  cout<<words[3]<<" is not activiated to attack"<<endl;
                }
              }


            }




          }


          else {
            cout<<"can't attack creature with "<<words[3]<<endl;
          }


        }


      } 
      else {
        cout <<"Incorrect command of attack!!!"<<endl;
      }

    }//end of the case "attack"

    else {
      cout <<"Invalid command with word size 4!!!"<<endl;
    }
  }//end of words.size() == 4

  else {
    cout<<"invalide command!!!!"<<endl;
  } 
}

void updateItems() {//update elements according to the cur_room
  //cout<<"update items..........."<<endl;
  cur_items.clear();
  cur_items.insert(cur_items.begin(),cur_room->items.begin(),cur_room->items.end());
  //cout<<"Items in the current rooms are: "<<endl<<cur_items<<endl;
  /*for(int i=0; i<cur_items.size(); ++i) {
    cout <<"the current items are: "<< cur_items[i] << endl;
  }*/
  cur_containers.clear();

  cur_containers.insert(cur_containers.end(), cur_room->containers.begin(), cur_room->containers.end());

  for(int i=0; i < cur_containers.size(); i++){

    Container* cur_container = getContainerFromMap(cur_containers[i]);
    if(cur_container->open == true){//containers has been opened, put them into cur_items ready to pick up
      cur_items.insert(cur_items.end(), cur_container->items.begin(), cur_container->items.end());
    }
  } 

  cur_creatures.clear();
  cur_creatures.insert(cur_creatures.end(),cur_room->creatures.begin(),cur_room->creatures.end());
  /*for(int i=0; i<cur_creatures.size(); ++i) {
    cout <<"the creatures are: "<< cur_creatures[i] << endl;
  }*/

  cur_triggers.clear();
  cur_triggers.insert(cur_triggers.end(),cur_room->triggers.begin(),cur_room->triggers.end());

  for(int i=0; i < cur_items.size(); i++){
    Item * cur_item = getItemFromMap(cur_items[i]);
    cur_triggers.insert(cur_triggers.end(), cur_item->triggers.begin(), cur_item->triggers.end());
  }
  //cout<<"  ww10"<<endl;
  for(int i=0; i < cur_containers.size(); i++){
    Container * cur_container = getContainerFromMap(cur_containers[i]);
    cur_triggers.insert(cur_triggers.end(), cur_container->triggers.begin(), cur_container->triggers.end());
  }
  //cout<<"  ww11"<<endl;
  //cout<<"cur_creature.size="<<cur_creatures.size()<<endl;
  if(cur_creatures.size()>0){
    for(int i=0; i < cur_creatures.size(); i++){
      if(all_creatures.find(cur_creatures[i]) != all_creatures.end()){
        Creature * cur_creature = getCreatureFromMap(cur_creatures[i]);
        cur_triggers.insert(cur_triggers.end(), cur_creature->triggers.begin(), cur_creature->triggers.end());
      }
    }
  }

}


void goToRoom(string input) {
  vector <Border> cur_borders = cur_room->borders;
  string cur_border;
  bool validinput = false;
  if (input == "n") {
    input = "north";
  } else if (input == "s") {
    input = "south";
  } else if (input == "w") {
    input = "west";
  } else if (input == "e") {
    input = "east";
  } 

  for (int i = 0; i < cur_borders.size(); i++) {
        cur_border = cur_borders[i].direction;
        if (cur_border == input) {
	  validinput = true;
	  cur_room = getRoomFromMap(cur_borders[i].name); 
	  //cout<<cur_room->description<<endl;//print the description of the new room
	}

  }
  if (validinput == false) {
    cout <<"You cannot go that direction!" <<endl;
  }
  cout<<cur_room->description<<endl;//print the current room description
  cout<<"Your current room is: "<<cur_room->name<<endl;//print the name of cur_room

}



void checkInput(string input) {//check the input of the entering parameter
  if (( input == "n") || (input == "s") || (input == "w") || (input == "e")) {
    goToRoom(input);
    //cout<<"enter another room...."<<endl;
  } else {
    dealItems(input);
    //cout <<"Invalid command"<<endl;
  }
}

int main (int argc, char ** argv) {
  
  cout<<argv[1]<<endl;
  xml_document<> doc;
  ifstream file(argv[1]);//read the map.xml file
  stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  string content(buffer.str());
  //cout <<buffer.str()<<endl;
  // cout <<content<<endl;
  doc.parse<0>(&content[0]);
  xml_node<> *pRoot = doc.first_node();//pRoot represents the map

  string big_name;

  for (xml_node<> *pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling()){
    //pNode represents rooms,creatures and items
      big_name = pNode->name();

      if (big_name.compare("room") == 0) {
        	Room room(pNode);
        	all_rooms.insert(make_pair(room.name,room));
        	aall_rooms.insert(make_pair(room.name,room));	
              } else if (big_name == "item") {
        	Item item(pNode);
        	all_items.insert(make_pair(item.name,item));
        	aall_items.insert(make_pair(item.name,item));
              } else if (big_name == "container") {
                Container container(pNode);
                all_containers.insert(make_pair(container.name,container));
                aall_containers.insert(make_pair(container.name,container));
              } else if (big_name == "creature") {
                Creature creature(pNode);
                all_creatures.insert(make_pair(creature.name,creature));
                aall_creatures.insert(make_pair(creature.name,creature));
              }

      }

      start_room = getRoomFromMap("Entrance");//get the first room to start the game
      cur_room = start_room;
      cout<<start_room->description<<endl;//print the description of the first room
      while(Exit == false) {
        	char input[100];
    	    cout<<">";
    	    cin.getline(input,100);
    	    updateItems();//update items based on the current room

          //vector<string> words;
          //SplitInput(input,words);
          //cout << "The out put splited words are:...." <<words[0] <<" "<<words[1] << endl;
          checkTriggersOverride(input);
          //cout <<"The override equals to....... "<<override<<endl;

    	    //checkInput(input);

      }

  return 0;
}
