#include "OnlineSystem.h"

void OnlineSystem::addComponent(Component * component)
{
	m_components.push_back(component);
}

void OnlineSystem::addSendingPlayer(OnlineSendComponent * component)
{
	m_sendingPlayers.push_back(component);
}

void OnlineSystem::addReceivingPlayer(OnlineInputComponent * component)
{
	m_receivingPlayers.push_back(component);
}

void OnlineSystem::update(double dt)
{
	if (isConnected)
	{
		bool s = false;
		if (tts > syncRate)
		{
			//s = true;
			tts = 0;
		}
		SendCommands(s);
		ReceiveCommands();
		tts+=dt;
	}
}

void OnlineSystem::SendCommands(bool sync)
{
	for (auto& plyr : m_sendingPlayers)
	{
		auto cmds = plyr->Send();
		if (cmds->size() == 1 || sync)
		{
			bool isFirst = true;
			string jsonString = "{ \"type\" : \"COMMANDS\", \"player\" : "+ 
				std::to_string(plyr->m_playerNumber) +
				",\"list\":[";
			while (cmds->size() > 0)
			{
				if (!isFirst) { jsonString += ","; }
				else { isFirst = false; }

				jsonString += "\"" + cmds->front() + "\"";

				cmds->pop();
			}
			jsonString += " ], \"sync\": ";
			if (true){
				jsonString += "true, ";
				OnlineSendComponent::syncStruct info =  plyr->getSync();
				jsonString += "\"pos\":[" + toString(floor(info.pos.x)) + "," + toString(floor(info.pos.y)) + "],";
				jsonString += "\"vel\":[" + toString(floor(info.vel.x)) + "," + toString(floor(info.vel.y)) + "],";
				jsonString += "\"dvel\":[" + toString(floor(info.dvel.x)) + "," + toString(floor(info.dvel.y)) + "]";
			}
			else {
				jsonString += "false";
			}
			jsonString += "}";
			m_Socket->sendString(jsonString);
		}
		else if(cmds->size() > 1){
			cout << "pop queue" << endl;
			cmds->pop();
		}
	}
}

void OnlineSystem::ReceiveCommands()
{
	// Check if we've received a message
	string receivedMessage = m_Socket->checkForIncomingMessages();

	 //If so then...
	if (receivedMessage == "Lost connection to the server!")
	{
		delete m_Socket;
		isConnected = false;
		m_isHost = false;
	}
	else if (receivedMessage != "")
	{
		json currentPacket = json::parse(receivedMessage);
		if (currentPacket["type"] == "START")
		{
			gameStarted = true;
		}
		else if (currentPacket["type"] == "PICKUP")
		{
			p_spawnPickup = currentPacket["pos"];
		}
		else if (currentPacket["type"] == "QUIT")
		{
			delete m_Socket;
			isConnected = false;
			m_isHost = false;
		}
		else if (currentPacket["type"] == "COMMANDS")
		{
			for (auto& plyr : m_receivingPlayers)
			{
				if (currentPacket["player"] == plyr->m_playerNumber)
				{
					vector<string> commands = currentPacket["list"];
					for (auto iter = commands.begin(); iter != commands.end(); iter++)
					{
						plyr->addCommand(*iter);
					}
					if (currentPacket["sync"])
					{
						plyr->addPositions(currentPacket["pos"][0], currentPacket["pos"][1], currentPacket["vel"][0], currentPacket["vel"][1], currentPacket["dvel"][0], currentPacket["dvel"][1]);
						//plyr->syncPosition();
					}
				}
			}
		}
	}
}


bool OnlineSystem::ConnectToServer()
{
	try
	{
		// Now try to instantiate the client socket
		// Parameters: server address, port number, buffer size (i.e. max message size)
		// Note: You can provide the serverURL as a dot-quad ("1.2.3.4") or a hostname ("server.foo.com")
		m_Socket = new ClientSocket("149.153.106.152", 1234, 512);

		m_Socket->connectToServer();
		isConnected = true;
		return true;
	}
	catch (SocketException e)
	{
		std::cerr << "Something went wrong creating a ClientSocket object." << std::endl;
		std::cerr << "Error is: " << e.what() << std::endl;
		std::cerr << "Terminating application." << std::endl;
		//exit(-1);
		isConnected = false;
		return false;
	}
}

string OnlineSystem::CheckMessages()
{
	string ret = m_Socket->checkForIncomingMessages();
	if(ret != "Lost connection to the server!")
		return ret;
	else {
		delete m_Socket;
		isConnected = false;
	}
}

vector<OnlineSystem::LobbyInfo> OnlineSystem::getLobbies()
{
	string jsonString = "{ \"type\" : \"LOBBY REQUEST\"}";

	m_Socket->sendString(jsonString);
	string receivedMessage;
	do {
		receivedMessage = m_Socket->checkForIncomingMessages();
	} while (receivedMessage == "");
	

	json lobbies = json::parse(receivedMessage);
	vector<OnlineSystem::LobbyInfo> retval;
	if (lobbies["type"] == "LOBBY LIST")
	{
		vector<vector<int>> lobbyList = lobbies["list"];
		for (auto info : lobbyList)
		{
			LobbyInfo l;
			l.name = std::to_string(info[0]);
			l.players = std::to_string(info[1]);
			retval.push_back(l);
		}
	}

	return retval;
}

void OnlineSystem::makeHost()
{
	if (!m_isHost)
	{
		string jsonString = "{ \"type\" : \"HOST\"}";

		m_Socket->sendString(jsonString);
		string receivedMessage;
		do {
			receivedMessage = m_Socket->checkForIncomingMessages();
		} while (receivedMessage == "");

		json lobby = json::parse(receivedMessage);
		if (lobby["type"] == "HOSTED")
		{
			m_lobbyNumber = lobby["lobby"];
			m_isHost = true;
			m_playerNumber = 0;
		}
	}
}

bool OnlineSystem::joinLobby(int lob)
{
	if (!m_isHost)
	{
		string jsonString = "{ \"type\" : \"JOIN\", \"lobby\":" + toString(lob) + "}";

		m_Socket->sendString(jsonString);
		string receivedMessage;
		do {
			receivedMessage = m_Socket->checkForIncomingMessages();
		} while (receivedMessage == "");

		json lobby = json::parse(receivedMessage);
		if (lobby["type"] == "JOINED")
		{
			m_lobbyNumber = lob;
			m_playerNumber = lobby["player"];
			//go to pregame screen
			return true;
		}
		else
		{
			cout << "join failed" << endl;
			return false;
		}
	}

}

vector<int> OnlineSystem::getPlayers()
{
	vector<int> retval;
	string jsonString = "{\"type\" : \"PLYRS\"}";
	m_Socket->sendString(jsonString);
	string receivedMessage;
	do {
		receivedMessage = m_Socket->checkForIncomingMessages();
	} while (receivedMessage == "");

	json players = json::parse(receivedMessage);

	if (players["type"] == "NMBRS")
	{
		vector<int> retvaL = players["list"];
		retval = retvaL;
	}


	return retval;
}

void OnlineSystem::assignPlayerSlots(vector<bool> slotsTaken)
{
	string jsonString;
	jsonString = "{\"type\":\"ASSIGNSLOTS\",\"list\":[";
	bool first = true;
	for (auto plyr : slotsTaken)
	{
		if (!first)
		{
			jsonString += ",";
		}
		else
		{
			first = false;
		}
		if (plyr)
			jsonString += "true";
		else
			jsonString += "false";
	}
	jsonString += "]}";
	m_Socket->sendString(jsonString);
}

void OnlineSystem::startGame()
{
	string jsonString = "{\"type\" : \"START\"}";
	m_Socket->sendString(jsonString);
	gameStarted = true;
}

void OnlineSystem::spawnPickup(int spawnPosition)
{
	string jsonString = "{\"type\" : \"PICKUP\", \"pos\":"+toString(spawnPosition)+"}";
	m_Socket->sendString(jsonString);
}

int OnlineSystem::pickupLocation()
{
	int retval = p_spawnPickup;
	p_spawnPickup = -1;
	return retval;
}

void OnlineSystem::disconnect(vector<int> relatedPlyrs)
{
	string pack = "{\"type\":\"QUIT\",\"list\":[";
	bool isFirst = true;
	for (auto p : relatedPlyrs) {
		if (!isFirst)
			pack += ",";
		else
			isFirst = false;
		pack += toString(p);
	}
	pack += "]}";
	m_Socket->sendString(pack);
	delete m_Socket;
	isConnected = false;
	m_isHost = false;
	
}
