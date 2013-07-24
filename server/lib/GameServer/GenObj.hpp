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

	GenObject() {
		guid = "";
		type = None;
		x = y = height = width = -1;
		hasChanges = true;
	}

	GenObject(std::string id, ObjType type, int x, int y, int height, int width) {
		this->guid = id;
		this->type = type;
		this->x = x;
		this->y = y;
		this->height = height;
		this->width = width;
		if(type == Bomber)
			hasChanges = true;
		else
			hasChanges = false;
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
			ss << "A" << SEPARATOR << guid << SEPARATOR << getType() << SEPARATOR << x << SEPARATOR << y << SEPARATOR << width << SEPARATOR << height;
			break;
		case Update:
			ss << "U" << SEPARATOR << guid << SEPARATOR << x << SEPARATOR << y;
			break;
		case Delete:
			ss << "D" << SEPARATOR << guid;
			break;
		default:
			ss << "invalid";
			break;
		}

		return ss.str();
/*
		if (remove) {
			switch(type) {
			case bomber:
				ss << "bomber" << SEPARATOR << guid << SEPARATOR << "delete";
				break;
			default:
				ss << "invalid";
				break;
			}
		}
		else {
			switch(type) {
			case square:
				ss << "sq" << SEPARATOR << x << SEPARATOR << y << SEPARATOR << width << SEPARATOR << height;
				break;
			case bomber:
				ss << "bomber" << SEPARATOR << guid << SEPARATOR << x << SEPARATOR << y;
				break;
			default:
				ss << "invalid";
				break;
			}
		}
		ss << SEPARATOR;
		return ss.str();
		*/
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
		default:
			return "None";
			break;
		}
		return "";
	}
};



#endif /* GENOBJ_HPP_ */
