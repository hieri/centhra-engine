#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/File.h>
#include <CE/Thread.h>
#include <CE/App.h>

using namespace std;

namespace ce
{
	void *monitorFunc(void *arg)
	{
		((Directory *)arg)->Monitor();
		Thread::Exit(NULL);
		return 0;
	}

	map<string, Directory *> Directory::ms_directoryMap = map<string, Directory *>();

	void File::Cleanup()
	{
		//- Delete all directories and their files -
	}

	//---------------------------- Directory ----------------------------
	Directory *Directory::GetFromPath(const char *path)
	{
		string str(path);
		if(ms_directoryMap.count(str))
			return ms_directoryMap[str];
		return 0;
	}
	Directory::Directory(const char *path) : m_path(path), m_isMonitoring(false), m_monitorThread(0)
	{
		ms_directoryMap[m_path] = this;
	}
	Directory::~Directory()
	{
		delete m_monitorThread;
	}
	string Directory::GetPath() const
	{
		return m_path;
	}
	bool Directory::IsMonitoring() const
	{
		return m_isMonitoring;
	}
	void Directory::SetMonitoring(bool monitoring)
	{
		if(m_isMonitoring == monitoring)
			return;

		m_isMonitoring = monitoring;

		if(monitoring)
		{
			m_monitorThread = new Thread(monitorFunc);
			m_monitorThread->Start(this);
		}
	}
	void Directory::Monitor()
	{
		App *app = App::GetCurrent();

		#ifdef _WIN32
			HANDLE dwDirectoryHandle = CreateFile(m_path.c_str(),
				FILE_LIST_DIRECTORY,
				FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			if(dwDirectoryHandle == INVALID_HANDLE_VALUE)
			{
				setError("Directory::WatchForUpdates: Invalid directory handle.\n");
				return;
			}
			if((dwDirectoryHandle == NULL))
			{
				setError("Directory::WatchForUpdates: Directory handle is undefined.\n");
				return;
			}
			
			//TODO: Find optimal size for this
			FILE_NOTIFY_INFORMATION strFileNotifyInfo[20];
			DWORD dwBytesReturned = 0;
			while(true)
			{
				memset(&strFileNotifyInfo, 0, sizeof(strFileNotifyInfo));
				if(ReadDirectoryChangesW(dwDirectoryHandle, (LPVOID)&strFileNotifyInfo, sizeof(strFileNotifyInfo), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, &dwBytesReturned, 0, 0) == FALSE)
				{
					setError("Directory::WatchForUpdates: ReadDirectoryChangesW failed.\n");
					return;
				}

				if(!app->IsRunning() || !m_isMonitoring)
					return;

				string targetFile;
				unsigned short size = (unsigned short)strFileNotifyInfo[0].FileNameLength / 2;
				targetFile.reserve(size);
				for(unsigned short a = 0; a < size; a++)
				{
					if((char)strFileNotifyInfo[0].FileName[a] == '~')
						break;
					targetFile.push_back((char)strFileNotifyInfo[0].FileName[a]);
				}

				if(m_files.count(targetFile))
					m_files[targetFile]->Refresh();
			}
		#endif
	}

	//---------------------------- File ----------------------------
	File::File(const char *path) : m_object(0)
	{
		m_filePath = fileCompressPath(path);
		m_fileName = fileName(m_filePath.c_str());
		m_fileBase = fileBase(m_filePath.c_str());
		m_fileExtension = fileExt(m_filePath.c_str());

		//- Add to directory -
		Directory *dir = Directory::GetFromPath(m_fileBase.c_str());
		if(dir == 0)
			dir = new Directory(m_fileBase.c_str());
		dir->m_files[m_fileName] = this;
	}
	File::~File()
	{
		//- Remove from directory -
	}
	void File::Refresh()
	{
		if(m_object)
			m_object->OnFileChange();
	}

	//- Attributes -
	string File::GetFileName() const
	{
		return m_fileName;
	}
	string File::GetFileExtension() const
	{
		return m_fileExtension;
	}
	string File::GetFileBase() const
	{
		return m_fileBase;
	}
	string File::GetFilePath() const
	{
		return m_filePath;
	}

	//- Virtual Object -
	File::FileObject::FileObject() : m_file(0)
	{
	}
	File *File::FileObject::GetFile() const
	{
		return m_file;
	}
	void File::FileObject::OnFileChange()
	{
	}

	File::FileObject *File::GetObject() const
	{
		return m_object;
	}
	void File::SetObject(File::FileObject *object)
	{
		m_object = object;
		m_object->m_file = this;
	}
}
