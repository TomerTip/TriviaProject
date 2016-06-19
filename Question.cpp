#include "Question.h"
#include <time.h>

Question::Question(int id, string question, string right_ans, string ans2, string ans3, string ans4)
{
	_id = id;
	srand(time(NULL));
	this->question = question;

	bool flag;
	int j;
	for (int i = 0; i < 4; i++){
		answers[i] = "0";
	}
	flag = true;
	while (flag){
		j = rand() % 4;
		if (answers[j] == "0"){
			flag = false;
			answers[j] = right_ans;
		}
	}
	flag = true;
	while (flag){
		j = rand() % 4;
		if (answers[j] == "0"){
			flag = false;
			answers[j] = ans2;
		}
	}
	flag = true;
	while (flag){
		j = rand() % 4;
		if (answers[j] == "0"){
			flag = false;
			answers[j] = ans3;
		}
	}
	flag = true;
	while (flag){
		j = rand() % 4;
		if (answers[j] == "0"){
			flag = false;
			answers[j] = ans4;
		}
	}
}

Question::~Question()
{
}

string Question::get_question(){
	return question;
}

string * Question::get_answers(){
	return answers;
}

int Question::get_correct_ans_index(){
	return correctansindex;
}

int Question::get_id(){
	return _id;
}