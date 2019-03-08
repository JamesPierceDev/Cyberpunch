#pragma once
#include<string>
#include <iostream>
#include <SDL_net.h>

class CIpAddress {
private:
	IPaddress m_Ip; //the IPaddress structure
public:
	CIpAddress(); //constructor
	CIpAddress(Uint16 port); //create and associate a port to the instance
	CIpAddress(std::string host, Uint16 port); //create and associate a port and a host to the instance
	void SetIp(IPaddress sdl_ip); //set a CIpAddress object from an existing SDL IPaddress
	bool Ok() const; //True if the object have a port and a host associated to it
	IPaddress GetIpAddress() const; //return a SDL_net IPaddress structure
	Uint32 GetHost() const; //return the host
	Uint16 GetPort() const; //return the port
};