#include <wiringPi.h>
#include <iostream>
#include <FileSystem.h>
#include <INIReader.h>
#include <Input.h>
#include <LiquidCrystal.h>
#include <Net.h>
#include <OptionParser.h>
#include <StringUtilty.h>
#include <Timer.h>
#include <OLED.h>
#include <ssd1306_i2c.h>

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
int speed;
int mode;

FileSystem		fs;
LiquidCrystal	lcd(0x3f, 16, 2);
Input			input;
OLED			oled(OLED_VCC_SWITCHCAP, 0x3C, 128, 64);

// * * * * * * * * * * * * * * //

void showLoading()
{
	lcd.clear();
	lcd.print("Loading...");
	lcd.setCursor(0, 1);
	for (int i = 0; i < 16; i++)
	{
		lcd.print((char)255);
		delay(speed);
	}
	delay(500);
	lcd.clear();
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
	delay(5000);

	lcd.clear().print("  Draw String   ");
	const char* text = "This is demo for SSD1306 i2c driver for Raspberry Pi";
	ssd1306_drawString(text);
	ssd1306_display();
	delay(5000);

	lcd.clear().print("  Scroll Right  ");
	ssd1306_dim(1);
	ssd1306_startscrollright(00, 0xFF);
	delay(5000);

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

	if (oled.setup())
	{
		//lcd.clear().print("setup");
		//oled.display();
		//oled.clear();
		//delay(1000);

		//lcd.clear().print("setCursor");
		//oled.setCursor(0, 0);
		//delay(1000);

		lcd.clear().print("drawString");
		oled.drawString("Hello, World!");
		delay(1000);

		lcd.clear().print("display");
		//oled.display();
		//delay(5000);
		//Timer timer;
		//timer.start();
		//uint64_t ms;
		//while ((ms = timer.elapsed().millis()) < 5000)
		//{
		//	oled.display();
		//	delay(16);
		//}
		//delay(1000);

		lcd.clear().print("clear");
		oled.clear();
		delay(1000);
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

void inputTest()
{
	const std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int index = 0;
	bool update = true;

	while (true)
	{
		if (update)
		{
			lcd.print(alpha);
			update = false;
		}

		input.beginStep();
		{
			if (input.getButtonDown(PIN_L))
			{
				// left
				update = true;
			}
			else if (input.getButtonDown(PIN_R))
			{
				// right
				update = true;
			}
			else if (input.getButtonDown(PIN_N))
			{
				break;
			}
		}
		input.endStep();

		delay(10);
	}
}

// * * * * * * * * * * * * * * //

int loadConfig(const std::string& filename)
{
	INIReader reader(filename);
	if (reader.ParseError() >= 0)
	{
		version = reader.GetInteger("General", "version", 0);
		speed	= reader.GetInteger("General", "speed", 100);
		mode	= reader.GetInteger("General", "mode", 0);

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int setup()
{
	if (loadConfig(fs.getRoot() + CONFIG_PATH))
	{
		std::cerr << "Failed Loading Config" << std::endl;
		return EXIT_FAILURE;
	}

	if (wiringPiSetup() < 0)
	{
		std::cerr << "wiringPi setup failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (!lcd.setup())
	{
		std::cerr << "LiquidCrystal setup failed" << std::endl;
		return EXIT_FAILURE;
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
	if (setup())
	{
		return EXIT_FAILURE;
	}

	showLoading();

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
			inputTest();
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
