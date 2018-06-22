#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <string>
#include <iostream>

namespace pi
{
	class Console
	{
	public:
		Console();
		Console(std::ostream& out, std::istream& in, std::ostream& err);
		Console(const Console& copy);
		~Console();

		bool setup();

		Console& print(char value);
		Console& print(const char* value);
		Console& print(const std::string& value);
		Console& printf(const char* fmt, ...);

		const std::string getLine(const std::string& prefix) const;

	private:
	};
}

#endif // !_CONSOLE_H_
