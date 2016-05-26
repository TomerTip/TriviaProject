#include <string>
#include <iostream>
#include<vector>
#include <fstream>

using namespace std;

#include "Question.h"

class DataBase
{
public:
	DataBase();
	~DataBase();
	bool is_user_exits(string);
	bool add_new_user(string, string, string);
	bool is_user_and_pass_match(string, string);
	vector<Question*> init_question(int);
	vector<string> get_best_scores();
	bool inset_new_game();
	bool add_ans_to_player(int, string, int, string, bool, int);
	vector<string> get_personal_status();



private:
	fstream myfile;
	static int callback_count(void*, int, char**, char**);
	static int callback_questions(void*, int, char**, char**);
	static int callback_best_scores(void*, int, char**, char**);
	static int callback_personal_status(void*, int, char**, char**);
};
