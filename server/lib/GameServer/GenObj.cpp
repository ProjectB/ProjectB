/*
 * GenObj.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: kodo
 */

#include "GenObj.hpp"

using namespace std;
#include <string>
#include <sstream>
#include <iostream>


GenObject::GenObject()
{
	guid = "";
	type = ObjType::_None;
	x = y = height = width = -1;
	hasChanges = true;
	timeInFrames = 0;
}
GenObject::GenObject(std::string id, ObjType type, int x, int y, int width, int height)
{
	this->guid = id;
	this->type = type;
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->hasChanges = false;
	if(type == ObjType::_Bomb)
		timeInFrames = 100;
}
GenObject::~GenObject()
{
}
std::string GenObject::generateObjectActionMessage(ObjectAction act) const {
	std::stringstream ss;

	switch(act) {
	case Add:
		ss << "[A]" << this->getGenObjectType() << ':' << guid << ';' << x << ';' << y << ';'<< width << ';' << height;
		break;
	case Update:
		ss << "[U]" << guid << ';' << x << ';' << y;
		break;
	case Delete:
		ss << "[D]" << guid;
		break;
	default:
		ss << "";
		break;
	}

	return ss.str();
}
void GenObject::Move(int x, int y)
{
	this->hasChanges = true;
	this->x += x;
	this->y += y;
}
std::string GenObject::getGenObjectType() const
{
	switch(this->type)
	{
		case ObjType::_Square:
			return "Square";
			break;
		case ObjType::_Bomber:
			return "Bomber";
			break;
		case ObjType::_Bomb:
			return "Bomb";
			break;
		default:
			return "None";
			break;
	}
	return "";
}

void GenObject::update(std::string msg)
{
	return;
}
