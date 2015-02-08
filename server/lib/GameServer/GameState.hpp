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
	std::vector<std::string> npObjects; //guids of Non-player Objects
	std::vector<std::string> players; //guids of the players
	std::map<std::string, GenObject> objects; //dictionary to any object with a guid in the game

	GameState();
	~GameState();
	GenObject newObject(short int posX, short int posY, ObjType);
	void deleteObject(unsigned int);
	std::string updateNPObjects();
	GenObject createPlayer(int, int);
	std::string generateDifStateMessage();
	std::string generateObjectsDifStateMessage();
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
