#include <TimePoint.h>
#include <inttypes.h>

namespace pi
{
	TimePoint::TimePoint()
		: m_millis(0)
	{
	}
	TimePoint::TimePoint(int64_t milliseconds)
		: m_millis(milliseconds)
	{
	}
	TimePoint::TimePoint(const std::chrono::duration<int64_t>& time)
		: m_millis(time.count())
	{
	}
	TimePoint::TimePoint(const millis_t & milliseconds)
		: m_millis(milliseconds.count())
	{
	}
	TimePoint::TimePoint(const TimePoint & copy)
		: m_millis(copy.m_millis)
	{
	}
	TimePoint::~TimePoint()
	{
	}


	int64_t TimePoint::nanos() const
	{
		return millis() * 1000000;
	}
	int64_t	TimePoint::millis() const
	{
		return m_millis;
	}
	int64_t	TimePoint::seconds() const
	{
		return millis() / 1000;
	}
	int64_t	TimePoint::minutes() const
	{
		return seconds() / 60;
	}
	int64_t	TimePoint::hours() const
	{
		return hours() / 24;
	}


	TimePoint	operator+(const TimePoint & lhs, const TimePoint & rhs)
	{
		return TimePoint(lhs.millis() + rhs.millis());
	}
	TimePoint	operator-(const TimePoint & lhs, const TimePoint & rhs)
	{
		return TimePoint(lhs.millis() - rhs.millis());
	}
	TimePoint & operator+=(TimePoint & lhs, const TimePoint & rhs)
	{
		lhs = (lhs + rhs);
		return lhs;
	}
	TimePoint & operator-=(TimePoint & lhs, const TimePoint & rhs)
	{
		lhs = (lhs - rhs);
		return lhs;
	}
}