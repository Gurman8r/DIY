#ifndef _MCP23017_H_
#define _MCP23017_H_

// * * * * * * * * * * * * * * * * * * //

namespace pi
{
	class MCP23017
	{
	public:
		enum Register
		{
			IODIRA		= 0x00, //	A | IO Direction
			IODIRB		= 0x01, //	B | IO Direction
			IPOLA		= 0x02, //	A | Pin Inversion
			IPOLB		= 0x03, //	B | Pin Inversion
			GPINTENA	= 0x04, //	A | 
			GPINTENB	= 0x05, //	B | 
			DEFVALA		= 0x06, //	A | Default Value
			DEFVALB		= 0x07, //	B | Default Value
			INTCONA		= 0x08, //	A | Interrupt Condition
			INTCONB		= 0x09, //	B | Interrupt Condition
			IOCONA		= 0x0A, //	A | 
			IOCONB		= 0x0B, //	B | 
			GPPUA		= 0x0C, //	A | Pullup
			GPPUB		= 0x0D, //	B | Pullup
			INTFA		= 0x0E, //	A | Interrupt Flag
			INTFB		= 0x0F, //	B | Interrupt Flag
			INTCAPA		= 0x10, //	A | Interrupt Data Capture
			INTCAPB		= 0x11, //	B | Interrupt Data Capture
			GPIOA		= 0x12, //	A | 
			GPIOB		= 0x13, //	B | 
			OLATA		= 0x14, //	A | Output Latch
			OLATB		= 0x15, //	B | Output Latch
			MAX_REGISTER
		};
		enum Flag
		{
			F_Reverse,
			MAX_FLAG
		};
	public:
		MCP23017();
		MCP23017(int addr);
		MCP23017(const MCP23017& copy);
		~MCP23017();

		bool setup();

		MCP23017& clearA();
		MCP23017& clearB();
		MCP23017& clearAB();

		MCP23017& writeA(int value);
		MCP23017& writeB(int value);
		MCP23017& writeAB(int value);
		MCP23017& writeReg8(int reg, int value);

		inline int addr()	const { return m_addr; };
		inline int handle() const { return m_handle; };

		inline const int& operator[](unsigned int index) const
		{
			return m_flags[index];
		}
		inline int& operator[](unsigned int index)
		{
			return m_flags[index];
		}

	private:
		int m_addr;
		int m_handle;
		int m_flags[MAX_FLAG];
	};
}

#endif // !_MCP23017_H_
