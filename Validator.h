#include <string>

using namespace std;

class Validator{
public:
	static bool is_username_valid(string str){
		if (str.length() == 0){
			return false;
		}
		for (int i = 0; i < str.length(); i++){
			if (str[i] == ' ')
			{
				return false;
			}
		}
		if (str[0] <= 'Z' && str[0] >= 'A' && str[0] >= 'a' && str[0] <= 'Z'){
			return true;
		}
	}
	static bool is_pass_valid(string str){
		bool flag1 = false;
		bool flag2 = false;
		bool flag3 = false;

		if (str.length() < 0){
			return false;
		}
		for (int i = 0; i < str.length(); i++){
			if (str[i] == ' ')
			{
				return false;
			}
			if (str[i] <= '9' && str[i] >= '0'){
				flag1 = true;
			}
			if (str[i] <= 'z' && str[i] >= 'a'){
				flag2 = true;
			}
			if (str[i] <= 'z' && str[i] >= 'A'){
				flag3 = true;
			}
		}
		if (flag1 && flag2 && flag3){
			return true;
		}
		else{
			return false;
		}
	}
};