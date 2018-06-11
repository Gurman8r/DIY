#include "../include/LiquidCrystal.h"
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdarg.h>

// HD44780U Commands
#define	LCD_CLEAR		0x01 // 
#define	LCD_HOME		0x02 // 
#define	LCD_ENTRY		0x04 // 
#define	LCD_CTRL		0x08 // 
#define	LCD_CDSHIFT		0x10 // 
#define	LCD_FUNC		0x20 // 
#define	LCD_CGRAM		0x40 // 
#define	LCD_DGRAM		0x80 // 

// Bits in the entry register
#define	LCD_ENTRY_SH	0x01 // 
#define	LCD_ENTRY_ID	0x02 // 

// Bits in the control register
#define	LCD_BLINK_CTRL	0x01 // 
#define	LCD_CURSOR_CTRL	0x02 // 
#define	LCD_DISP_CTRL	0x04 // 

// Bits in the function register
#define	LCD_FUNC_F		0x04 // 
#define	LCD_FUNC_N		0x08 // 
#define	LCD_FUNC_DL		0x10 // 
#define	LCD_CDSHIFT_RL	0x04 // 

namespace pi
{
	LiquidCrystal::LiquidCrystal()
		: m_handle(-1)
		, m_control(0)
		, m_cols(0)
		, m_rows(0)
		, m_x(0)
		, m_y(0)
	{
	}
	LiquidCrystal::LiquidCrystal(int handle, uint cols, uint rows)
		: m_handle(handle)
		, m_control(0)
		, m_cols(cols)
		, m_rows(rows)
		, m_x(0)
		, m_y(0)
	{
	}
	LiquidCrystal::LiquidCrystal(const LiquidCrystal & copy)
		: m_handle(copy.m_handle)
		, m_control(copy.m_control)
		, m_cols(copy.m_cols)
		, m_rows(copy.m_rows)
		, m_x(copy.m_x)
		, m_y(copy.m_y)
	{
	}
	LiquidCrystal::~LiquidCrystal()
	{
	}

	bool LiquidCrystal::setup()
	{
		if (m_handle >= 0)
		{
			if (wiringPiI2CSetup(m_handle) >= 0)
			{
				return true;
			}
		}

		return false;
	}

	LiquidCrystal & LiquidCrystal::clear()
	{
		return sendCommand(LCD_CLEAR).sendCommand(LCD_HOME).setPos(0, 0);
	}
	LiquidCrystal & LiquidCrystal::cursor(bool value)
	{
		if (value)
		{
			m_control |= LCD_CURSOR_CTRL;
		}
		else
		{
			m_control &= ~LCD_CURSOR_CTRL;
		}

		return sendCommand(LCD_CTRL | m_control);
	}
	LiquidCrystal & LiquidCrystal::cursorBlink(bool value)
	{
		if (value)
		{
			m_control |= LCD_BLINK_CTRL;
		}
		else
		{
			m_control &= ~LCD_BLINK_CTRL;
		}

		return sendCommand(LCD_CTRL | m_control);
	}
	LiquidCrystal & LiquidCrystal::display(int value)
	{
		delayMicroseconds(500);

		wiringPiI2CReadReg8(m_handle, (value | LCD_DISP_CTRL));

		delayMicroseconds(500);

		wiringPiI2CReadReg8(m_handle, (value & ~LCD_DISP_CTRL));

		return (*this);
	}
	
	LiquidCrystal & LiquidCrystal::print(char value)
	{
		return sendData(value);
	}
	LiquidCrystal & LiquidCrystal::print(const char * value)
	{
		while (*value)
		{
			print(*(value++));
		}
		return (*this);
	}
	LiquidCrystal & LiquidCrystal::print(const std::string & value)
	{
		return print(value.c_str());
	}
	LiquidCrystal & LiquidCrystal::printf(const char * fmt, ...)
	{
		va_list argp;
		char buffer[1024];

		va_start(argp, fmt);
		vsnprintf(buffer, 1023, fmt, argp);
		va_end(argp);

		return print(buffer);
	}
	
	LiquidCrystal & LiquidCrystal::sendByte(int value, int mode)
	{
		// High bits
		int bitsHi = mode | (value & 0xF0) | LCD_CTRL;
		wiringPiI2CReadReg8(m_handle, bitsHi);
		display(bitsHi);

		// Low bits
		int bitsLo = mode | ((value << 4) & 0xF0) | LCD_CTRL;
		wiringPiI2CReadReg8(m_handle, bitsLo);
		display(bitsLo);

		return (*this);
	}
	LiquidCrystal & LiquidCrystal::sendCommand(int value)
	{
		return sendByte(value, 0);
	}
	LiquidCrystal & LiquidCrystal::sendData(int value)
	{
		return sendByte(value, 1);
	}
	
	LiquidCrystal & LiquidCrystal::setPos(uint x, uint y)
	{
		return setX(x).setY(y);
	}
	LiquidCrystal & LiquidCrystal::setX(uint value)
	{
		m_x = value;
		return (*this);
	}
	LiquidCrystal & LiquidCrystal::setY(uint value)
	{
		m_y = value;
		return (*this);
	}
}