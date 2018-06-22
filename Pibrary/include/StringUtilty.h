#ifndef _STRING_UTILITY_H_
#define _STRING_UTILITY_H_

#include <Config.h>
#include <string>
#include <vector>
#include <sstream>

namespace pi
{
	class Str
	{
	public: // Manipulation
		static bool			Autocomplete(std::string& value, const std::vector<std::string>& list);
		static std::string	Extract(const std::string& src, const std::string& s);
		static std::string	Extract(const std::string& src, const std::string& first, const std::string& last);
		static std::string	Replace(const std::string& src, const std::string& find, const std::string& replace);
		static std::string	Sprintf(const std::string& fmt, ...);
		static std::string	ToLower(const std::string& src);
		static std::string	ToUpper(const std::string& src);

		static std::vector<std::string> Split(const std::string& src, const std::string& delim);
		static std::vector<std::string> Split(const std::string& src, const std::vector<std::string>& delims);

	public: // Checks
		static bool		Contains(const std::string& src, char c);
		static bool		IsAlpha(const std::string& src);
		static bool		IsAlnum(const std::string& src);
		static bool		IsBool(const std::string& src);
		static bool		IsDecimal(const std::string& src);
		static bool		IsInt(const std::string& src);
		static bool		IsLower(const std::string& src);
		static bool		IsName(const std::string& src);
		static bool		IsNumber(const std::string& src);
		static bool		IsText(const std::string& src);
		static bool		IsUpper(const std::string& src);
		static bool		IsWrap(const std::string& src, char c);
		static bool		IsWrap(const std::string& src, char front, char back);
		static bool		IsWrap(const std::string& src, const std::string& s);
		static bool		IsWrap(const std::string& src, const std::string& front, const std::string& back);

	public: // Conversions
		static bool		ToBool(const std::string& src);
		static int		ToInt(const std::string& src);
		static double	ToDecimal(const std::string& src);
		static float	ToFloat(const std::string& src);

		static bool		MakeBool(const std::string& src, bool& value);
		static bool		MakeInt(const std::string& src, int& value);
		static bool		MakeDecimal(const std::string& src, double& value);
		static bool		MakeFloat(const std::string& src, float& value);

	public: // Templates
		template<typename T, typename ... A>
		inline static std::string Format(const std::string& fmt, const T& arg0, const A&... args)
		{
			// Move args into stringstream
			std::stringstream ss;
			ss << arg0 << std::endl;

			int sink[] = { 0, ((void)(ss << args << std::endl), 0)... };
			(void)sink;

			std::string	out = fmt;
			uint	pos = 0;

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

		inline static std::string FormatA(const std::string& fmt, const std::vector<std::string>& args)
		{
			std::string out = fmt;

			for (unsigned i = 0; i < args.size(); i++)
			{
				std::string find = Str::Sprintf("{%i}", i);

				std::string replace = args[i];

				out = Str::Replace(out, find, replace);
			}

			return out;
		}
	};
}

#endif // !_STRING_UTILITY_H_
