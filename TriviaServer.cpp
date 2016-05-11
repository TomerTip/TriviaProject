#include "TriviaServer.h"

/*Creates a socket for server listening, binds and listens*/
void TriviaServer::bindAndListen()
{
	WSADATA info;
	int err;
	err = WSAStartup(MAKEWORD(2, 2), &info);


	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Socket creation. 
	struct sockaddr_in serverService;
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = INADDR_ANY;
	serverService.sin_port = htons(8820);

	int connection;
	connection = ::bind(_socket, (struct sockaddr*) &serverService, sizeof(serverService)); //binds the socket with the propreties above. Ready to use.

	listen(_socket, 20); //supports 20 users.
	
	accept();
}

/*Accepting clients and opens a thread for each one of them.*/
void TriviaServer::accept()
{
	sockaddr_in client;
	SOCKET client_sock;
	int c = sizeof(struct sockaddr_in);

	client_sock = _WINSOCK2API_::accept(this->_socket, (struct sockaddr*)&client, &c); //create connections with the clients.
	if (client_sock != INVALID_SOCKET)
	{
		std::thread t(&TriviaServer::clientHandler, this, client_sock); //creates a thread - for handling client.
		t.detach(); //works independently.
	}
	else
	{
		cout << "ERROR: Invalid Socket." << endl;
	}
	
}

void TriviaServer::clientHandler(SOCKET s)
{
	char* buff;
	recv(s, buff, , 0);


}

void TriviaServer::safeDeleteUser(RecievedMessage* m)
{
	
}

/*
User* TriviaServer::handleSignin(RecievedMessage* m);
bool TriviaServer::handleSignup(RecievedMessage* m);
void TriviaServer::handleSignout(RecievedMessage* m);

void TriviaServer::handleLeaveGame(RecievedMessage* m);
void TriviaServer::handleStartGame(RecievedMessage* m);
void TriviaServer::handlePlayerAnswer(RecievedMessage* m);

bool TriviaServer::handleCreateRoom(RecievedMessage* m);
bool TriviaServer::handleCloseRoom(RecievedMessage* m);
bool TriviaServer::handleJoinRoom(RecievedMessage* m);
bool TriviaServer::handleLeaveRoom(RecievedMessage* m);
void TriviaServer::handleGetUsersInRoom(RecievedMessage* m);
void TriviaServer::handleGetRooms(RecievedMessage* m);

void TriviaServer::handleGetBestScores(RecievedMessage* m);
void TriviaServer::handleGetPersonalStatus(RecievedMessage* m);

void TriviaServer::handleRecievedMessages();
void TriviaServer::addRecievedMessage(RecievedMessage* m);
RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET s, int num);

User* TriviaServer::getUserByName();
User* TriviaServer::getUserBySocket(SOCKET s);
Room* TriviaServer::getRoomById(int id);
*/