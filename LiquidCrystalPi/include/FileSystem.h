#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <string>

namespace pi
{
	class FileSystem
	{
	public:
		FileSystem();
		~FileSystem();

		std::string getCwd() const;
		std::string	getRoot() const;
		bool		setCwd(const std::string& value);

	private:
	};
}

#endif // !_FILE_SYSTEM_H_
