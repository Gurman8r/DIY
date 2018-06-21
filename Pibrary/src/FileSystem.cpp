#include <FileSystem.h>
#include <libgen.h>
#include <unistd.h>

#define MAX_PATH 512

namespace pi
{
	FileSystem::FileSystem()
	{
	}

	FileSystem::~FileSystem()
	{
	}


	std::string FileSystem::getCwd() const
	{
		char buf[1024];
		if (getcwd(buf, sizeof(buf)))
		{
			return std::string(buf);
		}
		return std::string();
	}

	std::string FileSystem::getRoot() const
	{
		// PID String
		pid_t pid = getpid();
		char buf[20] = { 0 };
		sprintf(buf, "%d", pid);

		// Process Path
		std::string link = "/proc/";
		link += buf;
		link += "/exe";

		char proc[MAX_PATH];
		ssize_t count = readlink(link.c_str(), proc, MAX_PATH);
		if (count != -1)
		{
			proc[count] = 0;
			std::string path = proc;
			std::size_t last = path.find_last_of("/");
			path = path.substr(0, last);

			return path + std::string("/");
		}
		return std::string();
	}

	bool FileSystem::setCwd(const std::string & value)
	{
		if (chdir(value.c_str()) == EXIT_SUCCESS)
		{
			return true;
		}
		return false;
	}
}
