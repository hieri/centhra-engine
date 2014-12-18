#ifndef _CE_FILE_H_
#define _CE_FILE_H_

//- Standard Library -
#include <map>
#include <string>

//- Centhra Engine -
#include <CE/Thread.h>

namespace ce
{
	class File;

	class Directory
	{
		friend class File;
	protected:
		static std::map<std::string, Directory *> ms_directoryMap;

		std::string m_path;
	public:
		static Directory *GetFromPath(const char *path);

		Directory(const char *path);
		~Directory();

		std::string GetPath() const;

		//- Subdirectories -
	protected:
		std::map<std::string, Directory *> m_subDirectories;
	public:

		//- Files -
	protected:
		std::map<std::string, File *> m_files;
	public:

		//- Change Monitoring -
	protected:
		bool m_isMonitoring;
		Thread *m_monitorThread;
	public:
		bool IsMonitoring() const;
		void SetMonitoring(bool monitoring);

		void Monitor();
	};

	class File
	{
		friend class Directory;
	public:
		static void Cleanup();

		File(const char *path);
		~File();

		void Refresh();

		//- Type -
		typedef enum FileType
		{
			Unknown,
			Image,
			Sound,
			NextType
		} FileType;

		//- Attributes -
	protected:
		std::string m_fileName, m_fileExtension, m_fileBase, m_filePath;
	public:
		std::string GetFileName() const;
		std::string GetFileExtension() const;
		std::string GetFileBase() const;
		std::string GetFilePath() const;

		//- Virtual Object -
	public:
		class FileObject
		{
			friend class File;
		protected:
			File *m_file;
		public:
			FileObject();

			File *GetFile() const;

			virtual void OnFileChange();
		};
	protected:
		FileObject *m_object;
	public:
		FileObject *GetObject() const;
		void SetObject(FileObject *object);
	};
}

#endif
