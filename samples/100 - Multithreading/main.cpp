//- Standard Library -
#include <string.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Thread.h>

using namespace ce;

void *threadFunc(void *arg)
{
	int counter = 0;
	while(true)
	{
		counter++;

		print("| o-> %s [%d/10]\n", (char *)arg, counter);
		if(counter >= 10)
			break;
		sleepMS(1000);
	}
	
	Thread::Exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("100 - Multithreading | Centhra Engine v%s\n", getVersionString().c_str());

	Thread threadA(&threadFunc), threadB(&threadFunc);

	print("o-> Main Started\n");
	threadA.Start((void *)"Thread A");
	sleepMS(500);
	threadB.Start((void *)"Thread B");

	threadA.Join();
	threadB.Join();

	print("o-> Main Ended\n");
	Thread::Exit(NULL);
	return 0;
}
