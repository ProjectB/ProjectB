/*
 * defs.hpp
 *
 *  Created on: Sep 4, 2012
 *      Author: ricardo
 */

#ifndef DEFS_HPP_
#define DEFS_HPP_

#define RCVBUFSIZE 256
#define _SERVER_DEBUG 1
#define _SERVER_ERR_DEBUG 1

#define _GAMESERVER_DEBUG 1
#define _GAMESERVER_ERR_DEBUG 1

#define _CLIENT_DEBUG 1
#define _CLIENT_MSG_DEBUG 0
#define _CLIENT_ERR_DEBUG 1

#define _GAMESTATE_DEBUG 0

/************ GAME DEFINES ************/

enum ObjType {
    _None, _Square, _Bomb, _Bomber, _Player
};

enum ObjectAction {
	Add, Update, Delete
};

#define FPS 30
#define BLOCK_WIDTH 38
#define BLOCK_HEIGHT 38
#define BOMB_WIDTH 20
#define BOMB_HEIGHT 20
#define BOMBER_WIDTH 32
#define BOMBER_HEIGHT 42
#define SEPARATOR '|'
#define SCREEN_WIDTH 440
#define SCREEN_HEIGHT 440

#endif /* DEFS_HPP_ */
