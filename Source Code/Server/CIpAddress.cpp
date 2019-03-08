#include "CIpAddress.h"

CIpAddress::CIpAddress(Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, NULL, port) < 0) {
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}


CIpAddress::CIpAddress(std::string host, Uint16 port) {
	if (SDLNet_ResolveHost(&m_Ip, host.c_str(), port) < 0) {
		std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
		m_Ip.host = 0;
		m_Ip.port = 0;
	}
}

CIpAddress::CIpAddress() {
	m_Ip.host = 0;
	m_Ip.port = 0;
}

bool CIpAddress::Ok() const {
	return !(m_Ip.port == 0);
}

void CIpAddress::SetIp(IPaddress sdl_ip) { //sets a CTcpSocket object from a existing SDL socket
	m_Ip = sdl_ip;
}

IPaddress CIpAddress::GetIpAddress() const {
	return m_Ip;
}

Uint32 CIpAddress::GetHost() const {
	return m_Ip.host;
}

Uint16 CIpAddress::GetPort() const {
	return m_Ip.port;
}