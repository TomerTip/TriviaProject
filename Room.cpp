#include "Room.h"
#include "User.h"
#include "Helper.h"
using namespace std;

Room::Room(int id, User* admin, string name, int max_users, int question_num, int question_time){
	_admin = admin;
	_name = name;
	_id = id;
	_max_users = max_users;
	_time_for_question = question_time;
	_num_of_question = question_num;
}

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
	if (_users.size < _max_users){
		_users.push_back(user);
		str.append("110");
		str.append(Helper::getPaddedNumber(_num_of_question, 2));
		str.append(Helper::getPaddedNumber(_time_for_question, 2));
		(*user).send(str);
		str2 = get_users_messages_list();
		sendMessage(str2);
	}
	else{
		(*user).send("1101");
		return false;
	}

}

void Room::leave_room(User* user){
	string str;
	for (vector<User*>::iterator it = _users.begin(),int i =0; it != _users.end(); ++it,i++){
		if (user == *it){
			_users.erase(_users.begin() + i);
		}
	}
	str = get_users_messages_list();
	sendMessage(user, str);
}
/*int Room::close_room(User* user){
	if (user != _admin){
		return -1;
	}

}*/
vector<User*> Room::get_users(){
	return _users;
}
string Room::get_users_messages_list(){
	string str;
	string name;
	int value;
	string value_;
	if (_users.size() > 0){
		str.append("108");
		for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it){
			name = (*it)->get_user_name();
			value = name.length();
			value_ = Helper::getPaddedNumber(value, 2);
			str.append(value_);
			str.append(name);
		}
		return str;
	}
	else{
		return "1080";
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