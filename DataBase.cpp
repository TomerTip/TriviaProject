#include "DataBase.h"
#include "Question.h"
#include <time.h>
#include <iostream>


Question* DataBase::_question = nullptr;
string DataBase::_user;
string DataBase::_pass;
int DataBase::_count;


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

int DataBase::check_user(void* notUsed, int argc, char** argv, char** azCol){_user = argv[0]; return 0;}
int DataBase::get_pass(void* notUsed, int argc, char** argv, char** azCol){_pass = argv[0]; return 0;}
int DataBase::callback_count(void* notUsed, int argc, char** argv, char** azCol){ _count = atoi(argv[0]); return 0; }


bool DataBase::is_user_exists(string name){

	string query = "select count(*) from t_users where username = ";
	query.append("'");
	query.append(name);
	query.append("'");
	query.append(";");
	const char* full_Query = query.c_str();
	rc = sqlite3_exec(db,full_Query, this->check_user, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		
		return false;
	}

	if(_user == "0"){
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
	rc = sqlite3_exec(db,full_Query,NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		cout << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		
		return false;
	}
	return true;
}

bool DataBase::is_user_and_pass_match(string name , string password){
	string query = "select password from t_users where username = ";
	query.append("'");
	query.append(name);
	query.append("'");
	query.append(";");
	cout << query << endl;
	const char* full_Query = query.c_str();
	rc = sqlite3_exec(db,full_Query,get_pass, 0, &zErrMsg);
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
	for(int i = 1; i <= NOQ ; i++){
		full_Query = query + to_string(i) + ';';
		rc = sqlite3_exec(db,full_Query.c_str(),callback_questions, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			cout << "SQL error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
		}
		q.push_back(_question);
	}
	return q;
}

int DataBase::insret_new_game(){
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

/*

bool DataBase::add_ans_to_player(int game_id, string username, int question_id, string player_ans, bool is_correct, int anser_time){
	
}
*/

DataBase::~DataBase()
{

}