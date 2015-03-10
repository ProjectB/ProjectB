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
	GameState();
	~GameState();
	std::string generateDifStateMessage(std::map<std::string, GenObject*>*, std::vector<GenObject*>*, std::vector<GenObject*>*);
	std::string generateObjectsDifStateMessage(std::map<std::string, GenObject*>*, std::vector<GenObject*>*, std::vector<GenObject*>*);
};
#endif /* GAMESTATE_HPP_ */
