#ifndef _OLED_H_
#define _OLED_H_

#include <string>
#include <inttypes.h>
#include <OLED_Commands.h>

// * * * * * * * * * * * * * * //

#define OLED_128x64
//#define OLED_128x32
//#define OLED_96x16

// * * * * * * * * * * * * * * //

#if defined(OLED_128x64)
#define OLED_W 128
#define OLED_H 64
#elif defined(OLED_128x32)
#define OLED_W 128
#define OLED_H 32
#elif defined(OLED_96x16)
#define OLED_W 96
#define OLED_H 16
#endif 

// * * * * * * * * * * * * * * //

namespace pi
{
	class OLED
	{
	public:
		using byte = uint8_t;
		using uint = uint32_t;

	public:
		OLED();
		OLED(int vcc, int address, int width, int height);
		~OLED();

		bool setup();

		OLED& clear();
		OLED& dim(uint value);
		OLED& display();
		OLED& drawChar(int x, int y, byte value, byte color, int size);
		OLED& drawFastHLine(int x, int y, int w, byte color);
		OLED& drawFastVLine(int x, int y, int h, byte color);
		OLED& drawFillRect(int x, int y, int w, int h, byte color);
		OLED& drawPixel(int x, int y, byte color);
		OLED& drawRect(int x, int y, int w, int h, byte color);
		OLED& drawString(const char* value);
		OLED& drawString(const std::string& value);
		OLED& invert(bool value);
		OLED& sendCommand(uint value);
		OLED& reset();
		OLED& setCursor(int xv, int yv);
		OLED& setX(int value);
		OLED& setY(int value);
		OLED& setTextSize(int value);

		inline int bufSize() const
		{
			return (m_w * m_h / 8);
		}
		inline int getW() const
		{
			return m_w;
		}
		inline int getH() const
		{
			return m_h;
		}

	private:
		int	m_vcc;
		int	m_address;
		int m_w;
		int m_h;

		int m_buffer[OLED_W * OLED_H / 8];
		int m_handle;
		int m_textSize;
		int m_wrap;
		int m_contrast;
		int m_x;
		int m_y;
	};
}

#endif // !_OLED_H_
