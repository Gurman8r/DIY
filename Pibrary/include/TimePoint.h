#ifndef _TIME_POINT_H_
#define _TIME_POINT_H_

#include <chrono>

namespace pi
{
	class TimePoint
	{
	public:
		using millis_t = std::chrono::milliseconds;

	public:
		TimePoint();
		TimePoint(int64_t milliseconds);
		TimePoint(const std::chrono::duration<int64_t>& time);
		TimePoint(const millis_t& milliseconds);
		TimePoint(const TimePoint& copy);
		~TimePoint();

		int64_t nanos() const;
		int64_t	millis() const;
		int64_t	seconds() const;
		int64_t	minutes() const;
		int64_t	hours() const;

	public:
		friend TimePoint	operator+(const TimePoint& lhs, const TimePoint& rhs);
		friend TimePoint	operator-(const TimePoint& lhs, const TimePoint& rhs);
		friend TimePoint&	operator+=(TimePoint& lhs, const TimePoint& rhs);
		friend TimePoint&	operator-=(TimePoint& lhs, const TimePoint& rhs);

	private:
		int64_t m_millis;
	};
}

#endif // !_TIME_POINT_H_
