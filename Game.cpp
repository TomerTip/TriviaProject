#include "Game.h"

Game::Game(const vector<User*>& players, int questionNo, DataBase& db)
{
	this->_users = players;
	this->_curret_question_index = questionNo;
	this->_db = db;
}

void Game::send_question_to_all_users(){
	_current_turn_answeres = 0;
	string str;
	vector<Question*>::iterator it;
	str.append(RES_START_GAME);
	if (_questions.size() > 0){
		*it = _questions.back();
		(*it)->get_question();
	}
}

void Game::send_first_question()
{
	send_question_to_all_users();
}

void Game::handle_finish_game()
{

}
bool Game::handle_next_turn()
{

}
bool Game::hanlde_answer_from_user(User*, int, int)
{

}
bool Game::leave_game(User*)
{

}
bool Game::insert_game_to_db()
{

}

void Game::init_questions_from_db()
{

}



void Game::sendMessage(User* user, string str){
	for (vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it){
		if (user != *it){
			(*it)->send(str);
		}
	}
}

void Game::sendMessage(string str){
	sendMessage(NULL, str);
}