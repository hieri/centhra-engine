//- Standard Library -
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string>

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

#ifdef _WIN32
	//- Windows Patches -
	#include <Windows.h>
	#define snprintf _snprintf_s
	#define vsnprintf _vsnprintf_s
#endif

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/App.h>

//- TODO: handle warn differently than print -

using namespace std;

namespace ce
{
	string g_error = "";

	string compileMessage(const char *format, va_list ap)
	{
		char text[256];
		vsnprintf(text, 256, format, ap);
		return string(text);
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
		g_error = string(compileMessage(format, ap));
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

	void error(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = compileMessage(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnError(msg.c_str());
		cerr << msg;
		va_end(ap);
	}
	void print(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = compileMessage(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnPrint(msg.c_str());
		cout << msg;
		va_end(ap);
	}
	void warn(const char *format, ...)
	{
		va_list	ap;
		va_start(ap, format);
		string msg = compileMessage(format, ap);
		App *current = App::GetCurrent();
		if(current)
			current->OnWarn(msg.c_str());
		cout << msg;
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
}
