#include <string>
#include <vector>

#pragma once
#include "Game.h"
#pragma once
#include "Room.h"

using namespace std;

class User{
private:
	string _username;
	Game * _currGame;
	Room* _currRoom;
	SOCKET _sock;
public:
	User(string, SOCKET);
	void send(string);
	string get_user_name();
	SOCKET get_sock();
	Room* get_room();
	Game * get_game();
	void set_game(Game*);
	void clear_room();
	bool create_room(int,string,int,int,int);
	bool join_room(Room*);
	void leave_room();
	bool leave_game();
	int close_room();
};

