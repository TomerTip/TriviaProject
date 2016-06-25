#include "Game.h"

Game::Game(vector<User*>& players, int questionNo, DataBase& db)
{
	this->_users = players;
	this->_question_num = questionNo;
	this->_db = db;

	try
	{
		this->_db.insert_new_game();
		this->_questions = this->_db.init_question(this->_question_num);
		
		for (int i = 0; i < this->_users.size(); i++)
		{
			this->_users[i]->set_game(this);
			this->_results[this->_users[i]->get_user_name()] = 0;
		}
			
	}
	catch (...)
	{
		cout << "ERORR: failed to open database." << endl;
	}
}

Game::~Game()
{
	this->_questions.clear();
	this->_users.clear();
}

void Game::send_question_to_all_users(){

	string message = RES_START_GAME;
	this->_current_turn_answers = 0;
	
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
		this->sendMessage(_users[i],message); //sends the first question and its answers to all the users in the game.
	}

	this->_curret_question_index++;
}

void Game::send_first_question()
{
	send_question_to_all_users();
}

void Game::handle_finish_game()
{
	//this->_db.updateGameStatus(int)

	string message = RES_GAME_SCORES;
	message.append(to_string(this->_users.size()));

	for (map<string, int>::iterator it = this->_results.begin(); it != this->_results.end(); it++)
	{
		message.append(Helper::getPaddedNumber(it->first.size(), 2));
		message.append(it->first);
		message.append(to_string(it->second));
	}

	for (int i = 0; i < this->_users.size(); i++)
	{
		try
		{
			this->_users[i]->send(message);
			this->_users[i]->set_game(nullptr);
		}
		catch (...)
		{
			cout << "ERROR: sending to user failed" << endl;
		}
	}
}

bool Game::handle_next_turn()
{
	if (!this->_users.empty())
	{
		if (this->_current_turn_answers == this->_users.size()) //if all the users answered the question:
		{
			if (this->_curret_question_index != this->_question_num) //if it wasn't the last question:
			{
				this->_curret_question_index++;
				this->send_question_to_all_users();
			}
		}

		return true;
	}
	else
	{
		this->handle_finish_game();
	}

	return false;
}

bool Game::handle_answer_from_user(User* user, int ansNo, int time)
{
	this->_current_turn_answers++;
	
	bool correct;
	string ans;
	int qIndex = this->_curret_question_index - 1; 
	int ansIndex = (ansNo < 5) ? ansNo-1 : ansNo; //if the user didn't answer in time, ansNo = 5 and, there is no answer.

	int correctIndex = this->_questions[qIndex]->get_correct_ans_index();

	if (ansIndex != 5)//if the user has answered in time:
	{
		ans = this->_questions[qIndex]->get_answers()[ansIndex];
		correct = (ansNo == correctIndex + 1);
	}
	else
		correct = true;
	

	if (correct) //if the user answered the correct answer:
	{
		this->_results[user->get_user_name()]++; //adds a point to the user;

		if (ansNo != 5) //if the user answered  in time:
		{
			this->_db.add_ans_to_player(this->_id, user->get_user_name(), qIndex, ans, correct, time);
			user->send(RES_ANS + '1'); //correct answer.
		}
		else
		{
			this->_db.add_ans_to_player(this->_id, user->get_user_name(), qIndex, "", correct, time); //user didn't answer in time.
			user->send(RES_ANS + '0');
		}
	}
	else
	{
		this->_db.add_ans_to_player(this->_id, user->get_user_name(), qIndex, ans, correct, time);
		user->send(RES_ANS + '0'); //wrong answer.
	}

	return this->handle_next_turn();
}

bool Game::leave_game(User* user)
{
	for (int i = 0; i < this->_users.size(); i++)
	{
		if (this->_users[i] == user)
		{
			this->_users.erase(this->_users.begin() + i); //erasing the user that quits.
		}
	}

	return this->handle_next_turn();
}

bool Game::insert_game_to_db()
{
	int res = this->_db.insert_new_game();

	if (res != -1)
	{
		this->_id = res;
		return true;
	}
	else
	{
		return false;
	}
}

int Game::getId()
{
	return this->_id;
}


void Game::init_questions_from_db()
{
	this->_questions = this->_db.init_question(this->_question_num);
}

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