#ifndef _LIQUID_CRYSTAL_H_
#define _LIQUID_CRYSTAL_H_

#include "../include/Config.h"
#include <string>

namespace pi
{
	class LiquidCrystal
	{
	public:
		LiquidCrystal();
		LiquidCrystal(int handle, uint cols, uint rows);
		LiquidCrystal(const LiquidCrystal& copy);
		~LiquidCrystal();

		bool setup();

		LiquidCrystal& clear();
		LiquidCrystal& cursor(bool value);
		LiquidCrystal& cursorBlink(bool value);
		LiquidCrystal& display(int value);

		LiquidCrystal& print(char value);
		LiquidCrystal& print(const char* value);
		LiquidCrystal& print(const std::string& value);
		LiquidCrystal& printf(const char* fmt, ...);

		LiquidCrystal& sendByte(int value, int mode);
		LiquidCrystal& sendCommand(int value);
		LiquidCrystal& sendData(int value);

		LiquidCrystal& setPos(uint x, uint y);
		LiquidCrystal& setX(uint value);
		LiquidCrystal& setY(uint value);

		inline int	handle()	const { return m_handle; }
		inline int	control()	const { return m_control; }
		inline uint cols()		const { return m_cols; }
		inline uint rows()		const { return m_rows; }
		inline uint x()			const { return m_x; }
		inline uint y()			const { return m_y; }

	private:
		int		m_handle;
		int		m_control;
		uint	m_cols;
		uint	m_rows;
		uint	m_x;
		uint	m_y;
	};
}

#endif