#include <Axis.h>

namespace pi
{
	Axis::Axis()
		: m_name(std::string())
		, m_mode(0)
		, m_pin(-1)
		, m_pud(0)
	{
	}

	Axis::Axis(const std::string & name, int mode, int pin, int pud)
		: m_name(name)
		, m_mode(mode)
		, m_pin(pin)
		, m_pud(pud)
	{
	}

	Axis::Axis(const Axis & copy)
		: m_name(copy.m_name)
		, m_mode(copy.m_mode)
		, m_pin(copy.m_pin)
		, m_pud(copy.m_pud)
	{
	}

	Axis::~Axis()
	{
	}
}
