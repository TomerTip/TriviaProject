#include "DataBase.h"
#include "Question.h"
#include <time.h>
#include <iostream>


Question* DataBase::_question = nullptr;
string DataBase::_user;
string DataBase::_pass;
int DataBase::_count;
string DataBase::_correct;
string DataBase::_false;
string DataBase::_avg_time;
string DataBase::_num_of_games;

DataBase::DataBase(){
	rc = sqlite3_open("trivia.db", &db);
	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(db) << endl;
		sqlite3_close(db);
	}
}

void DataBase::clearTable(){
	for (auto it = results.begin(); it != results.end(); ++it)
	{
		it->second.clear();
	}
	results.clear();
}

void DataBase::printTable(){
	auto iter = results.end();
	iter--;
	//	int size = results.begin()->second.size();
	int size = iter->second.size();
	for (int i = -2; i < size; i++)
	{
		for (auto it = results.begin(); it != results.end(); ++it)
		{
			if (i == -2)
			{
				//cout << it->first << " ";
				printf("|%*s|", 13, it->first.c_str());
			}
			else if (i == -1)
				cout << "_______________";
			else
				//	cout << it->second.at(i) << " ";
				printf("|%*s|", 13, it->second.at(i).c_str());
		}
		cout << endl;
	}
}

int DataBase::callback_questions(void* notUsed, int argc, char** argv, char** azCol){
	Question* qs = new Question(atoi(argv[0]), argv[1], argv[2], argv[3], argv[4], argv[5]);
	_question = qs;
	return 0;
}

bool DataBase::is_user_exists(string name){

	string query = "select count(*) from t_users where username = ";
	query.append("'");
	query.append(name);
	query.append("'");
	query.append(";");
	const char* full_Query = query.c_str();
	rc = sqlite3_exec(db, full_Query, this->check_user, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
	}

	if (_user == "0"){
		return false;
	}
	else{
		return true;
	}
}

bool DataBase::add_new_user(string name, string pass, string email){
	string query = "insert into t_users values(";
	query.append("'");
	query.append(name);
	query.append("'");
	query.append(",");
	query.append("'");
	query.append(pass);
	query.append("'");
	query.append(",");
	query.append("'");
	query.append(email);
	query.append("'");
	query.append(")");
	query.append(";");
	const char * full_Query = query.c_str();
	rc = sqlite3_exec(db, full_Query, NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
	}
	return true;
}

bool DataBase::is_user_and_pass_match(string name, string password){
	string query = "select password from t_users where username = ";
	query.append("'");
	query.append(name);
	query.append("'");
	query.append(";");
	cout << query << endl;
	const char* full_Query = query.c_str();
	rc = sqlite3_exec(db, full_Query, get_pass, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
	}

	if (password == _pass){
		return true;
	}
	else{
		return false;
	}
}

vector<Question*> DataBase::init_question(int NOQ){
	vector<Question*> q;
	string const query = "select * from t_questions where question_id = ";
	string full_Query;
	for (int i = 1; i <= NOQ; i++){
		full_Query = query + to_string(i) + ';';
		rc = sqlite3_exec(db, full_Query.c_str(), callback_questions, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			cout << "SQL error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
		}
		q.push_back(_question);
	}
	return q;
}

int DataBase::insert_new_game(){
	string query = "insert into t_games (status,start_time) values(0,DATETIME(\'now\'));";
	rc = sqlite3_exec(db, query.c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return -1;
	}


	query = "select last_insert_rowid();";
	rc = sqlite3_exec(db, query.c_str(), callback_count, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return -1;
	}

	return _count;
}


bool DataBase::add_ans_to_player(int game_id, string username, int question_id, string player_ans, bool is_correct, int answer_time){
	string query = "insert into t_players_answers values('" + to_string(game_id) + "','" + username + "','" + to_string(question_id) + "','" + player_ans + "','" + to_string(is_correct) + "','" + to_string(answer_time) + "');";
	rc = sqlite3_exec(db, query.c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;
}


DataBase::~DataBase()
{

}

bool DataBase::update_game_status(int id){
	string query = "update t_games set status = '1' , end_time = DATETIME(\'now\') where game_id = '" + to_string(id) + "';";
	rc = sqlite3_exec(db, query.c_str(),NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

		return false;
	}

	return true;

}

vector<string> DataBase::get_personal_status(string username){
	vector<string> q;
	string query1 = "select count(*) username from t_players_answers where username = '" + username + "' and is_correct = 1;";
	string query2 = "select count(*) username from t_players_answers where username = '" + username + "' and is_correct = 0;";
	string query3 = "select AVG(answer_time) from t_players_answers where username = '" + username + "'";
	string query4 = "select count(*) from(select game_id from t_players_answers where username = '" + username + "' group by game_id);";
	rc = sqlite3_exec(db, query1.c_str(), callback_correct, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

	}
	rc = sqlite3_exec(db, query2.c_str(), callback_false, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

	}
	rc = sqlite3_exec(db, query3.c_str(), callback_time, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

	}

	rc = sqlite3_exec(db, query4.c_str(), callback_games, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);

	}

	q.push_back(_num_of_games);
	q.push_back(_correct);
	q.push_back(_false);
	q.push_back(_avg_time);

	return q;
}

vector<string> DataBase::get_best_scores(){

}

int DataBase::callback_time(void* notUsed, int argc, char** argv, char** azCol){_avg_time = argv[0]; return 0;}
int DataBase::callback_correct(void* notUsed, int argc, char** argv, char** azCol){ _correct = argv[0]; return 0; }
int DataBase::callback_false(void* notUsed, int argc, char** argv, char** azCol){ _false = argv[0]; return 0; }
int DataBase::callback_games(void* notUsed, int argc, char** argv, char** azCol){ _num_of_games = argv[0]; return 0; }
int DataBase::check_user(void* notUsed, int argc, char** argv, char** azCol){ _user = argv[0]; return 0; }
int DataBase::get_pass(void* notUsed, int argc, char** argv, char** azCol){ _pass = argv[0]; return 0; }
int DataBase::callback_count(void* notUsed, int argc, char** argv, char** azCol){ _count = atoi(argv[0]); return 0; }
