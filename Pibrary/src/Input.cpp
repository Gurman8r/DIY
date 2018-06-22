#include <Input.h>
#include <wiringPi.h>
#include <inttypes.h>

namespace pi
{
	Input::Input()
	{
	}
	Input::~Input()
	{
	}

	void Input::setPin(int pin, int mode, int pud)
	{
		pinMode(pin, mode);
		pullUpDnControl(pin, pud);
	}

	void Input::beginStep()
	{
		m_state = 0;
		m_any = false;

		for (int i = 0; i < Pin::MAX_PIN; i++)
		{
			if ((m_new[i] = digitalRead(i)) != 0)
			{
				m_any = true;

				m_state |= 1UL << i;
			}
		}
	}
	void Input::endStep()
	{
		for (int i = 0; i < Pin::MAX_PIN; i++)
		{
			m_old[i] = m_new[i];
		}
	}

	void Input::capture()
	{
		beginStep();
		delayMicroseconds(500);
		endStep();
	}

	bool Input::getPin(int index) const
	{
		return m_new[index];
	}
	bool Input::getPinOn(int index) const
	{
		return m_new[index] && !m_old[index];
	}
	bool Input::getPinOff(int index) const
	{
		return !m_new[index] && m_old[index];
	}
}
