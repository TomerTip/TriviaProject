#include "DataBase.h"

DataBase::DataBase()
{
	myfile.open("C:\\Users\\User\\Documents\\Magshimim\\'שנה ב\\עקרונות מתקדמים\\סמסטר ב\\Trivia Project!\\Trivia\\Trivia\\list.txt");
}
DataBase::~DataBase()
{
	this->myfile.close();
}
bool DataBase::is_user_exits(string name)
{
	string line;
	bool found = false;
	while (getline(this->myfile, line) && !found)
	{
		if (line.find(name))
		{
			return true;
		}
	}
	return false;
}
bool DataBase::add_new_user(string user, string pass, string email)
{
	string line = user + ',';
	line.append(pass + ',');
	line.append(email);

	this->myfile.write(line.c_str(), line.length());
	return myfile.bad();
}
bool DataBase::is_user_and_pass_match(string user, string pass)
{
	string line;
	bool found = false;
	while (getline(this->myfile, line) && !found)
	{
		if (line.find(user) && line.find(pass))
		{
			return true;
		}
	}
	return false;
	
}

/*

vector<Question*> DataBase::init_question(int)
{

}
vector<string> DataBase::get_best_scores()
{

}
bool DataBase::inset_new_game()
{

}
bool DataBase::add_ans_to_player(int, string, int, string, bool, int)
{

}
vector<string> DataBase::get_personal_status()
{

}
*/