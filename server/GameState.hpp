/*
 * GamesState.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */
#include "defs.hpp"
#include "GenObj.hpp"

#ifndef GAMESSTATE_HPP_
#define GAMESSTATE_HPP_

    class GameState {
    public:
        int width, height, xPoint, yPoint, xMove, yMove, n;
        std::vector<GenObject> fixedObjects;
        std::map<std::string, GenObject> players;

        GameState() {
            n = 0;
            height = width = 11 * BLOCK_SIZE;
            xPoint = BLOCK_SIZE;
            yPoint = BLOCK_SIZE;
            xMove = BLOCK_SIZE / 5;
            yMove = BLOCK_SIZE / 5;

            newObject(xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(3 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(3 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(5 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(5 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(7 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(7 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);

            newObject(9 * xPoint, yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 3 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 5 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 7 * yPoint, xPoint - 2, yPoint - 2, square);
            newObject(9 * xPoint, 9 * yPoint, xPoint - 2, yPoint - 2, square);
        }

        void newObject(int x, int y, int w, int h, objType type) {
            std::stringstream ss;
            ss << (n++);
            GenObject square(ss.str(), type, x, y, w, h);
            fixedObjects.push_back(square);
        }

        std::string getMsg(bool onlyDiff = true) {
            std::string str;
            str += SEPARATOR;

            str += msgFixedObjects(onlyDiff);
            str += msgPlayers(onlyDiff);

            for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
                (*it).second.hasChanges = false;

            return str;
        }

        std::string msgFixedObjects(bool onlyDiff) {
            std::stringstream ss;

            for (unsigned int i = 0; i < fixedObjects.size(); i++)
                if (fixedObjects[i].hasChanges || !onlyDiff)
                    ss << fixedObjects[i].getMsg();

            return ss.str();
        }

        std::string msgPlayers(bool onlyDiff) {
            std::stringstream ss;

            for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++)
                if ((*it).second.hasChanges || !onlyDiff)
                    ss << (*it).second.getMsg();

            return ss.str();
        }

    };

#endif /* GAMESSTATE_HPP_ */
