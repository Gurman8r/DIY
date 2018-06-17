#include <StringUtilty.h>
#include <stdarg.h>
#include <stdarg.h>
#include <locale>
#include <sstream>
#include <assert.h>
#include <cctype>
#include <algorithm>
#include <string.h>
#include <vector>

namespace pi
{
	bool Str::IsInt(const std::string & s)
	{
		// https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) 
			return false;

		char * p;
		strtol(s.c_str(), &p, 10);

		return (*p == 0);
	}

	std::string	Str::Replace(const std::string& src, const std::string& find, const std::string& replace)
	{
		if (src == "" || find == "")
		{
			return src;
		}

		std::string copy = src;

		for (uint i = 0; (i = copy.find(find, i)) != std::string::npos;)
		{
			copy.replace(i, find.length(), replace);

			i += replace.length();
		}

		return copy;
	}

	std::string	Str::Sprintf(const std::string fmt, ...)
	{
		va_list argp;
		char buffer[1024];

		va_start(argp, fmt);
		vsnprintf(buffer, 1023, fmt.c_str(), argp);
		va_end(argp);

		return std::string(&buffer[0]);
	}
}
