#include <LiquidCrystal.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdarg.h>

// Commands
#define	CMD_CLEAR		0x01 // 
#define	CMD_HOME		0x02 // 
#define	CMD_ENTRY		0x04 // 
#define	CMD_DISP_CTL	0x08 // 
#define	CMD_DISP_SHIFT	0x10 // 
#define	CMD_FUNC_SET	0x20 // 
#define	CMD_CG_RAM		0x40 // 
#define	CMD_DG_RAM		0x80 // 

// Entry register
#define	ENTRY_SHIFT		0x01 // 
#define	ENTRY_INC		0x02 // 

// Control register
#define	CTRL_BLINK		0x01 // 
#define	CTRL_CURSOR		0x02 // 
#define	CTRL_DISPAY		0x04 // 

// Function register
#define	FUNC_F			0x04 // 
#define	FUNC_N			0x08 // 
#define	FUNC_DL			0x10 // 
#define	FUNC_CDSHIFT_RL	0x04 // 

// Delays
#define DELAY_DISP		5

// Row offsets
static const int RowOff[4] = { 0x00, 0x40, 0x14, 0x54 };

namespace pi
{
	LiquidCrystal::LiquidCrystal()
		: m_address(-1)
		, m_handle(-1)
		, m_w(0)
		, m_h(0)
		, m_x(0)
		, m_y(0)
	{
	}
	
	LiquidCrystal::LiquidCrystal(int address, int getW, int getH)
		: m_address(address)
		, m_handle(-1)
		, m_w(getW)
		, m_h(getH)
		, m_x(0)
		, m_y(0)
	{
	}
	
	LiquidCrystal::LiquidCrystal(const LiquidCrystal & copy)
		: m_address(copy.m_address)
		, m_handle(copy.m_handle)
		, m_w(copy.m_w)
		, m_h(copy.m_h)
		, m_x(copy.m_x)
		, m_y(copy.m_y)
	{
	}
	
	LiquidCrystal::~LiquidCrystal()
	{
	}


	bool LiquidCrystal::setup()
	{
		if ((m_handle = wiringPiI2CSetup(m_address)) >= 0)
		{
			sendCommand(0x33);
			sendCommand(0x32);
			sendCommand(0x06);

			//display(0);
			//delay(10);
			//display(1);
			
			clear();
			home();
			
			return true;
		}
		return false;
	}


	LiquidCrystal & LiquidCrystal::blink(bool value)
	{
		if (value)
		{
			return sendCommand(0x0F);
		}
		else
		{
			return sendCommand(0x0C);
		}
	}

	LiquidCrystal & LiquidCrystal::clear()
	{
		return sendCommand(CMD_CLEAR);
	}
	
	LiquidCrystal & LiquidCrystal::display(int value)
	{
		delay(DELAY_DISP);

		wiringPiI2CReadReg8(m_handle, (value | CTRL_DISPAY));

		delay(DELAY_DISP);

		wiringPiI2CReadReg8(m_handle, (value & ~CTRL_DISPAY));

		return (*this);
	}
	
	LiquidCrystal & LiquidCrystal::home()
	{
		return sendCommand(CMD_HOME);
	}

	LiquidCrystal & LiquidCrystal::moveCursor(int dx, int dy)
	{
		return setCursor(getX() + dx, getY() + dy);
	}

	LiquidCrystal & LiquidCrystal::setCursor(int x, int y)
	{
		if (x >= 0 && x < m_w)
		{
			m_x = x;
		}
		else if (x < 0)
		{
			x = 0;
		}
		else
		{
			x = m_w - 1;
		}

		if (y >= 0 && y < m_h)
		{
			m_y = y;
		}
		else if (y < 0)
		{
			m_y = 0;
		}
		else
		{
			m_y = m_h - 1;
		}

		return sendCommand(m_x + (CMD_DG_RAM | RowOff[m_y]));
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
		int bitsHi = mode | (value & 0xF0) | CMD_DISP_CTL;
		
		wiringPiI2CReadReg8(m_handle, bitsHi);
		
		display(bitsHi);


		// Low bits
		int bitsLo = mode | ((value << 4) & 0xF0) | CMD_DISP_CTL;
		
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
}