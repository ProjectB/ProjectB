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
std::string GameState::generateDifStateMessage(std::map<std::string, GenObject*>* objects, std::vector<GenObject*>* recentlyCreatedObjects, std::vector<GenObject*>* objectsToBeDeleted) {
	std::string str;

	//TODO: LOCK fixedOBjects + players WHILE DOING DIFF STATE
	str += this->generateObjectsDifStateMessage(objects, recentlyCreatedObjects, objectsToBeDeleted);
	if (_GAMESTATE_DEBUG == 1)
		if(!str.empty())
			std::cout << "DIF STATE >> " << str << std::endl;

	return str;
}
std::string GameState::generateObjectsDifStateMessage(std::map<std::string, GenObject*>* objects, std::vector<GenObject*>* recentlyCreatedObjects, std::vector<GenObject*>* objectsToBeDeleted)
{
	std::stringstream ss;
	unsigned int i=0;

	for (std::map<std::string, GenObject*>::iterator it = objects->begin(); it != objects->end(); it++)
	{
		(*it).second->stepUpdateBeforeDif();
		if((*it).second->hasChanges)
		{
			ss << (*it).second->generateObjectActionMessage((ObjectAction)Update) << SEPARATOR;
		}
		(*it).second->stepUpdateAfterDif();
	}

	for(i=0; i < recentlyCreatedObjects->size(); i++)
	{
		GenObject *obj = (*recentlyCreatedObjects)[i];
		ss << obj->generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
		(*objects)[obj->guid] = obj;
	}
	recentlyCreatedObjects->clear();

	for(i=0; i < objectsToBeDeleted->size(); i++)
	{
		GenObject *obj = (*objectsToBeDeleted)[i];
		ss << obj->generateObjectActionMessage((ObjectAction)Delete) << SEPARATOR;
		delete obj;
	}
	objectsToBeDeleted->clear();

	return ss.str();
}
