//- Centhra Engine -
#include <CE/ConfigBase.h>
#include <CE/Socket.h>
#include <CE/Base.h>

#if CE_BASE_USEPOSIXSOCKET
	//- POSIX -
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

namespace ce
{
	Socket *Socket::create(Domain domain, Type type, Protocol protocol)
	{
		int pDomain = -1, pType = -1, pProtocol = -1;

		switch(domain)
		{
		case IP4:
			pDomain = PF_INET;
			break;
		case IP6:
			pDomain = PF_INET6;
			break;
		}

		switch(type)
		{
		case Raw:
			pType = SOCK_RAW;
			break;
		case Stream:
			pType = SOCK_STREAM;
			break;
		case Datagram:
			pType = SOCK_DGRAM;
			break;
		case SequencedPacket:
			pType = SOCK_SEQPACKET;
			break;
		}

		switch(protocol)
		{
		case TCP:
			pProtocol = IPPROTO_TCP;
			break;
		case UDP:
			pProtocol = IPPROTO_UDP;
		}

		if(pDomain == -1 || pType == -1 || pProtocol == -1)
		{
			error("[Error] Socket::create - Unrecognized socket configuration.\n");
			return 0;
		}

		int pSocket = socket(pDomain, pType, pProtocol);

		if(pSocket == -1)
		{
			close(pSocket);
			error("[Error] Socket::create - Failed to create Socket.\n");
		}

		Socket *sock = new Socket();
		sock->m_socket = pSocket;
		return sock;
	}

	Socket::Socket()
	{
		m_socket = 0;
	}
	Socket::~Socket()
	{
		close(m_socket);
	}
	Socket *Socket::Accept()
	{
		int ret = accept(m_socket, NULL, NULL);

		if(ret == -1)
		{
			error("[Error] Socket::Accept - Unable to accept incomming connection.\n");
			return 0;
		}

		Socket *client = new Socket();
		client->m_socket = ret;
		return client;
	}
	bool Socket::Bind(unsigned int port)
	{
		struct sockaddr_in stSockAddr;
		memset(&stSockAddr, 0, sizeof(stSockAddr));
		
		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_port = htons(port);
		stSockAddr.sin_addr.s_addr = INADDR_ANY;
		if(bind(m_socket,(struct sockaddr *)&stSockAddr, sizeof(stSockAddr)) == -1)
		{
			error("[Error] Socket::Bind - Unable to bind to port %d.\n", port);
			return false;
		}

		return true;
	}
	bool Socket::Connect(const char *address, unsigned int port)
	{
		struct sockaddr_in stSockAddr;
		memset(&stSockAddr, 0, sizeof(stSockAddr));

		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_port = htons(port);
		int ret = inet_pton(AF_INET, address, &stSockAddr.sin_addr);

		if(ret < 0)
		{
			error("[Error] Socket::Connect - \"%s\" is not a valid address family.\n", address);
			return false;
		}
		else if(ret == 0)
		{
			error("[Error] Socket::Connect - \"%s:%d\" is not a valid ip address.\n", address, port);
			return false;
		}

		if(connect(m_socket, (struct sockaddr *)&stSockAddr, sizeof(stSockAddr)) == -1)
		{
			error("[Error] Socket::Connect - Unable to connect to \"%s:%d\"\n", address, port);
			return false;
		}

		return true;
	}
	bool Socket::Listen(unsigned int backlog)
	{
		if(listen(m_socket, backlog) == -1)
		{
			error("[Error] Socket::Listen - Unable to listen.\n");
			return false;
		}

		return true;
	}
	void Socket::Read(char *buffer, unsigned int length)
	{
		read(m_socket, buffer, length);
	}
	bool Socket::Shutdown()
	{
		if(shutdown(m_socket, SHUT_RDWR) == -1)
		{
			error("[Error] Socket::Listen - Unable to shutdown socket.\n");
			return false;
		}

		return true;
	}
	void Socket::Write(char *buffer, unsigned int length)
	{
		send(m_socket, buffer, length, 0);
	}
}
