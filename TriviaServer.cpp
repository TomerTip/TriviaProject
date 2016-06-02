#include "TriviaServer.h"
#include "Validator.h"
#include "Protocol.h"
using namespace std;

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


	//starts a thread that handles the messages from all of the clients.
	std::thread t(&TriviaServer::handleRecievedMessages, this); 
	t.detach(); 
	

	while (true)
	{
		accept();
	}
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
		while (((code = Helper::getMessageTypeCode(s)) != END))
		{
			cout << "message code: " << code << endl;
			rm = buildRecieveMessage(s, code); //inserts the new message to the queue.
			addRecievedMessage(rm);
		}

		rm = buildRecieveMessage(s, END); //creates end connection message, ends thread.
		addRecievedMessage(rm);
	}
	
	catch (...)
	{
		rm = buildRecieveMessage(s, END); 
		addRecievedMessage(rm);
	}
}


void TriviaServer::handleRecievedMessages()
{
	int code;
	unique_lock<mutex> ul(this->_mtxRecievedMessages, defer_lock);

	while (true)
	{
		ul.lock();

		//waits until the queue is not empty
		if (this->_queRcvMessages.empty())
			this->_cv.wait(ul);
		ul.unlock();

		//getting the first message in the queue, safely, with locks.
		RecievedMessage* rm = this->_queRcvMessages.front();
		this->_queRcvMessages.pop();

		rm->setUser(getUserBySocket(rm->getSock())); //init message's user.
		code = rm->getMessageCode();
		switch (code)
		{
		case SIGN_IN:
			handleSignin(rm);
			break;
		case SIGN_OUT:
			handleSignout(rm);
			break;
		case SIGN_UP:
			handleSignup(rm);
			break;
		case ROOMS_LIST:
			handleGetRooms(rm);
			break;
		case USERS_LIST:
			handleGetUsersInRoom(rm);
			break;
		case JOIN_ROOM:
			handleJoinRoom(rm);
			break;
		case LEAVE_ROOM:
			//handleLeaveRoom(rm);
			break;
		case CREATE_ROOM:
			handleCreateRoom(rm);
			break;
		case CLOSE_ROOM:
			handleCloseRoom(rm);
			break;
		case START_GAME:
			//handleStartGame(rm);
			break;
		case ANS:
			//handlePlayerAnswer(rm);
			break;
		case LEAVE_GAME:
			//handleLeaveGame(rm);
			break;
		case BEST_SCORES:
			//handleGetBestScores(rm);
			break;
		case MY_STATUS:
			//handleGetPersonalStatus(rm);
			break;
		case END:
			safeDeleteUser(rm);
			break;
		default:
			safeDeleteUser(rm);
			break;
		}
	}
}

void TriviaServer::addRecievedMessage(RecievedMessage* m)
{
	std::lock_guard<std::mutex> lock(this->_mtxRecievedMessages);
	this->_queRcvMessages.push(m);

	this->_cv.notify_all(); //notifies to all threads that the queue is not empty.
}

RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET s, int num)
{
	vector<string> values;
	string message;
	int size;

	if (num == SIGN_IN){
		size = Helper::getIntPartFromSocket(s, 2);
		string userName = Helper::getStringPartFromSocket(s, size);
		size = Helper::getIntPartFromSocket(s, 2);
		string pass = Helper::getStringPartFromSocket(s, size);
		values.push_back(userName);
		values.push_back(pass);
	}
	else if (num == SIGN_UP)
	{
		size = Helper::getIntPartFromSocket(s, 2);
		string userName = Helper::getStringPartFromSocket(s, size);
		size = Helper::getIntPartFromSocket(s, 2);
		string pass = Helper::getStringPartFromSocket(s, size);
		size = Helper::getIntPartFromSocket(s, 2);
		string email = Helper::getStringPartFromSocket(s, size);

		values.push_back(userName);
		values.push_back(pass);
		values.push_back(email);
	}
	else if (num == USERS_LIST || num == JOIN_ROOM)
	{
		size = Helper::getIntPartFromSocket(s, 4);
		string id = Helper::getStringPartFromSocket(s, size);
		values.push_back(id);
	}
	else if (num == CREATE_ROOM)
	{
		cout << "aasdf" << endl;
		size = Helper::getIntPartFromSocket(s, 2);
		string roomName = Helper::getStringPartFromSocket(s, size);

		string playerNum = Helper::getStringPartFromSocket(s, 1);
		string qNum = Helper::getStringPartFromSocket(s, 2);
		string qTime = Helper::getStringPartFromSocket(s, 2);

		values.push_back(roomName);
		values.push_back(playerNum);
		values.push_back(qNum);
		values.push_back(qTime);
	}

	//builds a receivedMessage.
	RecievedMessage* m = new RecievedMessage(s, num, values);

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
		User* u = this->_connectedUsers[s];
		return u;
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

void TriviaServer::safeDeleteUser(RecievedMessage* m)
{
	try
	{
		handleSignout(m);
		closesocket(m->getSock()); //closes the client's socket.
	}
	catch (...)
	{
		cout << "An ERROR has occured." << endl;
	}
}

User* TriviaServer::handleSignin(RecievedMessage* m)
{
	string username = m->getValues()[0];
	string password = m->getValues()[1];

	//Checks if the username and password are in the database:

	if (true/*this->_db.is_user_exits(username)*/)
	{
		if (true/*this->getUserByName(username)*/)
		{
			User* user = new User(username, m->getSock());
			this->_connectedUsers[m->getSock()] = user; //adds the user to the connected users.

			Helper::sendData(m->getSock(), RES_SIGN_IN_SUCCESS); //sends 1020 to client, "Success".
			return user;
		}
		else
		{
			Helper::sendData(m->getSock(), RES_SIGN_IN_ALREADY); //sends 1022 to client, "User is already connected".
			return nullptr;
		}
	}
	else
	{
		Helper::sendData(m->getSock(),RES_SIGN_IN_WRONG); //sends 1021 to client, "Wrong Details".
		return nullptr;
	}
	
}

bool TriviaServer::handleSignup(RecievedMessage* m)
{
	string username = m->getValues()[0];
	string password = m->getValues()[1];
	string email = m->getValues()[2];


	try
	{
		if (true/*Validator::is_pass_valid(password)*/)
		{
			if (true/*Validator::is_username_valid(username)*/)
			{
				if (true/*!this->_db.is_user_exits(username)*/)
				{
					//bool success = this->_db.add_new_user(username, password, email);
					if (true/*success*/)
					{
						Helper::sendData(m->getSock(),RES_SIGN_UP_SUCCESS); //sends 1040 to client, "success".
						return true;
					}
					else
					{
						Helper::sendData(m->getSock(), RES_SIGN_UP_OTHER); //sends 1044 to client, "other".
						return false;
					}
				}
				else
				{
					Helper::sendData(m->getSock(), RES_SIGN_UP_USER_ILLEGAL); //sends 1043 to client, "Username is illegal".
					return false;
				}
			}
			else
			{
				Helper::sendData(m->getSock(), RES_SIGN_UP_USER_ALREADY); //sends 1042 to client, "Username alreay exists.".
				return false;
			}
		}
		else
		{
			Helper::sendData(m->getSock(), RES_SIGN_UP_PASS); //sends 1041 to client, "password is illegal".
			return false;
		}
	}
	
	catch (...)
	{
		Helper::sendData(m->getSock(), RES_SIGN_UP_OTHER); //sends 1044 to client, "other".
		return false;
	}			
}				

void TriviaServer::handleSignout(RecievedMessage* m)
{
	map<SOCKET, User*>::iterator it;
	for (it = this->_connectedUsers.begin(); it != this->_connectedUsers.end(); it++)
	{
		if (it->second == m->getUser()) {
			this->_connectedUsers.erase(it);
		}
	}

	handleCloseRoom(m);
	handleLeaveRoom(m);
	//handleLeaveGame(m);
}

/*
void TriviaServer::handleLeaveGame(RecievedMessage* m)
{

}
void TriviaServer::handleStartGame(RecievedMessage* m)
{

}
void TriviaServer::handlePlayerAnswer(RecievedMessage* m)
{

}
*/

bool TriviaServer::handleCreateRoom(RecievedMessage* m)
{
	int id = this->_roomIdSequence;
	string name = m->getValues()[0];
	int playersNum = stoi(m->getValues()[1]);
	int questionNum = stoi(m->getValues()[2]);
	int questionSec = stoi(m->getValues()[3]);

	if (m->getUser()) //if a user owns the room:
	{
		bool success = m->getUser()->create_room(this->_roomIdSequence, name, playersNum, questionNum, questionSec);
		if (success)
		{
			Room* room = m->getUser()->get_room();
			pair<int, Room*> p(id, room);
			this->_roomsList.insert(p); //adds the new room to the room list.

			this->_roomIdSequence++;
			return true;
		}
	}
	return false;
}

bool TriviaServer::handleCloseRoom(RecievedMessage* m)
{
	if (m->getUser()->get_room())
	{
		if (m->getUser()->close_room() != -1)
		{
			for (map<int, Room*>::iterator it = this->_roomsList.begin(); it != this->_roomsList.end(); it++)
			{
				if (it->second == m->getUser()->get_room()) 
				{
					this->_roomsList.erase(it);
				}
			}
			return true;
		}
	}

	return false;
}

bool TriviaServer::handleLeaveRoom(RecievedMessage* m)
{
	if (m->getUser())
	{
		if (m->getUser()->get_room())
		{
			m->getUser()->leave_room();
		}
	}

	return false;
}

bool TriviaServer::handleJoinRoom(RecievedMessage* m)
{
	int id = stoi(m->getValues()[0]);

	if (m->getUser())
	{
		if (this->getRoomById(id))
		{
			Room* room = this->_roomsList[id];
			if (m->getUser()->join_room(room)) //joins the room;
			{
				string message = RES_JOIN_ROOM + to_string(room->get_question_no()) + to_string(room->get_question_time());
				m->getUser()->send(message);
				return true;
			}
		}
		else
		{
			m->getUser()->send(RES_JOIN_ROOM_FAIL);
		}
	}

	return false;
}

void TriviaServer::handleGetUsersInRoom(RecievedMessage* m)
{
	int id = stoi(m->getValues()[0]);
	Room* r = getRoomById(id);

	if (r != nullptr)
	{
		string message = m->getUser()->get_room()->get_users_messages_list();
		m->getUser()->send(RES_USERS_LIST + to_string(message.size()) + message);
	}
	else
	{
		m->getUser()->send(RES_USERS_LIST_NOT_EXIST);
	}

}

void TriviaServer::handleGetRooms(RecievedMessage* m)
{
	vector<string> rooms;

	string numberOfRooms = to_string(this->_roomsList.size());
	

	for (map<int, Room*>::iterator it = this->_roomsList.begin(); it != this->_roomsList.end(); it++)
	{
		string id = to_string(it->second->get_id());
		string roomNameSize = to_string(it->second->get_name().size());
		string roomName = it->second->get_name();
		
		string s = id + roomNameSize + roomName;

		rooms.push_back(s);
	}

	string message = RES_ROOMS_LIST + numberOfRooms;
	for (int i = 0; i < rooms.size(); i++)
	{
		message += rooms[i];
	}

	m->getUser()->send(message); //sends the list of rooms to the user.
}

/*
void TriviaServer::handleGetBestScores(RecievedMessage* m)
{

}
void TriviaServer::handleGetPersonalStatus(RecievedMessage* m)
{

}
*/

TriviaServer::TriviaServer()
{

}

TriviaServer::~TriviaServer()
{

}

void TriviaServer::server()
{
	this->bindAndListen();
}
