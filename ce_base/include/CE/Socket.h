#ifndef _CE_SOCKET_H_
#define _CE_SOCKET_H_

namespace ce
{
	class Socket
	{
		static int ms_count;

		int m_socket;

		Socket();

	public:
		typedef enum Domain
		{
			IP4,
			IP6
		} Domain;
		typedef enum Protocol
		{
			TCP,
			UDP
		} Protocol;
		typedef enum Type
		{
			Raw,
			Stream,
			Datagram,
			SequencedPacket
		} Type;

		static Socket *create(Domain domain, Type type, Protocol protocol);

		~Socket();

		Socket *Accept();
		bool Bind(unsigned int port);
		bool Listen(unsigned int backlog);

		bool Connect(const char *address, unsigned int port);
		bool Shutdown();

		void Read(char *buffer, unsigned int length);
		void Write(char *buffer, unsigned int length);
	};
}

#endif
