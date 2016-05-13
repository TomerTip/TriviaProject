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
	cout << ">> Binded." << endl;

	listen(_socket, 20); //supports 20 users.
	cout << ">> Listening. (Port = 8820)" << endl;

	accept();
}

/*Accepting clients and opens a thread for each one of them.*/
void TriviaServer::accept()
{
	sockaddr_in client;
	SOCKET client_sock;
	int c = sizeof(struct sockaddr_in);

	client_sock = _WINSOCK2API_::accept(this->_socket, (struct sockaddr*)&client, &c); //create connections with the clients.
	cout << "Accepting Clients." << endl;

	if (client_sock != INVALID_SOCKET)
	{
		cout << endl << "Accepted client on port = " << ntohs(client.sin_port) << endl;
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

	int code;
	RecievedMessage* rm;

	try
	{
		while (((code = Helper::getMessageTypeCode(s)) != END) || ((code = Helper::getMessageTypeCode(s)) != 0))
		{
			rm = buildRecieveMessage(s, code); //inserts the new message to the queue.
		}

		rm = buildRecieveMessage(s, END); //creates end connection message, ends thread.
	}
	
	catch (...)
	{
		rm = buildRecieveMessage(s, END); //creates end connection message, ends thread.
	}

}

void TriviaServer::handleRecievedMessages()
{
	unique_lock<std::mutex> ul(this->_mtxRecievedMessages, defer_lock);

	if (!this->_queRcvMessages.empty())
	{

	}
	else
	{
		this->_cv.wait(ul);
	}


	this->_mtxRecievedMessages.lock();

	RecievedMessage* rm = this->_queRcvMessages.back();
	this->_queRcvMessages.pop();

	this->_mtxRecievedMessages.unlock();

	rm->getSock();




}

void TriviaServer::safeDeleteUser(RecievedMessage* m)
{

	map<SOCKET, User*>::iterator it;
	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
	{
		if (it->second == m->getUser()) {
			this->_connectedUsers.erase(it->first);
		}
	}

}

void TriviaServer::addRecievedMessage(RecievedMessage* m)
{
	this->_queRcvMessages.push(m);
}

RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET s, int num)
{
	//builds a receivedMessage.
	RecievedMessage* m = new RecievedMessage(s, num);
	addRecievedMessage(m);

	return m;
}

User* TriviaServer::getUserByName(string name)
{
	map<SOCKET, User*>::iterator it;
	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
	{
		if (it->second->get_user_name() == name) {
			return it->second;
		}
	}

	return nullptr;
}

User* TriviaServer::getUserBySocket(SOCKET s)
{
	try
	{
		return this->_connectedUsers.at(s);
	}
	
	catch (...)
	{
		return nullptr;
	}

}

Room* TriviaServer::getRoomById(int id)
{
	try
	{
		return this->_roomsList.at(id);
	}

	catch (...)
	{
		return nullptr;
	}
}


User* TriviaServer::handleSignin(RecievedMessage* m)
{
	m->getValues
}

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

*/