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
	
	Thread::exit(NULL);
	return 0;
}

int main(int argc, char **argv)
{
	print("100 - Multithreading | Centhra Engine v%s\n", getVersionString().c_str());

	Thread threadA(&threadFunc), threadB(&threadFunc);

	print("o-> Main Started\n");
	threadA.start((void *)"Thread A");
	sleepMS(500);
	threadB.start((void *)"Thread B");

	print("o-> Main Ended\n");
	Thread::exit(NULL);
	return 0;
}
