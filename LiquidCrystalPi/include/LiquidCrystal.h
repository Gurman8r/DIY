#ifndef _LIQUID_CRYSTAL_H_
#define _LIQUID_CRYSTAL_H_

#include <string>

namespace pi
{
	class LiquidCrystal
	{
	public:
		LiquidCrystal();
		LiquidCrystal(int address, int width, int height);
		LiquidCrystal(const LiquidCrystal& copy);
		~LiquidCrystal();

		bool setup();

		LiquidCrystal& blink(bool value);
		LiquidCrystal& clear();
		LiquidCrystal& display(int value);
		LiquidCrystal& home();
		LiquidCrystal& moveCursor(int dx, int dy);
		LiquidCrystal& setCursor(int x, int y);

		LiquidCrystal& print(char value);
		LiquidCrystal& print(const char* value);
		LiquidCrystal& print(const std::string& value);
		LiquidCrystal& printf(const char* fmt, ...);

		LiquidCrystal& sendByte(int value, int mode);
		LiquidCrystal& sendCommand(int value);
		LiquidCrystal& sendData(int value);

		inline int	address()	const { return m_address; }
		inline int	handle()	const { return m_handle; }
		inline int	getW()		const { return m_w; }
		inline int	getH()		const { return m_h; }
		inline int	getX()		const { return m_x; }
		inline int	getY()		const { return m_y; }

	private:
		int	m_address;
		int	m_handle;
		int	m_w, m_h;
		int	m_x, m_y;
	};
}

#endif
