#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

#include <WS2tcpip.h>
#include<WinSock2.h>
#include<Windows.h>

#include <queue>
#include <map>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "Helper.h"
#include "RecievedMessage.h"
#include "Protocol.h"
#include "DataBase.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define MAX_LENGTH 1024

/***********/
class TriviaServer
{
public:

	TriviaServer();
	~TriviaServer();

	void server();

private:

	void bindAndListen();
	void accept();
	void clientHandler(SOCKET s);
	void safeDeleteUser(RecievedMessage* m);

	User* handleSignin(RecievedMessage* m);
	bool handleSignup(RecievedMessage* m);
	void handleSignout(RecievedMessage* m);

	//void handleLeaveGame(RecievedMessage* m);
	//void handleStartGame(RecievedMessage* m);
	//void handlePlayerAnswer(RecievedMessage* m);

	bool handleCreateRoom(RecievedMessage* m);
	bool handleCloseRoom(RecievedMessage* m);
	bool handleJoinRoom(RecievedMessage* m);
	bool handleLeaveRoom(RecievedMessage* m);
	void handleGetUsersInRoom(RecievedMessage* m);
	void handleGetRooms(RecievedMessage* m);

	//void handleGetBestScores(RecievedMessage* m);
	//void handleGetPersonalStatus(RecievedMessage* m);

	void handleRecievedMessages();
	void addRecievedMessage(RecievedMessage* m);
	RecievedMessage* buildRecieveMessage(SOCKET s, int num);

	User* getUserByName(string name);
	User* getUserBySocket(SOCKET s);
	Room* getRoomById(int id);


	/********Local Variables**********/
	SOCKET _socket;
	map<SOCKET, User*> _connectedUsers;
	map<int, Room*> _roomsList;
	DataBase _db;

	queue<RecievedMessage*> _queRcvMessages;
	mutex _mtxRecievedMessages;
	condition_variable _cv;

	 int _roomIdSequence;

};