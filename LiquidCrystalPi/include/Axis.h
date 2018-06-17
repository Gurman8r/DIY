#ifndef _AXIS_H_
#define _AXIS_H_

#include <string>

namespace pi
{
	class Axis
	{
	public:
		Axis();
		Axis(const std::string& name, int mode, int pin, int pud);
		Axis(const Axis& copy);
		~Axis();

		inline const std::string& getName() const
		{
			return m_name;
		}
		inline const int& getMode() const
		{
			return m_mode;
		}
		inline const int& getPin() const
		{
			return m_pin;
		}
		inline const int& getPud() const
		{
			return m_pud;
		}

	private:
		std::string m_name;
		int m_mode;
		int m_pin;
		int m_pud;
	};
}

#endif // !_AXIS_H_
