/*
 * GenObj.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */

#include "../defs.hpp"

#ifndef GENOBJ_HPP_
#define GENOBJ_HPP_

using namespace std;
#include <string>
#include <sstream>
#include <iostream>

class GenObject
{
public:
	std::string guid;
	ObjType type;
	int x, y, height, width;
	bool hasChanges;
	unsigned int timeInFrames; //temporary! for bombs!

	GenObject();
	GenObject(std::string, ObjType, int, int, int, int);
	/* Create a message that describe the action to be taken by the javascript client.
	 * Message schema should be:
	 *  "Action|Object|Properties"
	 *  where Action is a string of: "A"(Add) or "D"(Delete) or "U"(Update);
	 *  Object is an integer identifying the object which the action is taken upon;
	 *  Properties are the attributes that define the object.
	 *  The message is spliced by a separator "|".
	 */
	std::string generateObjectActionMessage(ObjectAction) const;
	void Move(int, int);
	std::string getGenObjectType() const;
	void update(std::string);
};



#endif /* GENOBJ_HPP_ */
