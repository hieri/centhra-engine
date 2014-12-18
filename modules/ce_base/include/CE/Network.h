#ifndef _CE_NETWORK_H_
#define _CE_NETWORK_H_

//- Standard Library -
#include <vector>
#include <deque>
#include <string>

//- Centhra Engine -
#include <CE/Mutex.h>
#include <CE/Socket.h>
#include <CE/Thread.h>
#include <CE/Vector2.h>

namespace ce
{
	class PacketBuffer
	{
		char *m_buffer;
		std::pair<char *, unsigned char> *m_library;
		unsigned short m_nextIndex, m_edge, m_bufferSize, m_librarySize;

	public:
		PacketBuffer(unsigned short bufferSize, unsigned short librarySize);
		~PacketBuffer();

		char *Allocate(unsigned char size, unsigned short &index);
		char *GetAddress(unsigned short idx) const;
		unsigned short GetSize(unsigned short idx) const;
	};

	class PacketQueue
	{
	public:
		unsigned short *m_packetQueue, m_firstPacket, m_lastPacket;

		PacketQueue();
	};

	class PacketHandler
	{
	protected:
		unsigned char m_type;

	public:
		static void ProcessPacket(char *start, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);

		PacketHandler(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};
	class PacketHandler_ServerInfo : public PacketHandler
	{
		static unsigned char ms_type;

	public:
		static unsigned short Create(PacketBuffer *buffer);

		PacketHandler_ServerInfo(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};
	class PacketHandler_ClientJoin : public PacketHandler
	{
		static unsigned char ms_type;

	public:
		static unsigned short Create(PacketBuffer *buffer);

		PacketHandler_ClientJoin(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};
	class PacketHandler_ClientDisconnect : public PacketHandler
	{
		static unsigned char ms_type;

	public:
		static unsigned short Create(PacketBuffer *buffer);

		PacketHandler_ClientDisconnect(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};
	class PacketHandler_Ping : public PacketHandler
	{
		static unsigned char ms_type;

	public:
		static unsigned short Create(PacketBuffer *buffer, unsigned short id);

		PacketHandler_Ping(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};
	class PacketHandler_Pong : public PacketHandler
	{
		static unsigned char ms_type;

	public:
		static unsigned short Create(PacketBuffer *buffer, unsigned short id);

		PacketHandler_Pong(unsigned char type);
		virtual void OnProcess(char *data, PacketBuffer *buffer, Socket *socket, void *sockAddr, int sockAddrLen, void *client = 0);
	};

	void *serverProcessFunc(void *arg);
	void *serverClientConnectionFunc(void *arg);
	class Server
	{
		static bool ms_isRunning;
		static Thread *ms_listenThread;
		static unsigned short ms_port;

	public:
		static PacketBuffer *ms_packetBuffer;
		static Socket *ms_socket;
		static Mutex ms_commMutex;

		class Client
		{
			std::string m_id;
			bool m_isAlive;
			Thread *m_connectionThread;
			std::deque<std::string> m_incomingPackets;
			unsigned long long m_lastCommTime;

		public:
			void *m_sockAddr, *m_userData;
			int m_sockAddrLen;
			std::string m_alias;
			unsigned long long m_pingTable[8];
			unsigned char m_lastPingID;
			unsigned short m_ping;

			Client();
			~Client();

			bool IsAlive() const;
			void Delete();
			void SetUserData(void *userData);
			void *GetUserData() const;

			friend void *serverClientConnectionFunc(void *arg);
			friend void *serverProcessFunc(void *arg);
		};

		static bool Start(unsigned short port);
		static bool IsRunning();
		static void Shutdown();
		static void LockCommMutex();
		static void UnlockCommMutex();

		friend void *serverProcessFunc(void *arg);
		friend void *serverClientConnectionFunc(void *arg);

	private:
		static std::vector<Server::Client *> ms_clients;
	};

	void *clientProcessFunc(void *arg);
	class Client
	{
		static bool ms_isRunning;
		static Thread *ms_connectionThread;
		static std::string ms_address;
		static unsigned short ms_port;

	public:
		static void *ms_sockAddr;
		static int ms_sockAddrLen;
		static Socket *ms_socket;

		static bool Connect(std::string address, unsigned short port);
		static bool IsRunning();
		static void Disconnect();

		friend void *clientProcessFunc(void *arg);
	};

	void SetPacketInitializer(void (*initializer)(unsigned char, std::vector<PacketHandler *> *));

	class Network
	{
		static Network *ms_current;

	public:
		static Network *GetCurrent();

		Network();

		std::string m_alias;

		//- Server Callbacks -
		virtual void OnClientConnected(Server::Client *client, PacketBuffer *packetBuffer);
		virtual void OnClientDisconnected(Server::Client *client);
		virtual void UpdateClients();

		//- Client Callbacks -
		virtual void OnConnectionAccepted();
		virtual void OnConnectionLost();
		virtual void UpdateServer(PacketBuffer *packetBuffer);
	};
}

#endif
