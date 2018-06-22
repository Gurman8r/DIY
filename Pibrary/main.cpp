#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <Args.h>
#include <Console.h>
#include <FileSystem.h>
#include <INIReader.h>
#include <Input.h>
#include <LiquidCrystal_I2C.h>
#include <MCP23017.h>
#include <StringUtilty.h>
#include <Timer.h>

using namespace pi;

// * * * * * * * * * * * * * * //

#define B2BIN_PAT "%c%c%c%c%c%c%c%c"
#define B2BIN(b) \
	(b & 0x80 ? '1' : '0'), \
	(b & 0x40 ? '1' : '0'), \
	(b & 0x20 ? '1' : '0'), \
	(b & 0x10 ? '1' : '0'), \
	(b & 0x08 ? '1' : '0'), \
	(b & 0x04 ? '1' : '0'), \
	(b & 0x02 ? '1' : '0'), \
	(b & 0x01 ? '1' : '0')

#define S2BIN_PAT "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
#define S2BIN(s) \
	(s & 0x8000 ? '1' : '0'), \
	(s & 0x4000 ? '1' : '0'), \
	(s & 0x2000 ? '1' : '0'), \
	(s & 0x1000 ? '1' : '0'), \
	(s & 0x0800 ? '1' : '0'), \
	(s & 0x0400 ? '1' : '0'), \
	(s & 0x0200 ? '1' : '0'), \
	(s & 0x0100 ? '1' : '0'), \
	(s & 0x0080 ? '1' : '0'), \
	(s & 0x0040 ? '1' : '0'), \
	(s & 0x0020 ? '1' : '0'), \
	(s & 0x0010 ? '1' : '0'), \
	(s & 0x0008 ? '1' : '0'), \
	(s & 0x0004 ? '1' : '0'), \
	(s & 0x0002 ? '1' : '0'), \
	(s & 0x0001 ? '1' : '0')

// * * * * * * * * * * * * * * //

#define CONFIG_PATH "../../../config.ini"

// * * * * * * * * * * * * * * //

struct Data
{
	int adr;
	int spd;
	int dir;
	int pgm;
	int dur;

	friend std::ostream& operator<<(std::ostream& out, const Data& rhs)
	{
		std::cout
			<< "adr: " << rhs.adr << std::endl
			<< "spd: " << rhs.spd << std::endl;
		return out;
	}
};

// * * * * * * * * * * * * * * //

Data				data;
Console				console(std::cout, std::cin, std::cerr);
FileSystem			fs;
Input				input;
LiquidCrystal_I2C	lcd(0x3f, 16, 2);
MCP23017			mcp(0x20);

// * * * * * * * * * * * * * * //

void pause()
{
	lcd.setPos(lcd.getW() - 1, 1).blink(true);

	std::cout 
		<< std::endl << "Press any Key to Continue..."
		<< std::endl;

	while (!std::cin.get()) { }

	lcd.blink(false);
}

void testFlash()
{
	auto flash = [](int i, unsigned n, unsigned ms)
	{
		mcp.writeA(i & 1 ? 0 : 0xFF).writeB(i & 2 ? 0 : 0xFF);
		delay(ms);
		mcp.writeAB(0);
		delay(ms);
	};

	lcd.clear().printf("Flash");
	for (int i = -1; i < 3; i++)
	{
		lcd.clear().printf("Flash: %d", i);
		flash(i, 4, 250);
		delay(1000);
	}
}

void testCounter()
{
	lcd.clear().printf("Counter");

	mcp[0] = data.dir;
	mcp.clearAB();

	Timer timer;
	timer.start();
	for (int i = 0; i < 0xFF; i++)
	{
		mcp.writeA(i).writeB(i);
		delay(data.spd);
	}
	
	for (int i = 0, imax = 0xFFFF; i <= imax; i++)
	{
		mcp.writeAB(i);
	}

	timer.stop();
	lcd.clear().printf("Time: %d", timer.elapsed().millis());
	delay(1500);

	
}

void testValues()
{
	auto testAB = [](int a, int b)
	{
		lcd.clear();

		lcd.setPos(0, 0).printf("A: %d", a);
		lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(a));
		mcp.writeA(a);

		lcd.setPos(8, 0).printf("B: %d", b);
		lcd.setPos(8, 1).printf(B2BIN_PAT, B2BIN(b));
		mcp.writeB(b);

		lcd.setPos(lcd.getW() - 1, 0).printf("%d", mcp[MCP23017::F_Reverse]);
	};

	lcd.clear().printf("Values");
	mcp.clearAB();
	delay(1000);

	for (int n = 0; n < 5; n++)
	{
		for (int i = -1; i < 3; i++)
		{
			mcp[MCP23017::F_Reverse] = i;
			testAB(0b11110000, 0b11110000);
			delay(250);
		}
	}

}

void testKnight()
{
	std::vector<int> values = {
		0x0001,
		0x0002,
		0x0004,
		0x0008,
		0x0010,
		0x0020,
		0x0040,
		0x0080,
		0x0100,
		0x0200,
		0x0400,
		0x0800,
		0x1000,
		0x2000,
		0x4000,
		0x8000,
	};

	std::vector<int>::const_iterator i;
	std::vector<int>::const_reverse_iterator r;

	Timer timer;
	timer.start();
	
	int64_t el;
	while ((el = timer.elapsed().seconds()) < data.dur)
	{
		for (i = values.begin(); i != values.end(); i++)
		{
			mcp.writeAB(*i);
			delay(45);
		}

		for (r = values.rbegin(); r != values.rend(); r++)
		{
			mcp.writeAB(*r);
			delay(45);
		}
	}
}

void testDip()
{
	lcd.clear().print("Dip\'");

	Timer	timer;
	int64_t ms;
	char	value = 0, state;

	timer.start();
	while ((ms = timer.elapsed().millis()) < 20000)
	{
		input.beginStep();
		{
			if (value != (state = (char)input.getState()))
			{
				value = state;
				lcd.clear().printf("Value: %d", value);
				lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(value));
				mcp.writeA(value).writeB(value);
			}
		}
		input.endStep();
		delay(10);
	}

}

void testSegment()
{
	lcd.clear().print("Segment");
	delay(1000);

	std::vector<int> table = { 
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00100111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b01110111, // A
		0b01111100, // B
		0b00111001, // C
		0b01011110, // D
		0b01111001, // E
		0b01110001, // F
	};

	for (int i : table)
	{
		lcd.clear().printf("%d", i);
		lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(i));
		mcp.writeA(i).writeB(i);
		delay(1000);
	}

	mcp.clearAB();
	delay(1000);
}

// * * * * * * * * * * * * * * //

int loadConfig(const std::string& filename)
{
	INIReader ini(filename);
	if (ini.ParseError() >= 0)
	{
		data.adr = ini.GetInteger("Misc", "adr", -1);
		data.spd = ini.GetInteger("Misc", "spd", 100);
		data.dir = ini.GetInteger("Misc", "dir", 0);
		data.pgm = ini.GetInteger("Misc", "pgm", 0);
		data.dur = ini.GetInteger("Misc", "dur", 5);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int setup()
{
	if (loadConfig(fs.getRoot() + CONFIG_PATH))
	{
		std::cerr << "Load Config Failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (wiringPiSetup() == -1)
	{
		std::cerr << "Wiring Pi Setup Failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (!lcd.setup())
	{
		std::cerr << "LiquidCrystal_I2C Setup Failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (!mcp.setup())
	{
		std::cerr << "MCP23017 Setup Failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (!console.setup())
	{
		std::cerr << "Console Setup Failed" << std::endl;
		return EXIT_FAILURE;
	}

	for (int i = 0; i < Input::MAX_PIN; i++)
	{
		input.setPin(i, INPUT, PUD_DOWN);
	}

	return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
	if (setup())
	{
		return EXIT_FAILURE;
	}

	lcd.clear().home().blink(false).print("    Welcome!    ");
	delay(1000);
	lcd.clear();

	mcp.writeReg8(MCP23017::IODIRA, 0).clearA();
	mcp.writeReg8(MCP23017::IODIRB, 0).clearB();

	input.capture();

	const char state = (char)input.getState();

	lcd.clear();
	lcd.setPos(4, 0).printf("Mode: %d", state);
	lcd.setPos(4, 1).printf(B2BIN_PAT, B2BIN(state));
	delay(1000);

	switch (input.getState())
	{
	case 0b0000: testFlash();	break;
	case 0b0001: testKnight();	break;
	case 0b0010: testValues();	break;
	case 0b0011: testCounter();	break;
	case 0b0100: testDip();		break;
	case 0b0101: testSegment();	break;
	default:
		lcd.clear().print("    Goodbye!    ");
		delay(1000);
		mcp.clearA().clearB();
		lcd.clear();
		break;
	}

	return EXIT_SUCCESS;
}

// * * * * * * * * * * * * * * //
