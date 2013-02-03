#ifndef _CE_SOCKET_H_
#define _CE_SOCKET_H_

namespace ce
{
	/**	@brief Socket Data Structure Class
	 */
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

		/**	@brief Constructor.
		 */
		static Socket *Create(Domain domain, Type type, Protocol protocol);

		/**	@brief Destructor.
		 */
		~Socket();

		/**	@brief Returns the Socket of a newly accepted connection.
		 */
		Socket *Accept();

		/**	@brief Attempts to bind the socket to a port and returns true if successful.
		 */
		bool Bind(unsigned int port);

		/**	@brief Attempts to listen to the port the socket is bound to and returns true if successful.
		 */
		bool Listen(unsigned int backlog);

		/**	@brief Attempts to connect to a location and returns true if successful.
		 */
		bool Connect(const char *address, unsigned int port);

		/**	@brief Attempts to shutdown the socket connection and returns true if successful.
		 */
		bool Shutdown();

		/**	@brief Reads data from the socket connection.
		 */
		void Read(char *buffer, unsigned int length);

		/**	@brief Writes data to the socket connection.
		 */
		void Write(char *buffer, unsigned int length);
	};
}

#endif
