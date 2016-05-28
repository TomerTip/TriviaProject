#pragma once

#include "User.h"
#include "Room.h"
#include "Helper.h"
#include "Protocol.h"
using namespace std;


/*Game *User::get_game(){
	return _currGame;
}*/

Room *User::get_room(){
	return _currRoom;
}

SOCKET User::get_sock(){
	return _sock;
}

string User::get_user_name() {
	return _username;
}
/*void User::set_game(Game * gm){
	_currRoom = nullptr;
	_currGame = gm;
}*/

void  User::send(string str){
	Helper::sendData(this->_sock, str);
}

void User::clear_room(){
	if(_currRoom)
		_currRoom = nullptr;
}

bool User::create_room(int room_id, string room_name, int max_users, int question_num, int question_time){
	if (_currRoom){
		send(RES_CREATE_ROOM_FAIL);
		return false;
	}
	Room *room = new Room(room_id, this, room_name, max_users, question_num, question_time);
	this->_currRoom = room;
	send(RES_CREATE_ROOM);
	return true;
}

bool User::join_room(Room* newRoom){
	if (_currRoom){
		return false;
	}
	else{
		_currRoom = newRoom;
		send(RES_JOIN_ROOM);
		return true;
	}
}

void User::leave_room(){
	_currRoom->leave_room(this);
	_currRoom = nullptr;
}

int User::close_room(){
	if (_currRoom) 
		if (_currRoom->close_room(this) == 1) 
			_currRoom = nullptr;
	return -1;
}

/*
bool User::leave_game(){
	
}
*/