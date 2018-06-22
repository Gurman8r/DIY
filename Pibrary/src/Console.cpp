#include <Console.h>
#include <stdarg.h>
#include <StringUtilty.h>

namespace pi
{
	Console::Console()
	{
	}

	Console::Console(std::ostream& out, std::istream& in, std::ostream& err)
	{
	}

	Console::Console(const Console & copy)
	{
	}

	Console::~Console()
	{
	}


	bool Console::setup()
	{
		return true;
	}
	
	
	Console & Console::print(char value)
	{
		std::cout << value;
		return (*this);
	}
	
	Console & Console::print(const char * value)
	{
		while (*value)
		{
			print(*(value++));
		}
		return (*this);
	}
	
	Console & Console::print(const std::string & value)
	{
		return print(value.c_str());
	}
	
	Console & Console::printf(const char * fmt, ...)
	{
		va_list argp;
		char buffer[1024];

		va_start(argp, fmt);
		vsnprintf(buffer, 1023, fmt, argp);
		va_end(argp);

		return print(buffer);
	}
	
	
	const std::string Console::getLine(const std::string & prefix) const
	{
		static std::string line;
		std::cout << std::endl << prefix;
		std::getline(std::cin, line);
		return line;
	}
}
