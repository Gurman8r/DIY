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
		m_any = false;

		for (int i = 0; i < Button::MAX_BUTTON; i++)
		{
			if ((m_new[i] = digitalRead(i)) != 0)
			{
				m_any = true;
			}
		}
	}
	void Input::endStep()
	{
		for (int i = 0; i < Button::MAX_BUTTON; i++)
		{
			m_old[i] = m_new[i];
		}
	}

	bool Input::getButton(int index) const
	{
		return m_new[index];
	}
	bool Input::getButtonDown(int index) const
	{
		return m_new[index] && !m_old[index];
	}
	bool Input::getButtonUp(int index) const
	{
		return !m_new[index] && m_old[index];
	}
}