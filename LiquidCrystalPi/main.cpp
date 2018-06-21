#include <wiringPi.h>
#include <iostream>

#include <FileSystem.h>
#include <INIReader.h>
#include <Input.h>
#include <LiquidCrystal.h>
#include <Net.h>
#include <OLED.h>
#include <ssd1306_i2c.h>
#include <StringUtilty.h>
#include <Timer.h>

using namespace pi;

// * * * * * * * * * * * * * * //

#define CONFIG_PATH "../../../data/config.ini"

#define PIN_U 0
#define PIN_D 1
#define PIN_L 2
#define PIN_R 3
#define PIN_Y 4
#define PIN_N 5

int version;
int loadSpd;
int mode;

FileSystem		fs;
Input			input;
LiquidCrystal	lcd(0x3f, 16, 2);
LiquidCrystal	display(0x20, 16, 2);
OLED			oled(OLED_VCC_SWITCHCAP, 0x3C, 128, 64);

// * * * * * * * * * * * * * * //

void showLoading()
{
	lcd.clear();
	if (loadSpd > 0)
	{
		lcd.print("Loading...");
		lcd.setCursor(0, 1);
		for (int i = 0; i < 16; i++)
		{
			lcd.print((char)255);
			delay(loadSpd);
		}
		delay(500);
		lcd.clear();
	}
}

void demo()
{
	lcd.print("Backlight:");
	delay(1000);
	lcd.display(false);
	delay(1000);
	lcd.display(true);
	lcd.clear();

	lcd.print("Blink");
	lcd.blink(true);
	delay(2000);
	lcd.blink(false);
	delay(2000);

	Timer timer;
	timer.start();

	while (true)
	{
		input.beginStep();
		{
			lcd.home();
			lcd.print("Press any key...");
			lcd.setCursor(0, 1);
			lcd.printf("Time: %i", timer.elapsed().seconds());

			if (input.getAny())
				break;

			delay(10);
		}
		input.endStep();
	}
}

void netTest()
{
	lcd.setCursor(0, 0);
	const char* e0 = Net::getAddr("eth0");
	lcd.printf("e0 %s", (e0 ? e0 : "Offline"));

	lcd.setCursor(0, 1);
	const char* w0 = Net::getAddr("wlan0");
	lcd.printf("w0 %s", (w0 ? w0 : "Offline"));
}

void cursorTest()
{
	lcd.clear().home().blink(true);

	while (true)
	{
		input.beginStep();

		if (input.getButtonDown(0))
		{
			lcd.moveCursor(0, -1);
		}
		else if (input.getButtonDown(1))
		{
			lcd.moveCursor(0, 1);
		}
		else if (input.getButtonDown(2))
		{
			lcd.moveCursor(-1, 0);
		}
		else if (input.getButtonDown(3))
		{
			lcd.moveCursor(1, 0);
		}
		else if (input.getButtonDown(4))
		{
			lcd.clear();
			lcd.print("Clear");
			delay(1000);
			lcd.clear();
			lcd.home();
		}
		else if (input.getButtonDown(5))
		{
			lcd.blink(false);
			break;
		}

		input.endStep();
	}
}

void dirTest()
{
	lcd.clear();
	lcd.print(" Directory Test ");
	delay(1000);
	lcd.clear();

	const std::string dir = fs.getCwd();
	lcd.printf("dir len: %i", dir.size());
	delay(1000);
	lcd.clear();

	if (dir.size() <= 16)
	{
		lcd.print(dir);
	}
	else
	{
		lcd.print(dir.substr(0, 16));
		lcd.setCursor(0, 1);
		lcd.print(dir.substr(16, 32));
	}

	delay(1000);
	lcd.clear();
}

void oledDemo1()
{
	lcd.print("  OLED Demo: 1  ");

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);

	lcd.clear().print("    Adafruit    ");
	ssd1306_display(); //Adafruit logo is visible
	ssd1306_clearDisplay();
	delay(2500);

	lcd.clear().print("  Draw String   ");
	const char* text = "This is demo for SSD1306 i2c driver for Raspberry Pi";
	ssd1306_drawString(text);
	ssd1306_display();
	delay(1500);

	lcd.clear().print("  Scroll Right  ");
	ssd1306_dim(1);
	ssd1306_startscrollright(00, 0xFF);
	delay(1500);

	lcd.clear().print("   Rectangles   ");
	ssd1306_clearDisplay();
	ssd1306_fillRect(10, 10, 50, 20, WHITE);
	ssd1306_fillRect(80, 10, 130, 50, WHITE);
	ssd1306_display();

	lcd.clear();
}

void oledDemo2()
{
	lcd.print("  OLED Demo: 2  ");
	delay(1500);

	lcd.clear();
	lcd.printf("W: %d", oled.getW());
	lcd.setCursor(0, 1);
	lcd.printf("H: %d", oled.getH());
	delay(1500);

	if (oled.setup())
	{
		lcd.clear().print("      Setup     ");
		oled.display(); //Adafruit logo is visible
		oled.clear();
		delay(2500);

		lcd.clear().print("   Set Cursor   ");
		oled.setCursor(0, 0);
		delay(1500);

		lcd.clear().print("  Draw String   ");
		oled.drawString("Hello, World!");
		oled.display();
		delay(1500);

		lcd.clear().print("      Timer     ");
		Timer timer;
		timer.start();
		int ms;
		while ((ms = (int)timer.elapsed().millis()) < 5000)
		{
			lcd.setCursor(0, 1).printf("mS: %d", ms);
			oled.clear();
			oled.drawString("Millis: " + std::to_string(ms));
			oled.display();
		}

		lcd.clear().print("clear");
		oled.clear();
		oled.display();
		delay(1500);
	}
	else
	{
		lcd.clear();
		lcd.print("   OLED Error   ");
		delay(2500);
	}

	lcd.clear();
}

void printScroll(const std::string& value)
{
}

void scrollTest()
{
	lcd.clear().print("  Scroll Test   ");
	delay(1500);

	const std::string msg16 = "This is 16 chars";
	const std::string msg32 = "This is 32 chars. Hello, World! ";

	const size_t count16 = msg16.size();
	const size_t count32 = msg32.size();

	const int scrollSpeed = 250;

	lcd.clear().print(msg16);
	delay(1000);

	for (size_t i = count16; i-- != 0;)
	{
		lcd.clear().print(msg16.substr(0, i));
		delay(scrollSpeed);
	}
	
	delay(1000);

	for (size_t i = 0; i <= count16; i++)
	{
		lcd.clear().print(msg16.substr(0, i));
		delay(scrollSpeed);
	}
	
	delay(1000);

	for (size_t i = 0; i <= count16; i++)
	{
		lcd.clear().print(msg16.substr(i, count16 - i));
		delay(scrollSpeed);
	}
	
	delay(1000);

	for (size_t i = 0; i <= count32; i++)
	{
		lcd.clear().print(msg32.substr(i, count32 - i));
		delay(scrollSpeed);
	}

	delay(1000);

	for (size_t i = 0; i < count32 - lcd.getW(); i++)
	{
		lcd.clear().print(msg32.substr(i, count32 - i));
		delay(scrollSpeed);
	}

	delay(1000);
	lcd.clear();
}

void rotaryTest()
{
	lcd.clear().print("Rotary Encoder");
	delay(1500);
	lcd.clear();

	bool running = true;
	while (running)
	{
		input.beginStep();
		{
			//for (int i = 0; i < Input::MAX_BUTTON; i++)
			//{
			//	if (input.getButtonDown(i))
			//	{
			//		lcd.clear().printf("Press: %d", i);
			//		delay(1000);
			//		lcd.clear();
			//		running = false;
			//		break;
			//	}
			//}

			if (!input.getButtonDown(7))
			{
				lcd.clear().print("Press!");
				delay(1000);
				lcd.clear();
			}
		}
		input.endStep();
	}
}

// * * * * * * * * * * * * * * //

int loadINI(const std::string& filename)
{
	INIReader ini(filename);
	if (ini.ParseError() >= 0)
	{
		version	= ini.GetInteger("General", "version", 0);
		loadSpd	= ini.GetInteger("General", "loadSpd", 100);
		mode	= ini.GetInteger("General", "mode", 0);

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int setup(int argc, char** argv)
{
	if (loadINI(fs.getRoot() + CONFIG_PATH))
	{
		std::cerr << "Failed Loading Config" << std::endl;
		return EXIT_FAILURE;
	}

	if (wiringPiSetup() < 0)
	{
		std::cerr << "Failed Loading WiringPi" << std::endl;
		return EXIT_FAILURE;
	}

	if (!lcd.setup())
	{
		std::cerr << "Failed Loading LiquidCrystal" << std::endl;
		return EXIT_FAILURE;
	}

	if (!oled.setup())
	{
		std::cerr << "Failed Loading OLED" << std::endl;
		return EXIT_FAILURE;
	}

	if (!display.setup())
	{
		std::cerr << "Failed Loading LiquidCrystal" << std::endl;
		lcd.clear().print("Error: Display 2");
		delay(2500);
	}
	else
	{
		display.clear();
		display.print("Hello, World!");
	}

	for (int i = 0; i < Input::MAX_BUTTON; i++)
	{
		input.setPin(i, INPUT, PUD_DOWN);
	}

	return EXIT_SUCCESS;
}

// * * * * * * * * * * * * * * //

int main(int argc, char** argv)
{
	if (setup(argc, argv))
	{
		return EXIT_FAILURE;
	}

	showLoading();

	oled.reset().display().clear();

	lcd.printf("    Mode: %d     ", mode);
	delay(1500);
	lcd.clear();
	{
		switch (mode)
		{
		case 0:
			demo();
			break;
		case 1:
			netTest();
			return EXIT_SUCCESS;
		case 2:
			cursorTest();
			break;
		case 3:
			dirTest();
			break;
		case 4:
			oledDemo1();
			break;
		case 5:
			oledDemo2();
			break;
		case 6:
			scrollTest();
			break;
		case 7:
			rotaryTest();
			break;
		}
	}
	lcd.clear();
	lcd.print("    Goodbye!    ");
	delay(1500);
	lcd.clear();

	netTest();

	return EXIT_SUCCESS;
}

// * * * * * * * * * * * * * * //
