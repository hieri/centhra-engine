//- Standard Library -
#include <string.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Socket.h>

using namespace ce;

int main(int argc, char **argv)
{
	print("151 - TCP Server | Centhra Engine v%s\n", getVersionString().c_str());

	Socket *server = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP, false);

	server->Bind(1234);
	server->Listen(16);

	while(true)
	{
		Socket *client = server->Accept();

		if(client)
		{
			char buff[1024];
			memset(buff, 0, 1024);
			client->Read(buff, 1024);
			print("Receiving Msg: %s\n", buff);

			client->Shutdown();
			delete client;
		}

		sleepMS(1);
	}

	delete server;

	return 0;
}
