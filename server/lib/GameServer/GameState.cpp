/*
 * GameState.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#include "GameState.hpp"

using namespace std;

GameState::GameState()
{
}
GameState::~GameState()
{
}
std::string GameState::generateDifStateMessage(std::map<std::string, GenObject*>* objects, std::vector<GenObject*>* recentlyCreatedObjects) {
	std::string str;

	//TODO: LOCK fixedOBjects + players WHILE DOING DIFF STATE
	str += this->generateObjectsDifStateMessage(objects, recentlyCreatedObjects);
	if (_GAMESTATE_DEBUG == 1)
		if(!str.empty())
			std::cout << "DIF STATE >> " << str << std::endl;

	return str;
}
std::string GameState::generateObjectsDifStateMessage(std::map<std::string, GenObject*>* objects, std::vector<GenObject*>* recentlyCreatedObjects)
{
	std::stringstream ss;
	unsigned int i=0;

	for (std::map<std::string, GenObject*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		GenObject *obj = (*it).second;
		if(obj->hasChanges)
		{
			ss << obj->generateObjectActionMessage((ObjectAction)Update) << SEPARATOR;
			obj->hasChanges = false;
		}
	}

	for(i=0; i < recentlyCreatedObjects->size(); i++)
	{
		GenObject *obj = (*recentlyCreatedObjects)[i];
		ss << obj->generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
		(*objects)[obj->guid] = obj;
	}
	recentlyCreatedObjects->clear();

	return ss.str();
}
