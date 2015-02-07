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

	GameState();
	~GameState();
	GenObject newNPObject(short int posX, short int posY, ObjType);
	void deleteNPObject(unsigned int);
	std::string updateNPObjects();
	GenObject createPlayer(int, int);
	std::string generateDifStateMessage(bool);
	std::string generateNPObjectsDifStateMessage(bool);
	std::string generatePlayersDifStateMessage(bool);
	void update(std::string, std::string);
	std::vector<std::string> splitMessages(std::string);
	std::string getTopic(std::string) const;
	std::vector<std::string> getAttributes(unsigned int, std::string) const;
	/*
	 * collision test for fixed objects. Possible problems with 2 moving objects (crossing each other)
	 * false = collision happens, true otherwise
	 */
	bool collisionTest(int, int, int, int) const;

private:
	/* squared distance from point x to point y. */
	int distance(int, int, int, int) const;
};
#endif /* GAMESTATE_HPP_ */
