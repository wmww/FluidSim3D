#include "WidapUDP.h"
#include <iostream>
#include <string.h>

WidapUDP::WidapUDP(
	unsigned short newMyPort,
	unsigned short newPartnerPort,
	bool (*newProcessPacket)(char * data, int lng),
	int newMaxPacketLng)
{
	myPort=newMyPort;
	partnerPort=newPartnerPort;
	
	processPacket=newProcessPacket;
	
	maxPacketLng=newMaxPacketLng;
	
	internalData=new char[maxPacketLng+headerLng];
	data=internalData+headerLng;
	
	findPublicIpSoon=0;
	connected=0;
	publicIpFound=0;
	publicIpValid=0;
	partnerIpSet=0;
	partnerIpValid=0;
	
	lastSentConfirmNum=0;
	lastReceiveConfirmNum=0;
	waitingForConfirm=0;
	
	socket.bind(myPort);
	socket.setBlocking(false);
	
	findLocalIp();
}

WidapUDP::~WidapUDP()
{
	delete[] internalData;
	socket.unbind();
}

void WidapUDP::findLocalIp()
{
	std::cout << "finding local IP address" << std::endl;
	localIp=sf::IpAddress::getLocalAddress();
	if (localIp==sf::IpAddress::None)
	{
		localIpValid=0;
		std::cout << "not connected to network" << std::endl;
	}
	else
	{
		localIpValid=1;
		std::cout << "connected to network, local IP is " << localIp.toString() << std::endl;
	}
}

void WidapUDP::findPublicIp()
{
	std::cout << "finding public IP address (can take a few seconds)..." << std::endl;
	publicIp=sf::IpAddress::getPublicAddress(sf::seconds(6));
	if (publicIp==sf::IpAddress::None)
	{
		publicIpValid=0;
		std::cout << "not connected to the internet" << std::endl;
	}
	else
	{
		publicIpValid=1;
		std::cout << "connected to internet, public IP is " << publicIp.toString() << std::endl;
	}
	
	publicIpFound=1;
}

const char * WidapUDP::getPublicIp()
{
	if (!publicIpFound)
		return "Unknown";
	else if (!publicIpValid)
		return "None";
	else
	{	
		strcpy(addressString, publicIp.toString().data());
		return addressString;
	}
}

const char * WidapUDP::getLocalIp()
{
	if (!localIpValid)
		return "None";
	else
	{	
		strcpy(addressString, localIp.toString().data());
		return addressString;
	}
}

const char * WidapUDP::getPartnerIp()
{
	if (!partnerIpValid)
		return "None";
	else
	{	
		strcpy(addressString, partnerIp.toString().data());
		return addressString;
	}
}

void WidapUDP::setPartnerIp(const char *address)
{
	partnerIp=sf::IpAddress(address);
	
	partnerIpSet=1;
	
	if (partnerIp==sf::IpAddress::None)
	{
		partnerIpValid=0;
		std::cout << "partner IP set to invalid address of " << address << "\n";
	}
	else
	{
		partnerIpValid=1;
		std::cout << "partner IP set to " << partnerIp.toString() << "\n";
	}
}

void WidapUDP::send(int packetLng, bool confirm)
{
	static const char packetHeader[]="Widap_P_______";
	char *dataCopy;
	int i;
	
	if (partnerIpValid)
	{
		for (i=0; i<headerLng; ++i)
		{
			internalData[i]=packetHeader[i];
		}
		
		data[packetLng]=0;
		packetLng++;
		
		if (confirm)
		{
			++lastSentConfirmNum;
			
			internalData[6]='C';
			
			internalData[ 8]=(lastSentConfirmNum/100000)%10+'0';
			internalData[ 9]=(lastSentConfirmNum/10000)%10+'0';
			internalData[10]=(lastSentConfirmNum/1000)%10+'0';
			internalData[11]=(lastSentConfirmNum/100)%10+'0';
			internalData[12]=(lastSentConfirmNum/10)%10+'0';
			internalData[13]=(lastSentConfirmNum/1)%10+'0';
			internalData[14]='_';
		}
		
		socket.send(internalData, packetLng+headerLng, partnerIp, partnerPort);
		
		if (confirm)
		{
			std::cout << "waiting for confirmation on message\n";
			
			dataCopy=new char[packetLng+headerLng];
			
			for (i=0; i<packetLng+headerLng; ++i)
			{
				dataCopy[i]=data[i];
			}
			
			waitingForConfirm=1;
			
			int counter=0;
			
			while (connected && waitingForConfirm)
			{
				receive();
				tmr.waitFor(checkForConfirmInterval, 0);
				
				if (!((++counter)%resendFrequency))
					socket.send(dataCopy, packetLng+headerLng, partnerIp, partnerPort);
			}
			
			delete[] dataCopy;
			
			if (connected)
				std::cout << "message confirmed received\n";
		}
	}
}

void WidapUDP::receive()
{
	short unsigned int port0;
	std::size_t dataLngSize;
	int dataLng;
	sf::IpAddress newPartnerIp;
	
	if (findPublicIpSoon)
	{
		findPublicIp();
		findPublicIpSoon=0;
	}
	
	if (connected && tmr.checkTime()>timeoutTime)
	{
		connected=0;
		std::cout << "network connection lost" << std::endl;
	}
	
	while (socket.receive(internalData, maxPacketLng+headerLng, dataLngSize, newPartnerIp, port0)!=sf::Socket::NotReady)
	{
		if (port0==partnerPort)
		{
			dataLng=dataLngSize;
			
			if (!checkPacket(&dataLng))
			{
				if (dataLng>headerLng+1 && processPacket && processPacket(internalData+headerLng, dataLng-headerLng))
				{
					std::cout << "received invalid message over network\n";
					std::cout << "message contents: " << data << "\n";
				}
				else
				{
					tmr.update(1);
					
					if (!connected)
					{
						std::cout << "connected to partner\n";
						connected=1;
					}
					
					if (!partnerIpValid || newPartnerIp!=partnerIp)
					{
						partnerIp=newPartnerIp;
						partnerIpValid=1;
						std::cout << "received valid message from " << partnerIp.toString() << ", it has been set as the partner IP\n";
					}
				}
			}
			else
			{
				std::cout << "received bad packet from " << newPartnerIp.toString() << "\n";
				std::cout << "message contents: " << internalData << "\n";
			}
		}
		else
			std::cout << "ignoring packet sent from port " << port0 << "instead of the designated partner port, " << partnerPort << "\n";
	}
}

void WidapUDP::searchForPartner()
{
	const char searchData[]="Widap_S________\0";
	
	std::cout << "broadcasting packet to search for partner\n";
	
	if (connected)
	{
		std::cout << "called searchForPartner() while already connected\n";
		return;
	}
	
	socket.send(searchData, headerLng+1, sf::IpAddress::Broadcast, partnerPort);
	socket.send(searchData, headerLng+1, sf::IpAddress::LocalHost, partnerPort);
}

bool WidapUDP::checkPacket(int *dataLng)
{
	int i;
	static const char headerStart[]="Widap_";
	int confirmNum=0;
	
	if (internalData[*dataLng-1]!=0)
	{
		std::cout << "UDP packet invalid because it does not end with NULL\n";
		std::cout << "packets alleged length is " << *dataLng << "\n";
		return 1;
	}
	
	for (i=0; i<6; ++i)
	{
		if (internalData[i]!=headerStart[i])
		{
			std::cout << "UDP packet invalid because it does not have the right header\n";
			return 1;
		}	
	}
	
	if (internalData[i]=='P') //a normal packet
		return 0;
	else if (internalData[i]=='S') //a search packet, no response is necessary, will automatically save address and reply
		return 0;
	else if (internalData[i]=='C')
	{
		confirmNum+=(internalData[ 8]-'0')*100000;
		confirmNum+=(internalData[ 9]-'0')*10000;
		confirmNum+=(internalData[10]-'0')*1000;
		confirmNum+=(internalData[11]-'0')*100;
		confirmNum+=(internalData[12]-'0')*10;
		confirmNum+=(internalData[13]-'0')*1;
		
		if (confirmNum<lastReceiveConfirmNum)
			return 0; //it is old, the client has moved on and so no action is required
		else
		{
			if (confirmNum==lastReceiveConfirmNum)
				*dataLng=headerLng; //this will stop the packet from being passed on more then once
			
			lastReceiveConfirmNum=confirmNum;
			for (i=0; i<6; ++i)
			{
				internalData[i]=headerStart[i];
			}
			internalData[i]='R';
			internalData[i+1]='_';
			internalData[ 8]=(lastReceiveConfirmNum/100000)%10+'0';
			internalData[ 9]=(lastReceiveConfirmNum/10000)%10+'0';
			internalData[10]=(lastReceiveConfirmNum/1000)%10+'0';
			internalData[11]=(lastReceiveConfirmNum/100)%10+'0';
			internalData[12]=(lastReceiveConfirmNum/10)%10+'0';
			internalData[13]=(lastReceiveConfirmNum/1)%10+'0';
			internalData[14]=0;
			socket.send(internalData, headerLng, partnerIp, partnerPort);
			
			return 0;
		}
	}
	else if (internalData[i]=='R')
	{
		if (waitingForConfirm)
		{
			int confirmNum=0;
			
			confirmNum+=(internalData[ 8]-'0')*100000;
			confirmNum+=(internalData[ 9]-'0')*10000;
			confirmNum+=(internalData[10]-'0')*1000;
			confirmNum+=(internalData[11]-'0')*100;
			confirmNum+=(internalData[12]-'0')*10;
			confirmNum+=(internalData[13]-'0')*1;
			
			if (confirmNum==lastSentConfirmNum)
				waitingForConfirm=0;
		}
		
		return 0;
	}
	else
	{
		std::cout << "UDP packet invalid because it is of unknown type\n";
		return 1;
	}
}

