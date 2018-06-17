#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <string>
#include <vector>
#include <sstream>

namespace pi
{
	class Str
	{
	public:
		static bool IsInt(const std::string& s);

		static std::string	Replace(const std::string& src, const std::string& find, const std::string& replace);
		static std::string	Sprintf(const std::string fmt, ...);

		template<typename T, typename ... A>
		inline static std::string Format(const std::string& fmt, const T& arg0, const A&... args)
		{
			// Move args into stringstream
			std::stringstream ss;
			ss << arg0 << std::endl;

			int sink[] = { 0, ((void)(ss << args << std::endl), 0)... };
			(void)sink;

			std::string	out = fmt;
			std::size_t pos= 0;

			// Replace each format value with the ones in the stringstream
			while (ss.good())
			{
				std::string find = Str::Sprintf("{%i}", pos);

				std::string replace;

				std::getline(ss, replace);

				out = Str::Replace(out, find, replace);

				pos++;
			}

			return out;
		}
	};
}

#endif // !_STRING_UTILITY_H_
