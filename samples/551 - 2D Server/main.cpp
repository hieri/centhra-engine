//- Standard Library -
#include <string.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Socket.h>
#include <CE/Thread.h>
#include <CE/Mutex.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/DefaultPhysicsHandler.h>

using namespace ce;
using namespace std;

Mutex g_physicsMutex;
game2d::PhysicalGroup *g_group = 0;
game2d::DefaultPhysicsHandler *g_defaultPhysicsHandler = 0;

void *physicsFunc(void *arg)
{
	unsigned long long lastProcess = getRunTimeMS();

	bool *isRunning = (bool *)arg;

	while(!isRunning)
	{
		unsigned long long t = getRunTimeMS();
		if((t - lastProcess) > 15)
		{
			float dt = (float)(t - lastProcess) / 1000.f;
			lastProcess = t;

			g_physicsMutex.Lock();
			g_group->ProcessPhysics(dt);
			g_physicsMutex.Unlock();
		}

		sleepMS(1);
	}

	Thread::Exit();
	return 0;
}

void *clientFunc(void *arg)
{
	pair<bool *, Socket *> *data = (pair<bool *, Socket *> *)arg;
	Socket *client = data->second;
	bool *isRunning = data->first;
	delete data;

	print("New Connection\n");

	delete client;

	Thread::Exit();
	return 0;
}

int main(int argc, char **argv)
{
	print("551 - 2D Server | Centhra Engine v%s\n", getVersionString().c_str());

	bool isRunning = true;

	g_group = new game2d::PhysicalGroup();
	g_defaultPhysicsHandler = new game2d::DefaultPhysicsHandler();
	g_group->AttachHandler(g_defaultPhysicsHandler);

	g_physicsMutex.Init();
	Thread physicsThread(physicsFunc);
	physicsThread.Start(&isRunning);


	Socket *server = Socket::Create(Socket::IP4, Socket::Stream, Socket::TCP, true);
	server->Bind(12346);
	server->Listen(16);
	vector<Thread *> clientThreads;

	while(true)
	{
		Socket *client = server->Accept();

		if(client)
		{
			pair<bool *, Socket *> *data = new pair<bool *, Socket *>(&isRunning, client);
			Thread *clientThread = new Thread(clientFunc);
			clientThreads.push_back(clientThread);
			clientThread->Start(data);
		}

		sleepMS(16);
	}

	isRunning = false;

	physicsThread.Join();
	for(vector<Thread *>::iterator it = clientThreads.begin(); it != clientThreads.end(); it++)
	{
		Thread *thread = *it;
		thread->Join();
		delete thread;
	}

	g_physicsMutex.Destroy();
	g_group->DetachHandler();
	delete g_defaultPhysicsHandler;
	delete g_group;
	delete server;

	Thread::Exit();
	return 0;
}
