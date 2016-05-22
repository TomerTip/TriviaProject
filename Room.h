#include <string>
#include <vector>
#include <map>

#include "Helper.h"
#include "Protocol.h"

#pragma once
#include "User.h"

using namespace std;

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
	Room(int, User*, string, int, int, int);
	bool join_room(User*);
	void leave_room(User*);
	int close_room(User*);
	vector<User*> get_users();
	string get_users_messages_list();
	int get_id();
	string get_name();
	int get_question_no();

};