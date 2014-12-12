//- Standard Library -
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <ctime>

#ifdef linux
	//- Linux -
	#include <unistd.h>
	#include <sys/time.h>
	#include <sys/stat.h>
#endif

#ifdef _WIN32
	//- Windows Patches -
	#include <Windows.h>
	#include <direct.h>

	#if _MSC_VER >= 1400
		#define snprintf _snprintf_s
		#define vsnprintf _vsnprintf_s
	#endif
#endif

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/App.h>
#include <CE/Mutex.h>

using namespace std;

namespace ce
{
	string g_error = "";

	string formatString(const char *format, va_list args, unsigned short size)
	{
		char *buffer = new char[size + 1];
		#ifdef _WIN32
			vsnprintf(buffer, size + 1, size, format, args);
		#else
			vsnprintf(buffer, size + 1, format, args);
		#endif
		string str(buffer);
		delete [] buffer;
		return str;
	}

	bool m_verboseLogging = false;
	void clearError()
	{
		g_error = "";
	}
	string getError()
	{
		return g_error;
	}
	bool getVerboseLogging()
	{
		return m_verboseLogging;
	}
	void setError(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		g_error = string(formatString(format, ap));
		va_end(ap);

		if(m_verboseLogging)
			error("%s\n", g_error.c_str());
	}
	void setVerboseLogging(bool verboseLogging)
	{
		m_verboseLogging = verboseLogging;
	}

	bool fileExists(const char *file)
	{
		ifstream in(file, ifstream::in | ifstream::binary);
		bool exists = in.is_open();
		in.close();
		return exists;
	}
	bool fileDelete(const char *file)
	{
		return remove(file) == 0;
	}
	string fileExt(const char *file)
	{
		string str(file);
		return str.substr(str.find_last_of(".") + 1);
	}
	string filePath(const char *file)
	{
		string str(file);
		int a,b,c;
		a = str.find_last_of("/");
		b = str.find_last_of("\\");
		c = a > b ? a : b;
		return str.substr(0, c + 1);
	}
	string fileCompressPath(const char *file)
	{
		string str(file);

		int idx = 0;
		while((idx = str.find("\\")) != string::npos)
			str = str.replace(idx, 1, "/");
		while((idx = str.find("/./")) != string::npos)
			str = str.replace(idx, 3, "/");
		return str;
	}
	bool createFolder(const char *folder)
	{
		stringstream folderStream(folder);
		string curr, final;
		while(getline(folderStream, curr, '/'))
		{
			if(final.size())
				final.push_back('/');
			final.append(curr);
			#ifdef linux
				mkdir(final.c_str(), 0777);
			#endif
			#ifdef _WIN32
				_mkdir(final.c_str());
			#endif
		}

		return true;
	}

	Mutex g_ioMutex;
	void ioMutexInit()
	{
		g_ioMutex.Init();
	}
	void ioMutexDestroy()
	{
		g_ioMutex.Destroy();
	}
	void error(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = formatString(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnError(msg.c_str());
		if(g_ioMutex.IsAlive())
			g_ioMutex.Lock();
		cerr << msg;
		if(g_ioMutex.IsAlive())
			g_ioMutex.Unlock();
		va_end(ap);
	}
	void print(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = formatString(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnPrint(msg.c_str());
		if(g_ioMutex.IsAlive())
			g_ioMutex.Lock();
		cout << msg;
		if(g_ioMutex.IsAlive())
			g_ioMutex.Unlock();
		va_end(ap);
	}
	void warn(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = formatString(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnWarn(msg.c_str());
		if(g_ioMutex.IsAlive())
			g_ioMutex.Lock();
		cout << msg;
		if(g_ioMutex.IsAlive())
			g_ioMutex.Unlock();
		va_end(ap);
	}

	unsigned int getVersionMajor()
	{
		return CE_VERSION_MAJOR;
	}
	unsigned int getVersionMinor()
	{
		return CE_VERSION_MINOR;
	}
	string getVersionString()
	{
		char text[8];
		snprintf(text, 8, "%i.%.2i", CE_VERSION_MAJOR, CE_VERSION_MINOR);
		return string(text);
	}

	void sleepMS(unsigned long timeMS)
	{
		#ifdef linux
			usleep(timeMS * 1000);
		#endif

		#ifdef _WIN32
			Sleep(timeMS);
		#endif
	}
	unsigned long long g_startTimeMS = getRunTimeMS();
	unsigned long long getStartTimeMS()
	{
		return g_startTimeMS;
	}
	unsigned long long getRealTimeMS()
	{
		unsigned long long currTimeMS;

		#ifdef linux
			struct timeval currTime;
			gettimeofday(&currTime, 0);
			currTimeMS = currTime.tv_sec * 1000 + currTime.tv_usec / 1000;
		#endif

		#ifdef _WIN32
			LARGE_INTEGER frequency;
			if(QueryPerformanceFrequency(&frequency))
			{
				LARGE_INTEGER count;
				QueryPerformanceCounter(&count);
				currTimeMS = (unsigned long long)((1000 * count.QuadPart) / frequency.QuadPart);
			}
			else
				currTimeMS = (unsigned long long)GetTickCount();
		#endif

		return currTimeMS;
	}
	unsigned long long getRunTimeMS()
	{
		return getRealTimeMS() - g_startTimeMS;
	}
}
