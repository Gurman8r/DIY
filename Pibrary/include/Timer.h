#ifndef _TIMER_H_
#define _TIMER_H_

#include <TimePoint.h>

namespace pi
{
	class Timer
	{
	public:
		Timer();
		Timer(const Timer& copy);
		~Timer();

		Timer& pause(bool pause);
		Timer& reset();
		Timer& start();
		Timer& stop();

		TimePoint	elapsed() const;
		bool		paused() const;

	private:
		bool		m_paused;
		TimePoint	m_elapsed;

		std::chrono::high_resolution_clock			m_clock;
		std::chrono::_V2::system_clock::time_point	m_min;
		std::chrono::_V2::system_clock::time_point	m_max;
	};
}

#endif // !_TIMER_H_
