#include <MCP23017.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define WRITE_DELAY 100

namespace pi
{
	inline int reverseBits(int n)
	{
		int rev = 0;
		while (n)
		{
			rev <<= 1;
			if (n & 1)
				rev ^= 1;
			n >>= 1;
		}
		return rev;
	}


	MCP23017::MCP23017()
		: m_addr(-1)
		, m_handle(-1)
	{
	}

	MCP23017::MCP23017(int addr)
		: m_addr(addr)
		, m_handle(-1)
	{
	}

	MCP23017::MCP23017(const MCP23017 & copy)
		: m_addr(copy.m_addr)
		, m_handle(copy.m_handle)
	{
	}

	MCP23017::~MCP23017()
	{
	}


	bool MCP23017::setup()
	{
		if ((m_handle = wiringPiI2CSetup(m_addr)) >= 0)
		{
			return true;
		}
		return false;
	}


	MCP23017 & MCP23017::clearA()
	{
		return writeReg8(MCP23017::OLATA, 0);
	}

	MCP23017 & MCP23017::clearB()
	{
		return writeReg8(MCP23017::OLATB, 0);
	}

	MCP23017 & MCP23017::clearAB()
	{
		return clearA().clearB();
	}
	

	MCP23017 & MCP23017::writeA(int value)
	{
		if ((*this)[F_Reverse] == 1 || (*this)[F_Reverse] == 4)
		{
			return writeReg8(MCP23017::OLATA, reverseBits(value));
		}
		return writeReg8(MCP23017::OLATA, value);
	}
			 
	MCP23017 & MCP23017::writeB(int value)
	{
		if ((*this)[F_Reverse] == 2 || (*this)[F_Reverse] == 4)
		{
			return writeReg8(MCP23017::OLATB, reverseBits(value));
		}
		return writeReg8(MCP23017::OLATB, value);
	}

	MCP23017 & MCP23017::writeAB(int i)
	{
		return writeA((char)(i & 0xFF)).writeB((char)(((i >> 8) & 0xFF)));
	}
				 
	MCP23017 & MCP23017::writeReg8(int reg, int value)
	{
		wiringPiI2CWriteReg8(handle(), reg, static_cast<char>(value));
		delayMicroseconds(WRITE_DELAY);
		return (*this);
	}
}
