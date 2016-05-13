#include <string>
#include <thread>
#include <fstream>
#include <iostream>
#include<vector>
#include <math.h>
#include <iomanip>
#include <sstream>
using namespace std;


class Question
{
public:
	Question(int,string,string,string,string,string);
	~Question();
	string get_question();
	string * get_answers();
	int get_correct_ans_index();
	int get_id();
private:
	string question;
	string answers[4];
	int correctansindex;
	int _id;
};

