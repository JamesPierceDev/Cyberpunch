#include "CHostSocket.h"
CHostSocket::CHostSocket(CIpAddress& the_ip_address) {
	CTcpSocket::CTcpSocket();
	IPaddress iph = the_ip_address.GetIpAddress();
	if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
		SDLNet_FreeSocketSet(set);
		std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
	}
}

CHostSocket::CHostSocket(Uint16 port) {
	CIpAddress iplistener(port);
	if (!iplistener.Ok()) {
		m_Socket = NULL;
	}
	else {
		CTcpSocket::CTcpSocket();
		IPaddress iph = iplistener.GetIpAddress();
		if (!(m_Socket = SDLNet_TCP_Open(&iph))) {
			SDLNet_FreeSocketSet(set);
			std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
		}
	}
}


bool CHostSocket::Accept(CClientSocket& the_client_socket) {
	TCPsocket cs;
	if ((cs = SDLNet_TCP_Accept(m_Socket))) {
		the_client_socket.SetSocket(cs);
		return true;
	}
	else
		return false;
}

void CHostSocket::OnReady() {

}
