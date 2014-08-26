//- Standard Library -
#include <map>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Network.h>
#include <CE/App.h>
#include <CE/FloatTable.h>

#if CE_BASE_USEPOSIXSOCKET
	//- POSIX -
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <cstdio>
	#include <cstdlib>
	#include <cstring>
	#include <unistd.h>
	#include <fcntl.h>
#endif

#ifdef _WIN32
	#if CE_BASE_USEWINSOCKET
		//- WSA -
		#include <winsock2.h>
		#include <ws2tcpip.h>
	#endif
	#include <windows.h>
#endif

using namespace ce;
using namespace std;

#define CE_PACKET_BUFFER_SIZE 2048
#define CE_PACKET_LIBRARY_SIZE 256

namespace ce
{
	//TODO: Replace this with cleaner fix, source: http://stackoverflow.com/questions/15370033/how-to-use-inet-pton-with-the-mingw-compiler
#ifdef __MINGW32__
	#define NS_INADDRSZ  4
	#define NS_IN6ADDRSZ 16
	#define NS_INT16SZ   2
	#define inet_pton4 net_inet_pton4
	#define inet_pton6 net_inet_pton6
	#define inet_pton net_inet_pton
	int net_inet_pton4(const char *src, char *dst)
	{
		uint8_t tmp[NS_INADDRSZ], *tp;

		int saw_digit = 0;
		int octets = 0;
		*(tp = tmp) = 0;

		int ch;
		while((ch = *src++) != '\0')
		{
			if(ch >= '0' && ch <= '9')
			{
				uint32_t n = *tp * 10 + (ch - '0');

				if(saw_digit && *tp == 0)
					return 0;

				if(n > 255)
					return 0;

				*tp = n;
				if(!saw_digit)
				{
					if(++octets > 4)
						return 0;
					saw_digit = 1;
				}
			}
			else if(ch == '.' && saw_digit)
			{
				if(octets == 4)
					return 0;
				*++tp = 0;
				saw_digit = 0;
			}
			else
				return 0;
		}
		if(octets < 4)
			return 0;

		memcpy(dst, tmp, NS_INADDRSZ);

		return 1;
	}
	int net_inet_pton6(const char *src, char *dst)
	{
		static const char xdigits [] = "0123456789abcdef";
		uint8_t tmp[NS_IN6ADDRSZ];

		uint8_t *tp = (uint8_t*)memset(tmp, '\0', NS_IN6ADDRSZ);
		uint8_t *endp = tp + NS_IN6ADDRSZ;
		uint8_t *colonp = NULL;

		/* Leading :: requires some special handling. */
		if(*src == ':')
		{
			if(*++src != ':')
				return 0;
		}

		const char *curtok = src;
		int saw_xdigit = 0;
		uint32_t val = 0;
		int ch;
		while((ch = tolower(*src++)) != '\0')
		{
			const char *pch = strchr(xdigits, ch);
			if(pch != NULL)
			{
				val <<= 4;
				val |= (pch - xdigits);
				if(val > 0xffff)
					return 0;
				saw_xdigit = 1;
				continue;
			}
			if(ch == ':')
			{
				curtok = src;
				if(!saw_xdigit)
				{
					if(colonp)
						return 0;
					colonp = tp;
					continue;
				}
				else if(*src == '\0')
				{
					return 0;
				}
				if(tp + NS_INT16SZ > endp)
					return 0;
				*tp++ = (uint8_t)(val >> 8) & 0xff;
				*tp++ = (uint8_t)val & 0xff;
				saw_xdigit = 0;
				val = 0;
				continue;
			}
			if(ch == '.' && ((tp + NS_INADDRSZ) <= endp) &&
				inet_pton4(curtok, (char*)tp) > 0)
			{
				tp += NS_INADDRSZ;
				saw_xdigit = 0;
				break; /* '\0' was seen by inet_pton4(). */
			}
			return 0;
		}
		if(saw_xdigit)
		{
			if(tp + NS_INT16SZ > endp)
				return 0;
			*tp++ = (uint8_t)(val >> 8) & 0xff;
			*tp++ = (uint8_t)val & 0xff;
		}
		if(colonp != NULL)
		{
			/*
			* Since some memmove()'s erroneously fail to handle
			* overlapping regions, we'll do the shift by hand.
			*/
			const int n = tp - colonp;

			if(tp == endp)
				return 0;

			for(int i = 1; i <= n; i++)
			{
				endp[-i] = colonp[n - i];
				colonp[n - i] = 0;
			}
			tp = endp;
		}
		if(tp != endp)
			return 0;

		memcpy(dst, tmp, NS_IN6ADDRSZ);

		return 1;
	}

	int net_inet_pton(int af, const char *src, char *dst)
	{
		switch(af)
		{
		case AF_INET:
			return inet_pton4(src, dst);
		case AF_INET6:
			return inet_pton6(src, dst);
		default:
			return -1;
		}
	}
#endif

	unsigned long long g_clientPingTable[8];
	unsigned short g_clientPing = 0;
	unsigned char g_lastClientPingID = 0;

	/*	-------------------------- Packet Buffer --------------------------
	All packets use a shared buffer. Once a packet is queued, its
	contents are stored in the buffer while its location and size
	is stored in the library.
	*/
	PacketBuffer::PacketBuffer(unsigned short bufferSize, unsigned short librarySize) : m_bufferSize(bufferSize), m_librarySize(librarySize), m_nextIndex(0), m_edge(0)
	{
		m_buffer = new char[bufferSize];
		m_library = new pair<char *, unsigned char>[librarySize];
	}
	PacketBuffer::~PacketBuffer()
	{
		delete [] m_buffer;
		delete [] m_library;
	}
	char *PacketBuffer::Allocate(unsigned char size, unsigned short &index)
	{
		if((m_edge + size) > m_bufferSize)
			m_edge = 0;
		index = m_nextIndex;
		char *allocateAddress = &m_buffer[m_edge]; //TODO: Determine if this should be turned into a global to avoid memory allocation/dellocation
		m_edge += size;
		m_library[m_nextIndex].first = allocateAddress;
		m_library[m_nextIndex++].second = size;
		if(m_nextIndex == m_librarySize)
			m_nextIndex = 0;
		return allocateAddress;
	}
	char *PacketBuffer::GetAddress(unsigned short idx) const
	{
		return m_library[idx].first;
	}
	unsigned short PacketBuffer::GetSize(unsigned short idx) const
	{
		return m_library[idx].second;
	}

	/*	-------------------------- Packet Handlers --------------------------
	Packet handlers are given a generated ID based off of the order in
	initializePacketHandlers. The order must be the same for both client
	and server if they are going to communicate properly.
	*/
	vector<PacketHandler *> g_packetHandlerVec;
	void (*g_packetInitializer)(unsigned char, vector<PacketHandler *> *) = 0;
	void SetPacketInitializer(void (*initializer)(unsigned char, vector<PacketHandler *> *))
	{
		g_packetInitializer = initializer;
	}
	void initializePacketHandlers()
	{
		unsigned char id = 0;
		g_packetHandlerVec.reserve(16);
		g_packetHandlerVec.push_back(new PacketHandler_Ping(id++));
		g_packetHandlerVec.push_back(new PacketHandler_Pong(id++));
		g_packetHandlerVec.push_back(new PacketHandler_ServerInfo(id++));
		g_packetHandlerVec.push_back(new PacketHandler_ClientJoin(id++));
		g_packetHandlerVec.push_back(new PacketHandler_ClientDisconnect(id++));

		if(g_packetInitializer)
			(*g_packetInitializer)(id, &g_packetHandlerVec);
	}
	void cleanupPacketHandlers()
	{
		for(vector<PacketHandler *>::iterator it = g_packetHandlerVec.begin(); it != g_packetHandlerVec.end(); it++)
			delete *it;
		g_packetHandlerVec.clear();
	}

	void PacketHandler::ProcessPacket(char *start, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		unsigned char id = (unsigned char)*start;
		if(id < g_packetHandlerVec.size())
			g_packetHandlerVec[id]->OnProcess(start, buffer, socket, sockAddr, sockAddrLen, client);
	}

	PacketHandler::PacketHandler(unsigned char type) : m_type(type)
	{
	}
	void PacketHandler::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
	}

	unsigned char PacketHandler_ServerInfo::ms_type = 255;
	unsigned short PacketHandler_ServerInfo::Create(PacketBuffer *buffer)
	{
		unsigned short index;
		char *container = buffer->Allocate(1, index);
		container[0] = ms_type;
		return index;
	}
	PacketHandler_ServerInfo::PacketHandler_ServerInfo(unsigned char type) : PacketHandler(type)
	{
		if(ms_type == 255)
			ms_type = type;
	}
	void PacketHandler_ServerInfo::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		unsigned short packetIdx;
		packetIdx = PacketHandler_ClientJoin::Create(buffer);
		socket->SendTo(buffer->GetAddress(packetIdx), buffer->GetSize(packetIdx), sockAddr, sockAddrLen);
	}

	unsigned char PacketHandler_ClientJoin::ms_type = 255;
	unsigned short PacketHandler_ClientJoin::Create(PacketBuffer *buffer)
	{
		Network *net = Network::GetCurrent();
		unsigned short index, aliasSize = net->m_alias.size();;
		char *container = buffer->Allocate(3 + aliasSize, index);
		container[0] = ms_type;
		memcpy(&container[1], &aliasSize, 2);
		memcpy(&container[3], &net->m_alias.c_str()[0], aliasSize);
		return index;
	}
	PacketHandler_ClientJoin::PacketHandler_ClientJoin(unsigned char type) : PacketHandler(type)
	{
		if(ms_type == 255)
			ms_type = type;
	}
	void PacketHandler_ClientJoin::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		if(!client)
			return;
		unsigned short aliasSize = 0;
		memcpy(&aliasSize, &data[1], 2);
		string clientName(&data[3], aliasSize);

		Server::Client *cl = (Server::Client *)client;
		cl->m_alias = clientName;

		//TODO: Determine if the callback needs the socket/packet buffer information
		Network *net = Network::GetCurrent();
		net->OnClientConnected(cl, buffer);
	}

	unsigned char PacketHandler_ClientDisconnect::ms_type = 255;
	unsigned short PacketHandler_ClientDisconnect::Create(PacketBuffer *buffer)
	{
		unsigned short index;
		char *container = buffer->Allocate(1, index);
		container[0] = ms_type;
		return index;
	}
	PacketHandler_ClientDisconnect::PacketHandler_ClientDisconnect(unsigned char type) : PacketHandler(type)
	{
		if(ms_type == 255)
			ms_type = type;
	}
	void PacketHandler_ClientDisconnect::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		if(client)
			((Server::Client *)client)->Kill();
	}

	unsigned char PacketHandler_Ping::ms_type = 255;
	unsigned short PacketHandler_Ping::Create(PacketBuffer *buffer, unsigned short id)
	{
		unsigned short index;
		char *container = buffer->Allocate(3, index);
		container[0] = ms_type;
		memcpy(&container[1], &id, 2);
		return index;
	}
	PacketHandler_Ping::PacketHandler_Ping(unsigned char type) : PacketHandler(type)
	{
		if(ms_type == 255)
			ms_type = type;
	}
	void PacketHandler_Ping::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		unsigned short packetIdx, id;
		memcpy(&id, &data[1], 2);
		packetIdx = PacketHandler_Pong::Create(buffer, id);
		socket->SendTo(buffer->GetAddress(packetIdx), buffer->GetSize(packetIdx), sockAddr, sockAddrLen);
	}

	unsigned char PacketHandler_Pong::ms_type = 255;
	unsigned short PacketHandler_Pong::Create(PacketBuffer *buffer, unsigned short id)
	{
		unsigned short index;
		char *container = buffer->Allocate(3, index);
		container[0] = ms_type;
		memcpy(&container[1], &id, 2);
		return index;
	}
	PacketHandler_Pong::PacketHandler_Pong(unsigned char type) : PacketHandler(type)
	{
		if(ms_type == 255)
			ms_type = type;
	}
	void PacketHandler_Pong::OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client)
	{
		unsigned short id;
		memcpy(&id, &data[1], 2);
		if(id > 8)
			return;
		if(client)
		{
			Server::Client *cl = (Server::Client *)client;
			cl->m_ping = (unsigned short)(getRunTimeMS() - cl->m_pingTable[id]);
		}
		else
		{
			unsigned long long t = getRunTimeMS();
			g_clientPing = (unsigned short)(t - g_clientPingTable[id]);
//			print("Ping:\t%lld\t%lld\t%d\n", g_clientPingTable[id], t, g_clientPing);
		}
	}

	//-------------------------- Server --------------------------
	string g_clientIntro = "SMGC";
	map<string, Server::Client *> g_clientConnectionMap;
	PacketBuffer *Server::ms_packetBuffer = 0;

	void *serverClientConnectionFunc(void *arg)
	{
		unsigned long long lastProcess = getRunTimeMS();

		Server::Client *client = (Server::Client *)arg;
		App *app = App::GetCurrent();
		Network *net = Network::GetCurrent();

		Server::LockCommMutex();
		Server::ms_clients.push_back(client);
		Server::UnlockCommMutex();

		unsigned long long lastComm = getRunTimeMS(), t = 0;

		unsigned short packetIdx = PacketHandler_ServerInfo::Create(Server::ms_packetBuffer);
		Server::ms_socket->SendTo(Server::ms_packetBuffer->GetAddress(packetIdx), Server::ms_packetBuffer->GetSize(packetIdx), client->m_sockAddr, client->m_sockAddrLen);

		bool dead = false;
		while(app->IsRunning() && Server::ms_isRunning && client->IsAlive())
		{
			while(client->m_incomingPackets.size())
			{
				PacketHandler::ProcessPacket((char *)client->m_incomingPackets.front().c_str(), Server::ms_packetBuffer, Server::ms_socket, client->m_sockAddr, client->m_sockAddrLen, client);
				client->m_incomingPackets.pop_front();
			}

			sleepMS(16);
		}

		net->OnClientDisconnected(client);

		Server::LockCommMutex();
		g_clientConnectionMap.erase(client->m_id);
		Server::ms_clients.erase(find(Server::ms_clients.begin(), Server::ms_clients.end(), client));
		delete client;
		Server::UnlockCommMutex();

		Thread::Exit();
		return 0;
	}
	Server::Client::Client()
	{
		m_sockAddr = 0;
		m_sockAddrLen = 0;
		m_lastCommTime = 0;
		m_isAlive = true;
		for(unsigned char a = 0; a < 8; a++)
			m_pingTable[a] = 0;
		m_lastPingID = 0;
		m_ping = 0;
		m_userData = 0;

		m_connectionThread = new Thread(serverClientConnectionFunc);
		m_connectionThread->Start(this);
	}
	Server::Client::~Client()
	{
		if(m_sockAddr)
			delete (struct sockaddr_in *)m_sockAddr;
		delete m_userData;
	}
	bool Server::Client::IsAlive() const
	{
		return m_isAlive;
	}
	void Server::Client::Kill()
	{
		m_isAlive = false;
	}
	void Server::Client::SetUserData(void *userData)
	{
		m_userData = userData;
	}
	void *Server::Client::GetUserData() const
	{
		return m_userData;
	}

	void *serverProcessFunc(void *arg)
	{
		unsigned long long lastProcess = getRunTimeMS();

		App *app = App::GetCurrent();
		Network *net = Network::GetCurrent();

		char recvBuff[257];
		memset(recvBuff, 0, 257);
		int recvBuffLen = 0;
		struct sockaddr_in recvAddr;
		int recvAddrLen = sizeof(recvAddr);
		memset(&recvAddr, 0, recvAddrLen);
		string idStr, packetContainer;
		unsigned long long lastTimeoutCheck, lastUpdate, t;
		lastTimeoutCheck = lastUpdate = getRunTimeMS();

		while(app->IsRunning() && Server::ms_isRunning)
		{
			t = getRunTimeMS();

			recvBuffLen = Server::ms_socket->RecvFrom(recvBuff, 256, &recvAddr, &recvAddrLen);
			if(recvBuffLen > 0)
			{
				idStr.resize(recvAddrLen);
				memcpy(&idStr[0], &recvAddr, recvAddrLen);

				Server::LockCommMutex();
				Server::Client *client = 0;
				if(!g_clientConnectionMap.count(idStr))
				{
					if(g_clientIntro.compare(0, recvBuffLen, recvBuff))
					{
						memset(recvBuff, 0, recvBuffLen);
						Server::UnlockCommMutex();
						continue;
					}

					client = new Server::Client();
					client->m_id = idStr;
					client->m_sockAddr = new struct sockaddr_in;
					memcpy(client->m_sockAddr, &recvAddr, sizeof(recvAddr));
					client->m_sockAddrLen = recvAddrLen;
					g_clientConnectionMap[idStr] = client;
				}
				else
				{
					client = g_clientConnectionMap[idStr];
					packetContainer.resize(recvBuffLen);
					memcpy(&packetContainer[0], &recvBuff, recvBuffLen);
					client->m_incomingPackets.push_back(packetContainer);
				}

				client->m_lastCommTime = t;
				Server::UnlockCommMutex();
				memset(recvBuff, 0, recvBuffLen);
			}

			if((t - lastTimeoutCheck) > 1000)
			{
				for(map<string, Server::Client *>::iterator it = g_clientConnectionMap.begin(); it != g_clientConnectionMap.end(); it++)
				{
					Server::Client *client = it->second;
					if((t - client->m_lastCommTime) > 10000)
						client->Kill();
				}
				lastTimeoutCheck = t;
			}

			if((t - lastUpdate) >= 33)
			{
				net->UpdateClients();
				lastUpdate += 33;
			}

			sleepMS(1);
		}

		while(Server::ms_clients.size())
		{
			Server::Client *client = Server::ms_clients.back();
			client->Kill();
			client->m_connectionThread->Join();
		}

		Thread::Exit();
		return 0;
	}

	bool Server::ms_isRunning = false;
	Thread *Server::ms_listenThread = 0;
	Socket *Server::ms_socket = 0;
	unsigned short Server::ms_port = 0;
	Mutex Server::ms_commMutex;
	vector<Server::Client *> Server::ms_clients;
	bool Server::Start(unsigned short port)
	{
		if(ms_isRunning || !port)
			return false;

		ms_socket = Socket::Create(Socket::IP4, Socket::Datagram, Socket::UDP, false);
		if(!ms_socket->Bind(port))
		{
			delete ms_socket;
			print("Unable to start server on port %d...\n", port);
			return false;
		}

		initializePacketHandlers();
		ms_packetBuffer = new PacketBuffer(CE_PACKET_BUFFER_SIZE, CE_PACKET_LIBRARY_SIZE);

		ms_listenThread = new Thread(serverProcessFunc);

		ms_isRunning = true;
		ms_port = port;
		ms_commMutex.Init();
		ms_listenThread->Start(0);

		print("Started server on port %d...\n", port);
		return true;
	}
	bool Server::IsRunning()
	{
		return ms_isRunning;
	}
	void Server::Shutdown()
	{
		if(!ms_isRunning)
			return;

		ms_isRunning = false;
		ms_listenThread->Join();
		delete ms_listenThread;

		ms_socket->Shutdown();
		ms_commMutex.Destroy();
		delete ms_socket;

		delete ms_packetBuffer;

		cleanupPacketHandlers();
	}
	void Server::LockCommMutex()
	{
		ms_commMutex.Lock();
	}
	void Server::UnlockCommMutex()
	{
		ms_commMutex.Unlock();
	}

	bool Client::ms_isRunning = false;
	Thread *Client::ms_connectionThread = 0;
	Socket *Client::ms_socket = 0;
	string Client::ms_address;
	unsigned short Client::ms_port = 0;
	void *Client::ms_sockAddr = 0;
	int Client::ms_sockAddrLen = 0;
	void *clientProcessFunc(void *arg)
	{
		unsigned long long lastProcess = getRunTimeMS();

		App *app = App::GetCurrent();
		Network *net = (Network *)Network::GetCurrent();

		Client::ms_socket->SendTo((char *)g_clientIntro.c_str(), g_clientIntro.length(), Client::ms_sockAddr, Client::ms_sockAddrLen);

		if(false)
		{
			string aliasMsg = "A:";
			aliasMsg.append(net->m_alias);
			aliasMsg.push_back('\n');
			Client::ms_socket->SendTo((char *)aliasMsg.c_str(), aliasMsg.length(), Client::ms_sockAddr, Client::ms_sockAddrLen);
		}

		char recvBuff[257];
		memset(recvBuff, 0, 257);
		int recvBuffLen = 0;
		struct sockaddr_in recvAddr;
		int recvAddrLen = sizeof(recvAddr);
		memset(&recvAddr, 0, recvAddrLen);
		string idStr, serverIdStr;
		unsigned long long lastTimeoutCheck, lastCommTime, lastPing, lastUpdate, t = getRunTimeMS();
		lastPing = lastUpdate = lastTimeoutCheck = lastCommTime = t;

		serverIdStr.resize(Client::ms_sockAddrLen);
		memcpy(&serverIdStr[0], Client::ms_sockAddr, Client::ms_sockAddrLen);

		PacketBuffer packetBuffer(CE_PACKET_BUFFER_SIZE, CE_PACKET_LIBRARY_SIZE);
		unsigned short packetIdx = 0;

		while(app->IsRunning() && Client::ms_isRunning)
		{
			t = getRunTimeMS();

			recvBuffLen = Client::ms_socket->RecvFrom(recvBuff, 256, &recvAddr, &recvAddrLen);
			if(recvBuffLen > 0)
			{
				idStr.resize(recvAddrLen);
				memcpy(&idStr[0], &recvAddr, recvAddrLen);

				if(!serverIdStr.compare(idStr))
				{
					PacketHandler::ProcessPacket(recvBuff, &packetBuffer, Client::ms_socket, Client::ms_sockAddr, Client::ms_sockAddrLen);
					lastCommTime = t;
				}
			}

			if((t - lastPing) >= 1000)
			{
				g_clientPingTable[g_lastClientPingID] = t;
				packetIdx = PacketHandler_Ping::Create(&packetBuffer, g_lastClientPingID++);
				if(g_lastClientPingID > 7)
					g_lastClientPingID = 0;
				Client::ms_socket->SendTo(packetBuffer.GetAddress(packetIdx), packetBuffer.GetSize(packetIdx), Client::ms_sockAddr, Client::ms_sockAddrLen);
				lastPing += 1000;
			}

			if((t - lastUpdate) >= 33)
			{
				net->UpdateServer(&packetBuffer);
				lastUpdate += 33;
			}

			sleepMS(1);
		}

		{
			packetIdx = PacketHandler_ClientDisconnect::Create(&packetBuffer);
			Client::ms_socket->SendTo(packetBuffer.GetAddress(packetIdx), packetBuffer.GetSize(packetIdx), Client::ms_sockAddr, Client::ms_sockAddrLen);
		}

		net->OnConnectionLost();

		Thread::Exit();
		return 0;
	}
	bool Client::Connect(string address, unsigned short port)
	{
		if(ms_isRunning)
			return false;

		print("Connecting to %s:%d...\n", address.c_str(), port);

		struct sockaddr_in *stSockAddr = new struct sockaddr_in;
		memset(stSockAddr, 0, sizeof(*stSockAddr));

		stSockAddr->sin_family = AF_INET;
		stSockAddr->sin_port = htons(port);
		int ret = inet_pton(AF_INET, address.c_str(), (char *)&stSockAddr->sin_addr);

		if(ret < 0)
		{
			error("[Error] Client::Connect - \"%s\" is not a valid address family.\n", address.c_str());
			delete stSockAddr;
			return false;
		}
		else if(ret == 0)
		{
			error("[Error] Client::Connect - \"%s:%d\" is not a valid ip address.\n", address.c_str(), port);
			delete stSockAddr;
			return false;
		}

		initializePacketHandlers();

		ms_socket = Socket::Create(Socket::IP4, Socket::Datagram, Socket::UDP, false);
		ms_sockAddr = (void *)stSockAddr;
		ms_sockAddrLen = sizeof(*stSockAddr);

		ms_address = address;
		ms_port = port;

		ms_connectionThread = new Thread(clientProcessFunc);

		Network *net = (Network *)Network::GetCurrent();
		net->OnConnectionAccepted();

		ms_isRunning = true;
		ms_connectionThread->Start(0);

		for(unsigned char a = 0; a < 8; a++)
			g_clientPingTable[a] = 0;

		return true;
	}
	bool Client::IsRunning()
	{
		return ms_isRunning;
	}
	void Client::Disconnect()
	{
		if(!ms_isRunning)
			return;

		ms_isRunning = false;

		ms_connectionThread->Join();
		delete ms_connectionThread;

		ms_socket->Shutdown();
		delete ms_socket;

		if(ms_sockAddr)
			delete (struct sockaddr_in *)ms_sockAddr;
		ms_sockAddr = 0;
		ms_sockAddrLen = 0;

		cleanupPacketHandlers();
	}

	Network *Network::ms_current = 0;
	Network *Network::GetCurrent()
	{
		return ms_current;
	}

	Network::Network()
	{
		ms_current = this;
	}
	void Network::OnClientConnected(Server::Client *client, PacketBuffer *packetBuffer)
	{
	}
	void Network::OnClientDisconnected(Server::Client *client)
	{
	}
	void Network::UpdateClients()
	{
	}
	void Network::OnConnectionAccepted()
	{
	}
	void Network::OnConnectionLost()
	{
	}
	void Network::UpdateServer(PacketBuffer *packetBuffer)
	{
	}
}
