#pragma once

#include "../Widap_includes/WidapTimer.h"
#include <SFML/Network.hpp>

class WidapUDP
{
public:
	WidapUDP(unsigned short newMyPort,
		unsigned short newPartnerPort,
		bool (*newProcessPacket)(char * data, int lng)=0,
		int newMaxPacketLng=256);
	~WidapUDP();
	
	void send(int packetLng, bool confirm=0); //send
	void receive(); //receive incoming packets
	void searchForPartner(); //send a search packet to broadcast and local host if not connected
	
	void findLocalIp();
	void findPublicIp(); //this function may take some time
	void setPartnerIp(const char *address);
	
	char * getDataPtr() {return data;}
	const char * getPublicIp();
	const char * getLocalIp();
	const char * getPartnerIp();
	bool getIfConnected() {return connected;}
	
	bool getIfPublicIpFound() {return publicIpFound;}
	bool getIfPublicIpValid() {return publicIpValid;}
	bool getIfLocalIpValid() {return localIpValid;}
	
	bool getIfPartnerIpSet() {return partnerIpSet;}
	bool getIfPartnerIpValid() {return partnerIpValid;}
	
	unsigned short getMyPort() {return myPort;}
	unsigned short getPartnerPort() {return partnerPort;}
	
	bool findPublicIpSoon; //if receive is called while this is true, it will call findPublicIp()
	
private:
	
	WidapTimer tmr;
	static constexpr double timeoutTime=1.0; //time (seconds) after which if the program has received no packets, it reports  no connection
	static const int headerLng=15; //the header I put on and take off
	char *internalData; //the data to send or receive
	char *data; //points to internalData+headerLng, the data without the internal header
	int lastSentConfirmNum, lastReceiveConfirmNum; //I can get away with a number rather then a list of confirmed packets because the next packet is not sent until the previous one is confirmed
	bool waitingForConfirm;
	static constexpr double checkForConfirmInterval=0.05; //amount of time to wait in between checking for confirmation
	static const int resendFrequency=10; //how many loops to go through in between resends
	char addressString[32]; //used to copy addresses to before returning them
	
	sf::IpAddress publicIp, localIp, partnerIp;
	bool connected;
	
	bool publicIpFound, publicIpValid, localIpValid;
	bool partnerIpValid, partnerIpSet;
	
	unsigned short myPort; //ports 49152-65535 can be safely used in custom programs
	unsigned short partnerPort; //ports 49152-65535 can be safely used in custom programs
	
	sf::UdpSocket socket;
	
	int maxPacketLng;
	
	bool (*processPacket)(char * data, int lng); //returns 1 if it is a valid packet
	
	bool checkPacket(int *dataLng); //check to make sure the packet is good
};

