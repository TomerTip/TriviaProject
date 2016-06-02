#include "Question.h"
#include "sqlite3.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;


class DataBase
{
public:
	DataBase();
	~DataBase();
	bool is_user_exsits(string); //done
	bool add_new_user(string, string, string); //done
	bool is_user_and_pass_match(string, string); //done
	vector<Question*> init_question(int); //done
	//vector<string> get_best_scores();
	bool insret_new_game();
	bool add_ans_to_player(int, string, int, string, bool, int);
	//vector<string> get_personal_status();

	int rc;
	sqlite3* db;
	char *zErrMsg = 0;
	bool is_exist;
	Question* _question;
	unordered_map<string, vector<string>> results;

	string _user;
	string _pass;

	void clearTable();
	void printTable();

private:
	//static int callback_count(void*, int, char**, char**);
	static int callback_questions(void*, int, char**, char**);
	//static int callback_best_scores(void*, int, char**, char**);
	//static int callback_personal_status(void*, int, char**, char**);

	static int check_user(void* notUsed, int argc, char** argv, char** azCol);
	static int get_pass(void* notUsed, int argc, char** argv, char** azCol);
};