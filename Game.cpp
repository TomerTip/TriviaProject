#include "Game.h"

Game::Game(vector<User*>& players, int questionNo, DataBase& db)
{
	this->_users = players;
	this->_curret_question_index = questionNo;
	this->_db = db;

}

void Game::send_question_to_all_users(){
	string message = RES_START_GAME;

	_current_turn_answers = 0;

	string q = this->_questions[this->_curret_question_index]->get_question();
	string ans1 = this->_questions[this->_curret_question_index]->get_answers()[0];
	string ans2 = this->_questions[this->_curret_question_index]->get_answers()[1];
	string ans3 = this->_questions[this->_curret_question_index]->get_answers()[2];
	string ans4 = this->_questions[this->_curret_question_index]->get_answers()[3];

	message.append(Helper::getPaddedNumber(q.size(), 3));
	message.append(q);

	message.append(Helper::getPaddedNumber(ans1.size(), 3));
	message.append(ans1);

	message.append(Helper::getPaddedNumber(ans2.size(), 3));
	message.append(ans2);

	message.append(Helper::getPaddedNumber(ans3.size(), 3));
	message.append(ans3);

	message.append(Helper::getPaddedNumber(ans4.size(), 3));
	message.append(ans4);

	for (unsigned int i = 0; i < this->_users.size(); i++)
	{
		this->sendMessage(_users[i],"118001a001b001c001d001e"/*_users[i],message*/); //sends the first question and its answers to all the users in the game.
	}

	this->_curret_question_index--;
}

void Game::send_first_question()
{
	send_question_to_all_users();
}

/*

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
*/


void Game::sendMessage(User* user, string str){
	bool flag = false;
	for (unsigned int i = 0; i < this->_users.size() && !flag; i++)
	{
		if (_users[i] == user)
		{
			flag = true;
			_users[i]->send(str);
		}
	}

}

void Game::sendMessage(string str){
	sendMessage(NULL, str);
}