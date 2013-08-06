/*
 * GenObj.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */

#include "../defs.hpp"

#ifndef GENOBJ_HPP_
#define GENOBJ_HPP_


class GenObject {
public:
	std::string guid;
	ObjType type;
	int x, y, height, width;
	bool hasChanges;
	unsigned int timeInFrames; //temporary! for bombs!

	GenObject() {
		guid = "";
		type = None;
		x = y = height = width = -1;
		hasChanges = true;
		timeInFrames = 0;
	}

	GenObject(std::string id, ObjType type, int x, int y, int width, int height) {
		this->guid = id;
		this->type = type;
		this->x = x;
		this->y = y;
		this->height = height;
		this->width = width;
		this->hasChanges = false;
		if(type == (ObjType)Bomb)
			timeInFrames = 100;
	}



	/* Create a message that describe the action to be taken by the javascript client.
	 * Message schema should be:
	 *  "Action|Object|Properties"
	 *  where Action is a string of: "A"(Add) or "D"(Delete) or "U"(Update);
	 *  Object is an integer identifying the object which the action is taken upon;
	 *  Properties are the attributes that define the object.
	 *  The message is spliced by a separator "|".
	 */
	std::string generateObjectActionMessage(ObjectAction act) {
		std::stringstream ss;

		switch(act) {
		case Add:
			ss << "[A]" << getType() << ':' << guid << ';' << x << ';' << y << ';'<< width << ';' << height;
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

	void Move(int x, int y) {
		hasChanges = true;
		this->x += x;
		this->y += y;
	}

	std::string getType() {
		switch(type) {
		case Square:
			return "Square";
			break;
		case Bomber:
			return "Bomber";
			break;
		case Bomb:
			return "Bomb";
			break;
		default:
			return "None";
			break;
		}
		return "";
	}
};



#endif /* GENOBJ_HPP_ */
