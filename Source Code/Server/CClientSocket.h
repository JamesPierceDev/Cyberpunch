#pragma once
#include <SDL_net.h>
#include "CNetMessage.h"
#include "CIpAddress.h"
#include "CTcpSocket.h"

class CHostSocket;

class CClientSocket : public CTcpSocket {
private:
	CIpAddress m_RemoteIp; //the CIpAddress object corresponding to the remote host
public:
	CClientSocket(); //constructor
	CClientSocket(std::string host, Uint16 port); //Create the object and connect to a host, in a given port
	bool Connect(CIpAddress& remoteip); //make a connection to communicate with a remote host
	bool Connect(CHostSocket& the_listener_socket); //make a connection to communicate with a client
	void SetSocket(TCPsocket  the_sdl_socket); //set a CTcpSocket object from an existing SDL_net socket
	CIpAddress getIpAddress() const; //return a CIpAddress object associated to the remote host
	virtual void OnReady(); //pure virtual
	bool Receive(CNetMessage& rData); //receive data and load it into a CNetMessage object
	bool Send(CNetMessage& sData); //send data from a CNetMessage object
};