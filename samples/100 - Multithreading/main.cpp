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
		print((char *)arg);

		counter++;
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

	Thread threadA(&threadFunc);
	threadA.start((void *)"Thread A\n");

	sleepMS(500);

	Thread threadB(&threadFunc);
	threadB.start((void *)"Thread B\n");

	print("Main Finished\n");
	Thread::exit(NULL);
	return 0;
}
