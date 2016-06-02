#include "DataBase.h"
#include "Question.h"
DataBase::DataBase(){
	rc = sqlite3_open("FirstPart.db", &db);

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
	_question = NEW question(atoi(argv[0]),argv[1],argv[2],argv[3],argv[4],argv[5]);
	return 0;
}

int DataBase::check_user(void* notUsed, int argc, char** argv, char** azCol){_user = argv[0]; return 0;}
int DataBase::get_pass(void* notUsed, int argc, char** argv, char** azCol){_pass = argv[0]; return 0;}

bool DataBase::is_user_exsits(string name){

	string quarry = "select count(*) from t_users where username = " + '"' + name + '"' + ';';
	const char* full_Query = query.c_str();
	rc = sqlite3_exec(db,full_Query, check_user, 0, &zErrMsg);
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
	string quarry = "insert into t_users values(";
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
	return false;
}

bool DataBase::is_user_and_pass_match(string name , string password){
	string quarry = "select password from t_users where id = " + name + ";" 
	const char * full_Query = query.c_str();
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
	string const querry = "select * from t_questions where question_id = ";
	string full_Query;
	for(int i = 0; i <= NOQ ; i++){
		full_Query = querry + to_string(i) + ';';
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