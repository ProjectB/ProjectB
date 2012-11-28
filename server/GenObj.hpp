/*
 * GenObj.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */

#include "defs.hpp"

#ifndef GENOBJ_HPP_
#define GENOBJ_HPP_

    class GenObject {
    public:
        std::string guid;
        objType type;
        int x, y, height, width;

        GenObject() {
            guid = "";
            type = none;
            x = y = height = width = -1;
        }

        GenObject(std::string id, objType type, int x, int y, int height, int width) {
            this->guid = id;
            this->type = type;
            this->x = x;
            this->y = y;
            this->height = height;
            this->width = width;
        }

        std::string getMsg() {
            std::stringstream ss;
            if (type == square)
                ss << "sq" << SEPARATOR << x << SEPARATOR << y << SEPARATOR << width << SEPARATOR << height;
            else if (type == bomber)
                ss << "bomber" << SEPARATOR << guid << SEPARATOR << x << SEPARATOR << y;
            else
                ss << "invalid";

            ss << SEPARATOR;

            return ss.str();
        }
    };

#endif /* GENOBJ_HPP_ */
