#ifndef _LiquidCrystal_I2C_H_
#define _LiquidCrystal_I2C_H_

#include <string>

namespace pi
{
	class LiquidCrystal_I2C
	{
	public:
		LiquidCrystal_I2C();
		LiquidCrystal_I2C(int address, int width, int height);
		LiquidCrystal_I2C(const LiquidCrystal_I2C& copy);
		~LiquidCrystal_I2C();

		bool setup();

		LiquidCrystal_I2C& blink(bool value);
		LiquidCrystal_I2C& clear();
		LiquidCrystal_I2C& cursor(bool value);
		LiquidCrystal_I2C& display(int value);
		LiquidCrystal_I2C& home();
		LiquidCrystal_I2C& modPos(int dx, int dy);
		LiquidCrystal_I2C& setPos(int x, int y);
		LiquidCrystal_I2C& setX(int value);
		LiquidCrystal_I2C& setY(int value);

		LiquidCrystal_I2C& print(char value);
		LiquidCrystal_I2C& print(const char* value);
		LiquidCrystal_I2C& print(const std::string& value);
		LiquidCrystal_I2C& printf(const char* fmt, ...);

		LiquidCrystal_I2C& sendByte(int value, int mode);
		LiquidCrystal_I2C& sendCommand(int value);
		LiquidCrystal_I2C& sendData(int value);

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

#endif // !_LiquidCrystal_I2C_H_
