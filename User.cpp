#include "User.h"
#include "Helper.h"
#include "Room.h"
using namespace std;


User::User(string name, SOCKET sock){
	_username = name;
	_sock = sock;
}

Game * User::get_game(){
	return _currGame;
}

Room * User::get_room(){
	return _currRoom;
}

SOCKET User::get_sock(){
	return _sock;
}

void User::set_game(Game * gm){
	_currRoom = nullptr;
	_currGame = gm;
}

void  User::send(string str){
	Helper::sendData(this->_sock, str);
}

void User::clear_room(){
	_currGame = nullptr;
}

bool User::create_room(int room_id, string room_name, int max_users, int question_num, int question_time){
	if (_currRoom){
		send("1141");
		return false;
	}
	Room * room = new Room(room_id, this, room_name, max_users, question_num, question_time);
	send("1140");
	return true;
}

bool User::join_room(Room* newRoom){
	if (_currRoom){
		return false;
	}
	else{
		_currRoom = newRoom;

		return true;
	}
}

void User::leave_room(){

}

int User::close_room(){

}

bool User::leave_game(){

}