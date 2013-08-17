#ifndef _CE_BASE_SOCKET_H_
#define _CE_BASE_SOCKET_H_

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
		 *	@param domain Domain of the Socket.
		 *	@param type Type of the Socket.
		 *	@param protocol Protocol of the Socket.
		 *	@return Socket object or 0 if creation failed.
		 */
		static Socket *Create(Domain domain, Type type, Protocol protocol);

		/**	@brief Destructor.
		 */
		~Socket();

		/**	@brief Returns the Socket of a newly accepted connection.
		 *	@return Socket of newly accepted connection.
		 */
		Socket *Accept();

		/**	@brief Attempts to bind the socket to a port.
		 *	@param port Target port of the binding.
		 *	@return Whether or not the binding was successful.
		 */
		bool Bind(unsigned int port);

		/**	@brief Configures the socket to listen to the port the socket is bound to.
		 *	@param backlog Maximum number of connections queued for acceptance.
		 *	@return Whether or not configuration was successful.
		 */
		bool Listen(unsigned int backlog);

		/**	@brief Attempts to connect to a location.
		 *	@param address Target address of connection.
		 *	@param port Target port of connection.
		 *	@return Whether or not the connection was successful.
		 */
		bool Connect(const char *address, unsigned int port);

		/**	@brief Attempts to shutdown the socket connection.
		 *	@return Whether or not the shutdown was successful.
		 */
		bool Shutdown();

		/**	@brief Reads data from the socket connection.
		 *	@param buffer Buffer for storing the result of the read request.
		 *	@param length Length of the read request.
		 */
		void Read(char *buffer, unsigned int length);

		/**	@brief Writes data to the socket connection.
		 *	@param buffer Buffer of the write request.
		 *	@param length Length of the write request.
		 */
		void Write(char *buffer, unsigned int length);
	};
}

#endif
