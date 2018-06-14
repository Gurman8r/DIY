#include <Timer.h>

namespace pi
{
	Timer::Timer()
		: m_paused(true)
	{
	}

	Timer::Timer(const Timer & copy)
		: m_paused(copy.m_paused)
		, m_elapsed(copy.m_elapsed)
		, m_clock(copy.m_clock)
		, m_min(copy.m_min)
		, m_max(copy.m_max)
	{
	}

	Timer::~Timer()
	{
	}


	Timer& Timer::start()
	{
		m_min = m_clock.now();
		m_max = m_clock.now();
		m_elapsed = TimePoint();
		return pause(false);
	}

	Timer& Timer::reset()
	{
		return stop().start();
	}

	Timer& Timer::stop()
	{
		m_max = m_clock.now();
		m_elapsed = std::chrono::duration_cast<TimePoint::millis_t>(m_max - m_min);
		return (*this);
	}

	Timer& Timer::pause(bool pause)
	{
		if (pause && !m_paused)
		{
			m_paused = true;

			m_max = m_clock.now();

			m_elapsed = std::chrono::duration_cast<TimePoint::millis_t>(m_max - m_min);
		}
		else if (!pause && m_paused)
		{
			m_paused = false;

			m_min = m_clock.now();
		}

		return (*this);
	}


	TimePoint Timer::elapsed() const
	{
		if (!m_paused)
		{
			return std::chrono::duration_cast<TimePoint::millis_t>(m_clock.now() - m_min);
		}
		return m_elapsed;
	}

	bool Timer::paused() const
	{
		return m_paused;
	}
}