#pragma once
#include "CTcpSocket.h"
#include "CIpAddress.h"
#include "CClientSocket.h"


class CHostSocket : public CTcpSocket {
public:
	CHostSocket(CIpAddress& the_ip_address); //create and open a new socket, with an existing CIpAddress object

	CHostSocket(Uint16 port); //create and open a new socket with the desired port
	bool Accept(CClientSocket&); //set a client CTcpSocket object after listening to the port
	virtual void OnReady(); //pure virtual
};