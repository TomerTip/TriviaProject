#include "game.h"
#include "User.h"
#include "DataBase.h"

Game::Game(const vector<User*>&, int, DataBase&){

}

void Game::send_first_question(){
	send_question_to_all_users();
}
void handle_finish_game();
bool handle_next_turn();
bool hanlde_answer_from_user(User*, int, int);
bool leave_game(User*);
bool insert_game_to_db();
void init_questions_from_db();
void Game::send_question_to_all_users(){
	_current_turn_answeres = 0;
	string str;
	vector<Question*>::iterator it;
	str.append("118");
	if (_questions.size() > 0){
		*it = _questions.back();
		(*it)->get_question();
	}
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