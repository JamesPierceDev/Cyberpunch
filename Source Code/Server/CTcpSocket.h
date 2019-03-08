#pragma once
#include <iostream>
#include <SDL_net.h>

class CTcpSocket {
protected:
	TCPsocket m_Socket; //the TCP socket structure
	SDLNet_SocketSet set; //a set of sockets. Used here only to check existing packets
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void SetSocket(TCPsocket  the_sdl_socket); //set a CTcpSocket object from a existing SDL socket
	bool Ok() const; //indicate if theres is a TCPsocket associated to the instance
	bool Ready() const; //True if there are bytes ready to be read
	virtual void OnReady(); //pure virtual
};