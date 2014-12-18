#ifndef _CE_BASE_H_
#define _CE_BASE_H_

//- Standard Library -
#include <cstdarg>
#include <string>

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	//- Time -
	void sleepMS(unsigned long timeMS);
	unsigned long long getRunTimeMS();
	unsigned long long getRealTimeMS();
	unsigned long long getStartTimeMS();

	//- Error Handling -
	void clearError();
	std::string getError();
	bool getVerboseLogging();
	void setError(const char *error, ...);
	void setVerboseLogging(bool verboseLogging);

	//- File IO -
	bool fileExists(const char *file);
	bool fileDelete(const char *file);
	bool fileRename(const char *file, const char *target);
	std::string fileExt(const char *file);
	std::string fileBase(const char *file);
	std::string fileName(const char *file);
	std::string fileCompressPath(const char *file);
	unsigned long fileSize(const char *file);
	bool createFolder(const char *folder);

	//- Messages -
	void ioMutexInit();
	void ioMutexDestroy();
	void error(const char *format, ...);
	void print(const char *format, ...);
	void warn(const char *format, ...);
	std::string formatString(const char *format, va_list args, unsigned short size = 128);

	//- Version -
	unsigned int getVersionMajor();
	unsigned int getVersionMinor();
	std::string getVersionString();
}

#endif
