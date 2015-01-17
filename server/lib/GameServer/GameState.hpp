/*
 * GameState.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: ricardo
 */

#include <vector>
#include <map>



#include "../defs.hpp"
#include "GenObj.hpp"

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_

class GameState {
public:
	int width, height, xMove, yMove;
	unsigned int n;
	std::vector<GenObject> recentlyCreatedObjects;
	std::vector<GenObject> NPObject; //Non-playable/player Object
	std::map<std::string, GenObject> players;

	GameState() {
		n = 0;
		height = width = 11 * BLOCK_SIZE;
		xMove = 4;
		yMove = 4;


		newNPObject(2 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(2 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(2 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(2 * BLOCK_SIZE, 8 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);

		newNPObject(4 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(4 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(4 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(4 * BLOCK_SIZE, 8 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);

		newNPObject(6 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(6 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(6 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(6 * BLOCK_SIZE, 8 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);

		newNPObject(8 * BLOCK_SIZE, 2 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(8 * BLOCK_SIZE, 4 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(8 * BLOCK_SIZE, 6 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
		newNPObject(8 * BLOCK_SIZE, 8 * BLOCK_SIZE, BLOCK_SIZE - 2, BLOCK_SIZE - 2,Square);
	}

	~GameState() {
		recentlyCreatedObjects.clear();
		NPObject.clear();
		players.clear();
	}

	void newNPObject(int x, int y, int w, int h, ObjType type) {
		std::stringstream ss;
		ss << (n++);

		switch(type) {
		case Square:
			recentlyCreatedObjects.push_back(GenObject(ss.str(), type, x, y, w, h));
			break;
		case Bomb:
			recentlyCreatedObjects.push_back(GenObject(ss.str(), type, x, y, w, h));
			break;
		default:
			return;
			break;
		}

		if (_GAMESTATE_DEBUG == 1) {
			std::cout << "OBJECT CREATED[" << type << "]: n=" << n-1 << ": (" << x << "," << y << ")";
			std::cout << " (" << x+w << "," << y << ")";
			std::cout << " (" << x+w << "," << y+h << ")";
			std::cout << " (" << x << "," << y+h << ")" << std::endl;
			//printf("OBJECT CREATED: number %d: (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n", n-1, x, y, x+w, y, x+w, y+h, x, y+h);
		}
	}

	void deleteNPObject(unsigned int i) {
		NPObject.erase(NPObject.begin() + i);
	}

	std::string updateNPObjects() {
		std::stringstream ss;
		for(unsigned int i = 0; i < NPObject.size(); i++) {
			switch(NPObject[i].type) {
			case Bomb:
				if(--NPObject[i].timeInFrames == 0) {
					ss << NPObject[i].generateObjectActionMessage((ObjectAction)Delete) << SEPARATOR;
					deleteNPObject(i);
				}
				break;
			default:
				break;
			}
		}

		return ss.str();
	}

	GenObject createPlayer(int x, int y, int w, int h, std::string guid) {
		return GenObject(guid, Bomber, x, y, BOMBER_WIDTH, BOMBER_HEIGHT);
	}

	std::string generateDifStateMessage(bool firstDiff = false) {
		std::string str;

		//TODO: LOCK fixedOBjects + players WHILE DOING DIFF STATE
		str += generateNPObjectsDifStateMessage(firstDiff);
		str += generatePlayersDifStateMessage(firstDiff);
		if (_GAMESTATE_DEBUG == 1)
			if(!str.empty())
				std::cout << "DIF STATE >> " << str << std::endl;

		return str;
	}

	std::string generateNPObjectsDifStateMessage(bool firstDiff) {
		std::stringstream ss;
		unsigned int i=0;

		if(firstDiff) {
			for (i = 0; i < NPObject.size(); i++) {
				ss << NPObject[i].generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
			}
		}
		else {
			for(i=0; i < recentlyCreatedObjects.size(); i++) {
				ss << recentlyCreatedObjects[i].generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
				NPObject.push_back(recentlyCreatedObjects[i]);
			}
			recentlyCreatedObjects.clear();
			for (i = 0; i < NPObject.size(); i++) {
				if(NPObject[i].hasChanges) {
					ss << NPObject[i].generateObjectActionMessage((ObjectAction)Update) << SEPARATOR;
					NPObject[i].hasChanges = false;
				}
			}
		}

		return ss.str();
	}

	std::string generatePlayersDifStateMessage(bool firstDiff) {
		std::stringstream ss;

		if(firstDiff) {
			for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++) {
				ss << (*it).second.generateObjectActionMessage((ObjectAction)Add) << SEPARATOR;
			}
		}
		else {
			for (std::map<std::string, GenObject>::iterator it = players.begin(); it != players.end(); it++) {
				if((*it).second.hasChanges) {
					ss << (*it).second.generateObjectActionMessage((ObjectAction)Update) << SEPARATOR;
					(*it).second.hasChanges = false;
				}
			}
		}

		return ss.str();
	}

	void update(std::string guid, std::string msg) {
		int xdif = 0, ydif = 0;

		//translate input message into a vector of atomic inputs

		std::vector<std::string> input = splitMessages(msg);

		//process these inputs
		for(std::vector<std::string>::const_iterator it = input.begin(); it != input.end(); it++) {
			std::string topic = getTopic(*it);
			if (topic.compare("moveLeft") == 0)
				xdif = -xMove;
			else if (topic.compare("moveRight") == 0)
				xdif = xMove;
			else if (topic.compare("moveUp") == 0)
				ydif = -yMove;
			else if (topic.compare("moveDown") == 0)
				ydif = yMove;
			else if (topic.compare("dropBomb") == 0) {
				std::vector<std::string> attr = getAttributes(topic.size(), *it);
				newNPObject(atoi(attr[0].c_str()), atoi(attr[1].c_str()), 20, 20, Bomb);
			}
			else {
				std::cout << "WARNING: UNKNOWN TOPIC >> " << topic << std::endl;
			}
		}

		if(xdif != 0 || ydif != 0)
			this->players[guid].Move(xdif, ydif);

		if (_GAMESTATE_DEBUG == 1) {
			std::cout << "bomber: topleft(" << players[guid].x << "," << players[guid].y << ") topright(" << players[guid].x + players[guid].width;
			std::cout << "," << players[guid].y << ") bottomright(" << players[guid].x + players[guid].width << "," << players[guid].y + players[guid].height;
			std::cout << ") bottomleft(" << players[guid].x << "," << players[guid].y + players[guid].height << ")" << std::endl;
		}
	}

	std::vector<std::string> splitMessages(std::string msg) {
		std::vector<std::string> output;

		std::string currMsg;
		for(unsigned int i=0; i < msg.length(); i++) {
			if(msg[i] == (char)SEPARATOR) {
				output.push_back(currMsg);
				currMsg.clear();
			} else {
				currMsg.push_back(msg[i]);
			}
		}

		return output;
	}

	std::string getTopic(std::string msg) {
		std::string topic; //output

		unsigned int i=0;
		unsigned int len = msg.length();

		while(msg[i] != '|' && msg[i] != ':' && i < len)
			topic.push_back(msg[i++]);

		return topic;
	}

	std::vector<std::string> getAttributes(unsigned int offset, std::string msg) {
		std::vector<std::string> attributes;

		unsigned int i=offset;
		std::string str = "";

		while(msg[i++] != ':');

		for(;i < msg.size(); i++) {
			while(msg[i] != ';' && i < msg.size())
				str.push_back(msg[i++]);
			attributes.push_back(str);
			str.clear();
		}

		return attributes;
	}

	//collision test for fixed objects. Possible problems with 2 moving objects (crossing each other)
	//false = collision happens, true otherwise

	bool collisionTest(int x, int y, int w, int h) {
		/* outside screen */
		if(x < 0 || y < 0 || x > SCREEN_WIDTH - w || y > SCREEN_HEIGHT - h) return false;

		int px,py;
		int n;
		int leastDist=-1;
		int centerx, centery;
		int a;

		/* find nearest object */
		for (unsigned int i = 0; i < NPObject.size(); i++) {
			if (NPObject[i].type == Square) {
				if(leastDist<0) {
					leastDist = distance(x,y,NPObject[i].x + NPObject[i].width, NPObject[i].y + NPObject[i].height);
					n = i;
				}
				else if((a = distance(x,y,NPObject[i].x + NPObject[i].width, NPObject[i].y + NPObject[i].height)) < leastDist)
				{
					n = i;
					leastDist = a;
				}
			}
		}

		centerx = NPObject[n].x + NPObject[n].width/2;
		centery = NPObject[n].y + NPObject[n].height/2;

		/* find nearest point to the center of the object */
		leastDist = distance(x,y,centerx,centery);
		px=x;
		py=y;

		if((a = distance(x+w,y, centerx,centery))< leastDist){
			px = x+w;
			leastDist = a;
		}
		if((a = distance(x+w, y+h, centerx,centery)) < leastDist) {
			px = x+w;
			py = y+h;
			leastDist = a;
		}
		if((a = distance(x,y+h, centerx,centery)) < leastDist) {
			px = x;
			py = y+h;
		}

		/* if the nearest point is farther than the radius of the object */
		if(distance(px,py,centerx, centery) > distance(NPObject[n].x, NPObject[n].y, centerx, centery)) {
			std::cout << "DEBUG: FARTHER THAN POSSIBLE" << std::endl;
			//printf("DEBUG: FARTHER THAN POSSIBLE\n");
			return true;
		}

		/* 8 points of interest of the object */
		/* top */
		int p1x = centerx;
		int p1y = NPObject[n].y;

		/* right */
		int p2x = NPObject[n].x + NPObject[n].width;
		int p2y = centery;

		/* bottom */
		int p3x = centerx;
		int p3y = NPObject[n].y + NPObject[n].height;

		/* left */
		int p4x = NPObject[n].x;
		int p4y = centery;

		/* top left */
		int p5x = NPObject[n].x;
		int p5y = NPObject[n].y;

		/* top right */
		int p6x = NPObject[n].x + NPObject[n].width;
		int p6y = NPObject[n].y;

		/* bottom left */
		int p7x = NPObject[n].x;
		int p7y = NPObject[n].y + NPObject[n].height;

		/* bottom right */
		int p8x = NPObject[n].x + NPObject[n].width;
		int p8y = NPObject[n].y + NPObject[n].height;


		/* find the nearest PoI */
		int obx,oby;

		obx=p1x;
		oby=p1y;

		if(distance(px,py,p2x,p2y) < distance(px,py,obx,oby)) {
			obx = p2x;
			oby = p2y;
		}

		if(distance(px,py,p3x,p3y) < distance(px,py,obx,oby)) {
			obx = p3x;
			oby = p3y;
		}

		if(distance(px,py,p4x,p4y) < distance(px,py,obx,oby)) {
			obx = p4x;
			oby = p4y;
		}

		if(distance(px,py,p5x,p5y) < distance(px,py,obx,oby)) {
			obx = p5x;
			oby = p5y;
		}

		if(distance(px,py,p6x,p6y) < distance(px,py,obx,oby)) {
			obx = p6x;
			oby = p6y;
		}

		if(distance(px,py,p7x,p7y) < distance(px,py,obx,oby)) {
			obx = p7x;
			oby = p7y;
		}

		if(distance(px,py,p8x,p8y) < distance(px,py,obx,oby)) {
			obx = p8x;
			oby = p8y;
		}


		std::cout << "bomber(" << x << "," << y << "," << w << "," << h << ") -- p(" << px << "," << py << ") -- center[" << n << "](" << centerx << "," << centery;
		std::cout << ") -- PoI(" << obx << "," << oby << ") -- bottom(" << x << "," << y+h << ") -- top(" << centerx << "," << centery + NPObject[n].height/2 << ")." << std::endl;
		std::cout << "both distances: " << distance(px, py, centerx, centery) << " ---- " << distance(obx, oby, centerx, centery) << "." << std::endl;
		//printf("bomber(%d,%d,%d,%d) -- p(%d,%d) -- center[%d](%d,%d) -- PoI(%d,%d) -- bottom(%d,%d) -- top(%d,%d).\n", x,y,w,h, px,py, n,centerx,centery, obx,oby, x,y+h, centerx, centery + fixedObjects[n].height/2);
		//printf("both distances: %d ---- %d.\n", distance(px,py,centerx,centery), distance(obx,oby,centerx,centery));

		/* if the distance point-center is less than or equal to the distance to the nearest PoI, then we have a collision */
		if(distance(px,py,centerx, centery) <= distance(obx,oby,centerx,centery)) {
			std::cout << "DEBUG: COLLISION ENDED!" << std::endl;
			//printf("DEBUG: COLLISION ENDED!\n");
			return false;
		}

		std::cout << "end" << std::endl;
		//printf("end\n");


		return true;
	}


private:

	/* squared distance from point x to point y. */
	int distance(int x1, int x2, int y1, int y2)
	{
		int a = (x1 - y1)*(x1 - y1) + (x2 - y2)*(x2 - y2);
		if (a < 0) return -a;
		return a;
	}
};
#endif /* GAMESTATE_HPP_ */
