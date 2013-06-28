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
	objType type;
	int x, y, height, width;
	bool hasChanges;

	GenObject() {
		guid = "";
		type = none;
		x = y = height = width = -1;
		hasChanges = true;
	}

	GenObject(std::string id, objType type, int x, int y, int height, int width) {
		this->guid = id;
		this->type = type;
		this->x = x;
		this->y = y;
		this->height = height;
		this->width = width;
		if(type == bomber)
			hasChanges = true;
		else
			hasChanges = false;
	}

	std::string getMsg(bool withFirstSeparator = false, bool remove = false) {
		std::stringstream ss;
		if (withFirstSeparator)
			ss << SEPARATOR;

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
	}

	void Move(int x, int y) {
		hasChanges = true;
		this->x += x;
		this->y += y;
	}
};

#endif /* GENOBJ_HPP_ */
