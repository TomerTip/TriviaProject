#include <string>
#include<vector>
#include "Question.h"
#include "sqlite3.h"

using namespace std;


class DataBase
{
public:
	DataBase();
	~DataBase();
	bool is_user_exsits(string){

	}
	bool add_new_user(string, string, string);
	bool is_user_and_pass_match(string, string);
	vector<Question*> init_question(int);
	vector<string> get_best_scores();
	bool inset_new_game();
	bool add_ans_to_player(int, string, int, string, bool, int);
	vector<string> get_personal_status();



private:
	static int callback_count(void*, int, char**, char**);
	static int callback_questions(void*, int, char**, char**);
	static int callback_best_scores(void*, int, char**, char**);
	static int callback_personal_status(void*, int, char**, char**);
};
