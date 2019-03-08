#include "CClientSocket.h"
CClientSocket::CClientSocket() {
	CTcpSocket::CTcpSocket();
}

CClientSocket::CClientSocket(std::string host, Uint16 port) {
	CIpAddress remoteip(host.c_str(), port);
	if (!remoteip.Ok()) {
		m_Socket = NULL;
	}
	else {
		CTcpSocket::CTcpSocket();
		Connect(remoteip);
	}
}

bool CClientSocket::Connect(CHostSocket& the_listener_socket) {
	return false;
}

bool CClientSocket::Connect(CIpAddress& remoteip) {
	TCPsocket cs;
	IPaddress ip = remoteip.GetIpAddress();
	if ((cs = SDLNet_TCP_Open(&ip)))
	{
		SetSocket(cs);
		return true;
	}
	else {
		std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
		return false;
	}
}

void CClientSocket::SetSocket(TCPsocket the_sdl_socket) {
	CTcpSocket::SetSocket(the_sdl_socket);
	IPaddress* ips;
	if ((ips = SDLNet_TCP_GetPeerAddress(m_Socket))) {
		/* Print the address, converting it onto the host format */
		m_RemoteIp.SetIp(*ips);
		Uint32 hbo = m_RemoteIp.GetHost();
		Uint16 pbo = m_RemoteIp.GetPort();
		std::cout << "Client connected: " << SDLNet_Read32(&hbo) << ' '
			<< SDLNet_Read16(&pbo) << std::endl;
	}
	else
		std::cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << std::endl;
}

CIpAddress CClientSocket::getIpAddress() const {
	return m_RemoteIp;
}

bool CClientSocket::Receive(CNetMessage& rData) {
	//Firstly, check if there is a socket
	if (m_Socket == NULL)
		return false;
	charbuf buf;

	//Check if the instance can receive bytes, if it can, load the number of bytes specified by NumToLoad() virtual function
	while (rData.NumToLoad() > 0) {
		if (SDLNet_TCP_Recv(m_Socket, buf, rData.NumToLoad()) > 0) {
			rData.LoadBytes(buf, rData.NumToLoad());
		}
		else {
			return false;
		}
	}
	rData.finish();
	return true;
}

bool CClientSocket::Send(CNetMessage& sData) {
	//check if there is a socket
	if (m_Socket == NULL)
		return false;
	charbuf buf;
	int len;

	//Check if the instance can send bytes, if it can, unload the number of bytes specified by NumToLoad() virtual function
	while ((len = sData.NumToUnLoad()) > 0) {
		sData.UnLoadBytes(buf);
		if (SDLNet_TCP_Send(m_Socket, (void *)buf, len) < len) {
			std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
			return false;
		}
	}
	return true;
}

void CClientSocket::OnReady() {

}