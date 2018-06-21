/*
	Sources:
	https://mil.ufl.edu/3744/docs/lcdmanual/commands.html
*/

#include <LiquidCrystal_I2C.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdarg.h>

/*
	Clear Display
	RS  R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
	=== === === === === === === === === ===
	0   0   0   0   0   0   0   0   0   1
	Clears all display and returns the cursor to the home position (Address 0).
*/

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
#define DELAY_DISP		500

// Options
#define OPT_Increment		0x02     
#define OPT_Display_Shift	0x01 
#define OPT_Enable_Display	0x04
#define OPT_Enable_Cursor	0x02 
#define OPT_Enable_Blink	0x01  
//#define OPT_Display_Shift	0x08 
#define OPT_Shift_Right		0x04
#define OPT_2_Lines			0x08
#define OPT_5x10_Dots		0x04

// Row offsets
static const int RowOff[4] = { 0x00, 0x40, 0x14, 0x54 };

namespace pi
{
	LiquidCrystal_I2C::LiquidCrystal_I2C()
		: m_address(-1)
		, m_handle(-1)
		, m_w(0)
		, m_h(0)
		, m_x(0)
		, m_y(0)
	{
	}

	LiquidCrystal_I2C::LiquidCrystal_I2C(int address, int getW, int getH)
		: m_address(address)
		, m_handle(-1)
		, m_w(getW)
		, m_h(getH)
		, m_x(0)
		, m_y(0)
	{
	}

	LiquidCrystal_I2C::LiquidCrystal_I2C(const LiquidCrystal_I2C & copy)
		: m_address(copy.m_address)
		, m_handle(copy.m_handle)
		, m_w(copy.m_w)
		, m_h(copy.m_h)
		, m_x(copy.m_x)
		, m_y(copy.m_y)
	{
	}

	LiquidCrystal_I2C::~LiquidCrystal_I2C()
	{
	}


	bool LiquidCrystal_I2C::setup()
	{
		if ((m_handle = wiringPiI2CSetup(m_address)) >= 0)
		{
			//sendCommand(CMD_FUNC_SET | OPT_2_Lines);
			//sendCommand(CMD_DISP_CTL | OPT_Enable_Display | OPT_Enable_Cursor);
			//sendCommand(CMD_CLEAR);
			//sendCommand(CMD_ENTRY | OPT_Increment | OPT_Display_Shift);

			display(0);
			delay(10);
			display(1);

			clear();
			home();

			return true;
		}
		return false;
	}


	LiquidCrystal_I2C & LiquidCrystal_I2C::blink(bool value)
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

	LiquidCrystal_I2C & LiquidCrystal_I2C::clear()
	{
		return sendCommand(CMD_CLEAR);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::cursor(bool value)
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

	LiquidCrystal_I2C & LiquidCrystal_I2C::display(int value)
	{
		delayMicroseconds(DELAY_DISP);

		wiringPiI2CReadReg8(m_handle, (value | CTRL_DISPAY));

		delayMicroseconds(DELAY_DISP);

		wiringPiI2CReadReg8(m_handle, (value & ~CTRL_DISPAY));

		return (*this);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::home()
	{
		return sendCommand(CMD_HOME);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::modPos(int dx, int dy)
	{
		return setPos(getX() + dx, getY() + dy);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::setPos(int x, int y)
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

	LiquidCrystal_I2C & LiquidCrystal_I2C::setX(int value)
	{
		return setPos(value, getY());
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::setY(int value)
	{
		return setPos(getX(), value);
	}


	LiquidCrystal_I2C & LiquidCrystal_I2C::print(char value)
	{
		return sendData(value);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::print(const char * value)
	{
		while (*value)
		{
			print(*(value++));
		}
		return (*this);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::print(const std::string & value)
	{
		return print(value.c_str());
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::printf(const char * fmt, ...)
	{
		va_list argp;
		char buffer[1024];

		va_start(argp, fmt);
		vsnprintf(buffer, 1023, fmt, argp);
		va_end(argp);

		return print(buffer);
	}


	LiquidCrystal_I2C & LiquidCrystal_I2C::sendByte(int value, int mode)
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

	LiquidCrystal_I2C & LiquidCrystal_I2C::sendCommand(int value)
	{
		return sendByte(value, 0);
	}

	LiquidCrystal_I2C & LiquidCrystal_I2C::sendData(int value)
	{
		return sendByte(value, 1);
	}
}
