#include <OLED.h>
#include <stdio.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <OLED_Fonts.h>

#define OLED_ROT 0
#define OLED_swap(a, b) { int t = a; a = b; b = t; }

namespace pi
{
	OLED::OLED()
	{
	}

	OLED::OLED(int vcc, int address, int width, int height)
		: m_vcc(vcc)
		, m_address(address)
		, m_w(width)
		, m_h(height)
	{
	}

	OLED::~OLED()
	{
	}

	
	bool OLED::setup()
	{
		if ((m_handle = wiringPiI2CSetup(m_address)) >= 0)
		{
			sendCommand(OLED_DISP_OFF);
			sendCommand(OLED_SET_DISP_CLOCK_DIV);
			sendCommand(0x80); 	// the suggested ratio 0x80

			sendCommand(OLED_SET_MULTIPLEX);
			sendCommand(m_h - 1);

			sendCommand(OLED_SET_DISP_OFFSET);
			sendCommand(0x0);
			sendCommand(OLED_SET_START_LINE | 0x0);
			sendCommand(OLED_CHARGE_PUMP);
			
			if (m_vcc == OLED_VCC_EXTERNAL)
			{
				sendCommand(0x10);
			}
			else
			{
				sendCommand(0x14);
			}

			sendCommand(OLED_MEMORY_MODE);
			sendCommand(0x00); // 0x0 act like ks0108
			sendCommand(OLED_SEG_REMAP | 0x1);
			sendCommand(OLED_COM_SCAN_DEC);

#if defined(OLED_128x64)
			sendCommand(OLED_SET_COM_PINS);
			sendCommand(0x12);
			sendCommand(OLED_SET_CONTRAST);
			if (m_vcc == OLED_VCC_EXTERNAL)
			{
				sendCommand(0x9F);
			}
			else
			{
				sendCommand(0xCF);
			}
#elif defined(OLED_128x32)
			sendCommand(OLED_SET_COM_PINS);
			sendCommand(0x02);
			sendCommand(OLED_SET_CONTRAST);
			sendCommand(0x8F);
#elif defined(OLED_96x16)
			sendCommand(OLED_SET_COM_PINS);
			sendCommand(0x02);
			sendCommand(OLED_SET_CONTRAST);
			if (m_vcc == OLED_VCC_EXTERNAL)
			{
				sendCommand(0x10);
			}
			else
			{
				sendCommand(0xAF);
			}
#endif
			sendCommand(OLED_SET_PRECHARGE);

			if (m_vcc == OLED_VCC_EXTERNAL)
			{
				sendCommand(0x22);
			}
			else
			{
				sendCommand(0xF1);
			}

			sendCommand(OLED_SET_VCOM_DETECT);
			sendCommand(0x40);
			sendCommand(OLED_DISP_ALL_ON_RESUME);
			sendCommand(OLED_DISP_NORMAL);
			sendCommand(OLED_SCROLL_DEACTIVATE);
			sendCommand(OLED_DISP_ON);

			return true;
		}
		return false;
	}


	OLED & OLED::clear()
	{
		memset(m_buffer, 0, bufSize() * sizeof(int));

		return setCursor(0, 0);
	}

	OLED & OLED::dim(uint value)
	{
		if (value)
		{
			m_contrast = 0; // Dimmed display
		}
		else
		{
			if (m_vcc == OLED_VCC_EXTERNAL)
			{
				m_contrast = 0x9F;
			}
			else
			{
				m_contrast = 0xCF;
			}
		}
		// the range of contrast to too small to be really useful
		// it is useful to dim the display
		return sendCommand(OLED_SET_CONTRAST).sendCommand(m_contrast);
	}

	OLED & OLED::display()
	{
		sendCommand(OLED_COLUMN_ADDR);
		sendCommand(0); // Column start address (0 = reset)
		sendCommand(m_w - 1);

		sendCommand(OLED_PAGE_ADDR);
		sendCommand(0); // Page start address (0 = reset)

		sendCommand(7);

		for (int i = 0, imax = bufSize(); i < imax; i++)
		{
			//This sends byte by byte. 
			//Better to send all buffer without 0x40 first
			//Should be optimized
			wiringPiI2CWriteReg8(m_handle, 0x40, m_buffer[i]);
		}

		return (*this);
	}

	OLED & OLED::drawChar(int x, int y, byte value, byte color, int size)
	{
		if ((x >= m_w) || 
			(y >= m_h) ||
			((x + 6 * size - 1) < 0) ||
			((y + 8 * size - 1) < 0))
		{
			return (*this);
		}

		for (int i = 0; i < 6; i++)
		{
			int line = (i == 5) ? 0x0 : (*(const unsigned char *)((OLED_font + (value * 5) + i)));

			for (int j = 0; j < 8; j++)
			{
				if (line & 0x1)
				{
					if (size == 1)
					{
						drawPixel(x + i, y + j, color);
					}
					else
					{
						drawFillRect(x + (i + size), y + (j * size), size, size, color);
					}
				}
				line >>= 1;
			}
		}

		return (*this);
	}
	
	OLED & OLED::drawFastHLine(int x, int y, int w, byte color)
	{
		return (*this);
	}
	
	OLED & OLED::drawFastVLine(int x, int y, int h, byte color)
	{
		return (*this);
	}
	
	OLED & OLED::drawFillRect(int x, int y, int w, int h, byte color)
	{
		if (x >= m_w || y >= m_h)
		{
			return (*this);
		}

		if (y + h > m_h)
		{
			h = m_h - y - 1;
		}

		if (x + w > m_w)
		{
			w = m_w - x - 1;
		}

		switch (OLED_ROT)
		{
		case 0:
			break;
		case 1:
			OLED_swap(x, y);
			x = m_w - x - 1;
			break;
		case 2:
			x = m_w - x - 1;
			y = m_h - y - 1;
			break;
		case 3:
			OLED_swap(x, y);
			y = m_h - y - 1;
			break;
		}

		for (int i = 0; i < h; i++)
		{
			drawFastHLine(x, y + i, w, color);
		}

		return (*this);
	}
	
	OLED & OLED::drawPixel(int x, int y, byte color)
	{
		if ((x < 0) || (x >= m_w) || (y < 0) || (y >= m_h))
		{
			return (*this);
		}

		switch (OLED_ROT)
		{
		case 0:
			// 0 degree rotation
			// do nothing
			break;
		case 1:
			// 90 degree rotation
			// swap x & y for rotation, then invert x
			OLED_swap(x, y);
			x = m_w - x - 1;
			break;
		case 2:
			// 180 degree rotation
			// invert x and y - then shift y around for height.
			x = m_w - x - 1;
			y = m_h - y - 1;
			break;
		case 3:
			// 270 degree rotation
			// swap x & y for rotation, then invert y and adjust y for w (not to become h)
			OLED_swap(x, y);
			y = m_h - y - 1;
			break;
		}

		switch (color)
		{
		case 0: // Black
			m_buffer[x + (y / 8) * m_w] &= ~(1 << (y & 7));
			break;
		case 1: // White
			m_buffer[x + (y / 8) * m_w] |= (1 << (y & 7));
			break;
		case 2: // Inverse
			m_buffer[x + (y / 8) * m_w] ^= (1 << (y & 7));
			break;
		}

		return (*this);
	}
	
	OLED & OLED::drawRect(int x, int y, int w, int h, byte color)
	{
		return (*this);
	}
	
	OLED & OLED::drawString(const char * value)
	{
		for (int i = 0, imax = strlen(value); i < imax; i++)
		{
			const char c = value[i];

			switch (c)
			{
			case '\n':
				m_x = 0;
				m_y += m_textSize * 8;
				break;
			case '\r':
				// skip
				break;
			default:
				drawChar(m_x, m_y, (unsigned)c, 1, m_textSize);
				m_x += m_textSize * 6;
				if (m_wrap && (m_x > (m_w - m_textSize * 6)))
				{
					m_x = 0;
					m_y += m_textSize * 8;
				}
				break;
			}
		}
		return (*this);
	}
	
	OLED & OLED::drawString(const std::string & value)
	{
		return drawString(value.c_str());
	}
	
	OLED & OLED::invert(bool value)
	{
		if (value)
		{
			return sendCommand(OLED_DISP_INVERT);
		}
		else
		{
			return sendCommand(OLED_DISP_NORMAL);
		}
	}
	
	OLED & OLED::sendCommand(uint value)
	{
		uint control = 0x00;	// Co = 0, D/C = 0
		wiringPiI2CWriteReg8(m_handle, control, value);
		return (*this);
	}

	OLED & OLED::setCursor(int xv, int yv)
	{
		return setX(xv).setY(yv);
	}
	
	OLED & OLED::setX(int value)
	{
		m_x = value;
		return (*this);
	}

	OLED & OLED::setY(int value)
	{
		m_y = value;
		return (*this);
	}
	
	OLED & OLED::setTextSize(int value)
	{
		m_textSize = (value > 0) ? value : 1;
		return (*this);
	}
}
