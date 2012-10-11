#ifndef _CE_BASE_H_
#define _CE_BASE_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	//- Time -
	void sleepMS(unsigned int timeMS);
	void sleepUS(unsigned int timeUS);

	//- Error Handling -
	void clearError();
	std::string getError();
	bool getVerboseLogging();
	void setError(const char *error, ...);
	void setVerboseLogging(bool verboseLogging);

	//- File IO -
	bool fileExists(const char *file);
	std::string fileExt(const char *file);
	std::string filePath(const char *file);
	std::string fileCompressPath(const char *file);

	//- Messages -
	void error(const char *format, ...);
	void print(const char *format, ...);
	void warn(const char *format, ...);

	//- Version -
	unsigned int getVersionMajor();
	unsigned int getVersionMinor();
	std::string getVersionString();
}

#endif
