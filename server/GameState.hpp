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

    void update(std::string guid, std::string msg) {
        int xdif = 0, ydif = 0;

        if (msg.compare("leftKey") == 0)
            xdif = -xMove;
        else if (msg.compare("rightKey") == 0)
            xdif = xMove;
        else if (msg.compare("upKey") == 0)
            ydif = -yMove;
        else if (msg.compare("downKey") == 0)
            ydif = yMove;

        if (collisionTest(players[guid].x + xdif, players[guid].y + ydif, players[guid].width, players[guid].height))
            players[guid].Move(xdif, ydif);
        else
            printf("DEBUG: COLLISION\n");
    }

    //fixed collision test. Possible problems with 2 moving objects (crossing each other)
    //false = collision happens, true otherwise
    bool collisionTest(int x, int y, int w, int h) {
        for (unsigned int i = 0; i < fixedObjects.size(); i++)
            if (fixedObjects[i].type == square) {
                if (intersects(x, x + w, fixedObjects[i].x, fixedObjects[i].x + fixedObjects[i].width)
                        && intersects(y, y + h, fixedObjects[i].y, fixedObjects[i].y + fixedObjects[i].height)) {
                    printf("DEBUG: %d %d %d %d -- %d %d %d %d\n", x, x + w, fixedObjects[i].x, fixedObjects[i].x + fixedObjects[i].width, y, y + h, fixedObjects[i].y,
                            fixedObjects[i].y + fixedObjects[i].height);
                    return false;
                }
            }
        return true;
    }

private:

    //true = objects intersect, false otherwise
    bool intersects(int x1, int x2, int y1, int y2) {
        if (x1 >= y1 && x1 <= y2)
            return true;
        if (x2 >= y1 && x2 <= y2)
            return true;
        return false;
    }
};

#endif /* GAMESSTATE_HPP_ */
