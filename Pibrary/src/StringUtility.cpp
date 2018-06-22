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
	bool	Str::Autocomplete(std::string& value, const std::vector<std::string>& options)
	{
		std::vector<std::string> matches;

		for (std::string opt : options)
		{
			if (value == opt)
			{
				value = opt;

				return true;
			}

			if (value.size() <= opt.size())
			{
				bool found = true;

				for (uint i = 0; i < value.size(); i++)
				{
					if (value[i] != opt[i])
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					matches.push_back(opt);
				}
			}
		}

		if (matches.size() == 1)
		{
			value = matches.front();
			return true;
		}

		return false;
	}
	std::string	Str::Extract(const std::string & src, const std::string & s)
	{
		return Extract(src, s, s);
	}
	std::string	Str::Extract(const std::string& src, const std::string& first, const std::string& last)
	{
		uint ss = src.size();
		uint fs = first.size();
		uint ls = last.size();

		if (ss >= fs + ls)
		{
			std::string sf = src.substr(0, fs);
			std::string sl = src.substr(ss - ls, ls);

			if (sf == first && sl == last)
			{
				return src.substr(fs, ss - ls - 1);
			}
		}

		return src;
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
	std::string	Str::Sprintf(const std::string& fmt, ...)
	{
		std::vector<char> buf(512);

		va_list args;
		va_start(args, fmt);
		{
#if defined(ML_SYSTEM_WINDOWS)
			vsnprintf_s(
				&buf[0],
				buf.size(),
				buf.size() + strlen(fmt.c_str()),
				fmt.c_str(),
				args);
#elif defined(ML_SYSTEM_LINUX)
			vsnprintf(&buf[0], buf.size(), fmt.c_str(), args);
#endif
		}
		va_end(args);
		return std::string(&buf[0]);
	}
	std::string	Str::ToLower(const std::string& src)
	{
		std::locale loc;

		std::string toStr = src;

		for (std::string::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::tolower(src[i], loc);
		}

		return toStr;
	}
	std::string	Str::ToUpper(const std::string& src)
	{
		std::locale loc;

		std::string toStr = src;

		for (std::string::size_type i = 0; i < src.length(); i++)
		{
			toStr[i] = std::toupper(src[i], loc);
		}

		return toStr;
	}

	std::vector<std::string> Str::Split(const std::string& src, const std::string& delims)
	{
		std::vector<std::string> list;

		std::string	cpy = src;
		uint	pos = 0;
		std::string	tok;

		while ((pos = cpy.find(delims)) != std::string::npos)
		{
			tok = cpy.substr(0, pos);

			list.push_back(tok);

			cpy.erase(0, pos + delims.length());
		}

		list.push_back(cpy);

		return list;
	}
	std::vector<std::string> Str::Split(const std::string& src, const std::vector<std::string>& delims)
	{
		std::vector<std::string> list;

		std::string dlmstr = "";
		for (std::string d : delims)
		{
			dlmstr += d;
		}

		std::stringstream stream(src);
		std::string line;

		while (std::getline(stream, line))
		{
			uint prev = 0, pos;
			while ((pos = line.find_first_of(dlmstr, prev)) != std::string::npos)
			{
				if (pos > prev)
				{
					list.push_back(line.substr(prev, pos - prev));
				}

				prev = pos + 1;
			}

			if (prev < line.length())
			{
				list.push_back(line.substr(prev, std::string::npos));
			}
		}

		return list;
	}

	bool	Str::Contains(const std::string& src, char c)
	{
		return (src.find(c) != std::string::npos);
	}
	bool	Str::IsAlpha(const std::string& src)
	{
		for (char c : src)
		{
			if (!isalpha(c))
			{
				return false;
			}
		}

		return true;
	}
	bool	Str::IsAlnum(const std::string & src)
	{
		for (char c : src)
		{
			if (!isalpha(c) || !isdigit(c))
			{
				return false;
			}
		}

		return true;
	}
	bool	Str::IsBool(const std::string& src)
	{
		const std::string low = ToLower(src);
		return
			low == "true" ||
			low == "false" ||
			low == "1" ||
			low == "0";
	}
	bool	Str::IsDecimal(const std::string& src)
	{
		if (!src.empty())
		{
			char* endptr = 0;
			strtod(src.c_str(), &endptr);
			if (*endptr != '\0' || endptr == src)
			{
				return false;
			}
			return true;
		}
		return false;
	}
	bool	Str::IsInt(const std::string& src)
	{
		if (!src.empty())
		{
			std::string::const_iterator it = src.begin();

			if (*it == '-')
				it++;

			while (it != src.end() && std::isdigit(*it))
				++it;

			return !src.empty() && it == src.end();
		}
		return false;
	}
	bool	Str::IsLower(const std::string & src)
	{
		for (auto c : src)
		{
			if (!std::islower(c))
				return false;
		}
		return true;
	}
	bool	Str::IsName(const std::string& src)
	{
		if (src.empty())
		{
			return false;
		}
		else if (src.size() == 1)
		{
			return IsAlpha(src);
		}
		else
		{
			if (isalpha(src[0]) || src[0] == '_')
			{
				for (uint i = 1; i < src.size(); i++)
				{
					if (src[i] == '_')
					{
						continue;
					}
					else if (!isalnum(src[i]))
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	bool	Str::IsNumber(const std::string& src)
	{
		return IsInt(src) || IsDecimal(src);
	}
	bool	Str::IsText(const std::string& src)
	{
		return IsWrap(src, '\"');
	}
	bool	Str::IsUpper(const std::string & src)
	{
		for (auto c : src)
		{
			if (!std::isupper(c))
				return false;
		}
		return true;
	}
	bool	Str::IsWrap(const std::string& src, char c)
	{
		return IsWrap(src, c, c);
	}
	bool	Str::IsWrap(const std::string& src, char front, char back)
	{
		return
			src.length() > 2 &&
			src.front() == front &&
			src.back() == back;
	}
	bool	Str::IsWrap(const std::string & src, const std::string & s)
	{
		return IsWrap(src, s, s);
	}
	bool	Str::IsWrap(const std::string & src, const std::string & front, const std::string & back)
	{
		if (src.length() >= (front.length() + back.length()))
		{
			return
				(src.substr(0, front.length()) == front) &&
				(src.substr(src.length() - back.length()) == back);
		}

		return false;
	}

	bool	Str::ToBool(const std::string& src)
	{
		return (src == "1" || ToLower(src) == "true");
	}
	int		Str::ToInt(const std::string& src)
	{
		return std::stoi(src);
	}
	double	Str::ToDecimal(const std::string& src)
	{
		return std::stod(src);
	}
	float	Str::ToFloat(const std::string & src)
	{
		return std::stof(src);
	}

	bool	Str::MakeBool(const std::string& src, bool & value)
	{
		if (IsBool(src))
		{
			value = ToBool(src);
			return true;
		}
		value = false;
		return false;
	}
	bool	Str::MakeInt(const std::string& src, int & value)
	{
		if (IsInt(src))
		{
			value = std::stoi(src);
			return true;
		}
		value = 0;
		return false;
	}
	bool	Str::MakeDecimal(const std::string& src, double & value)
	{
		if (IsDecimal(src))
		{
			value = std::stod(src);
			return true;
		}
		value = 0.0;
		return false;
	}
	bool	Str::MakeFloat(const std::string& src, float & value)
	{
		if (IsDecimal(src))
		{
			value = std::stof(src);
			return true;
		}
		value = 0.f;
		return false;
	}

}
