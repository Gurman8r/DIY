#include <wiringPi.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <LiquidCrystal.h>
#include <Timer.h>
#include <Input.h>
#include <Net.h>
#include <StringUtilty.h>
#include <OptionParser.h>
#include <INIReader.h>

using namespace pi;

// * * * * * * * * * * * * * * //

#define CONFIG_PATH "../../../data/config.ini"

LiquidCrystal	lcd(0x3f, 16, 2);
Input			input;

// * * * * * * * * * * * * * * //

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

	delay(1500);
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

	char buf[1024];
	if (getcwd(buf, sizeof(buf)))
	{
		std::string dir(buf);

		lcd.printf("dir len: %i", dir.size());
		delay(1000);
		lcd.clear();

		lcd.print(dir.substr(0, 16));
		lcd.setCursor(0, 1);
		lcd.print(dir.substr(16, 32));
		
		delay(1000);
		lcd.clear();
	}

	delay(1000);
}

// * * * * * * * * * * * * * * //

int loadConfig(const char* filename)
{
	INIReader reader(filename);
	if (reader.ParseError() > 0)
	{
		lcd.printf("%s", reader.Get("General", "test", "Error").c_str());

		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

// * * * * * * * * * * * * * * //

int main(int argc, char** argv)
{
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

	if (loadConfig(CONFIG_PATH))
	{
		// Error
		std::cerr << "Failed Loading Config" << std::endl;
		return EXIT_FAILURE;
	}

	for (int i = 0; i < Input::MAX_BUTTON; i++)
	{
		input.setPin(i, INPUT, PUD_DOWN);
	}

	lcd.print("    Welcome!    ");
	delay(1000);
	lcd.clear();
	{
		
	}
	lcd.clear();
	lcd.print("    Goodbye!    ");
	delay(1000);
	lcd.clear();

	netTest();

	return EXIT_SUCCESS;
}

// * * * * * * * * * * * * * * //