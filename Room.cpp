#include "Room.h"
#include "User.h"
#include "Helper.h"
#include "Protocol.h"
using namespace std;


string Room::get_users_as_string(vector<User*> user_list, User* excluded_user){
	string str;
	for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it){
		if (excluded_user != *it){
			str.append((*it)->get_user_name());
		}
	}
	return str;
}

bool Room::join_room(User* user){
	string str;
	string str2;
	if (_users.size() < (unsigned int)_max_users){
		_users.push_back(user);
		str.append(RES_JOIN_ROOM);
		str.append(Helper::getPaddedNumber(_num_of_question, 2));
		str.append(Helper::getPaddedNumber(_time_for_question, 2));
		(*user).send(str);
		str2 = get_users_messages_list();
		sendMessage(str2);
		return true;
	}
	else{
		(*user).send(RES_JOIN_ROOM_FAIL);
		return false;
	}

}

void Room::leave_room(User* user){
	string str;
	int i = 0;
	for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it, i++) {
		if (user == *it){
			_users.erase(_users.begin() + i);
		}
	}
	str = get_users_messages_list();
	sendMessage(user, str);
}

int Room::close_room(User* user){
	if (user != _admin){
		return -1;
	}
	sendMessage(RES_CLOSE_ROOM);
	for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if(*it != _admin)
			(*it)->clear_room();
	}
	return 1;
}

vector<User*> Room::get_users(){
	return _users;
}

string Room::get_users_messages_list(){
	string str;
	string name;
	int value;
	string value_;
	if (_users.size() > 0){
		str.append(RES_USERS_LIST);
		str.append(to_string(_users.size()));
		for (int i = 0; i < _users.size(); i++){
			name = _users[i]->get_user_name();
			value = name.length();
			str.append(to_string(value));
			str.append("0");
			str.append(name);
		}
		return str;
	}
	else{
		return RES_USERS_LIST_NOT_EXIST;
	}
}

int Room::get_id(){
	return _id;
}

string Room::get_name(){
	return _name;
}

int Room::get_question_no(){
	return _num_of_question;
}

int Room::get_question_time(){
	return this->_time_for_question;
}

void Room::sendMessage(string str){
	sendMessage(NULL, str);
}

void Room::sendMessage(User* user, string str){
	for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it){
		if (user != *it){
			(*it)->send(str);
		}
	}
}