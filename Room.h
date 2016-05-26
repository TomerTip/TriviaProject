#pragma once

#include <string>
#include <vector>

#include "User.h"


using namespace std;

class User;
class Room{
private:
	vector<User*> _users;
	User* _admin;
	int _max_users;
	int _num_of_question;
	int _time_for_question;
	int _id;
	string _name;

	string get_users_as_string(vector<User*>, User*);
	void sendMessage(string);
	void sendMessage(User*, string);

public:
	Room::Room(int id, User* admin, string name, int max_users, int question_num, int question_time) {
		_admin = admin;
		_name = name;
		_id = id;
		_max_users = max_users;
		_time_for_question = question_time;
		_num_of_question = question_num;
	}
	bool join_room(User* user);
	void leave_room(User* user);
	int close_room(User* user);
	vector<User*> get_users();
	string get_users_messages_list();
	int get_id();
	string get_name();
	int get_question_no();

};