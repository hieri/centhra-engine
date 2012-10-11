#ifndef CE_BASE_H
#define CE_BASE_H

//- Windows Patches -
#ifdef _MSC_VER
	#define snprintf _snprintf_s
#endif

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/Config.h>

namespace ce
{
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
