
//Client side:
#define SIGN_IN 200
#define SIGN_OUT 201
#define SIGN_UP 203
#define ROOMS_LIST 205
#define USERS_LIST 207
#define JOIN_ROOM 209
#define LEAVE_ROOM 211
#define CREATE_ROOM 213
#define CLOSE_ROOM 215
#define START_GAME 217
#define ANS 219
#define LEAVE_GAME 222
#define BEST_SCORES 223
#define MY_STATUS 225
#define END 299

//Server side:
#define RES_SIGN_OUT 102
#define RES_SIGN_UP 104
#define RES_ROOMS_LIST 106
#define RES_USERS_LIST 108
#define RES_JOIN_ROOM 110
#define RES_LEAVE_ROOM 112
#define RES_CREATE_ROOM 114
#define RES_CLOSE_ROOM 116
#define RES_START_GAME 118 //a question with 4 answers. 
#define RES_ANS 120
#define RES_GAME_SCORES 121 //The final score of the game, the number of correct answers.
#define RES_BEST_SCORES 124
#define RES_MY_STATUS 126
