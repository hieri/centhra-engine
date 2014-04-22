//- Standard Library -
#include <string.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Socket.h>

using namespace ce;

int main(int argc, char **argv)
{
	print("150 - TCP Client | Centhra Engine v%s\n", getVersionString().c_str());

	Socket *client = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP);

	client->Connect("127.0.0.1", 12345);

	const char *msg = "Hello World!";
	print("Sending Msg: %s\n", msg);
	client->Write((char *)msg, strlen(msg));

	client->Shutdown();
	delete client;

	return 0;
}
