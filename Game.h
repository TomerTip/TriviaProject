#include <string>
#include <vector>
#include <winsock2.h>
#include <map>

using namespace std;

#pragma once
#include "DataBase.h"
#include "User.h"
#include "Protocol.h"
#include "Helper.h"

class User;

class Game
{
public:
	Game(vector<User*>&, int, DataBase&);
	~Game();
	void send_first_question();
	/*
	void handle_finish_game();
	bool handle_next_turn();
	bool hanlde_answer_from_user(User*, int, int);
	bool leave_game(User*);
	*/
	void sendMessage(User* user, string str);
	void sendMessage(string str);

private:
	vector<Question*> _questions;
	vector<User*> _users;
	int question_num;
	int _curret_question_index;
	DataBase _db;
	map<string, int> _results;
	int _current_turn_answers;

	//bool insert_game_to_db();
	//void init_questions_from_db();
	void send_question_to_all_users();

};