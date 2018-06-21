#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <iostream>
#include <vector>
#include <FileSystem.h>
#include <INIReader.h>
#include <LiquidCrystal_I2C.h>
#include <MCP23017.h>
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
FileSystem			fs;
MCP23017			mcp(0x20);
LiquidCrystal_I2C	lcd(0x3f, 16, 2);

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

void testAB(int a, int b, bool p = true)
{
	if(p)
	{
		lcd.clear();
	}
	
	lcd.setPos(0, 0).printf("A: %d", a);
	lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(a));
	mcp.writeA(a);

	lcd.setPos(8, 0).printf("B: %d", b);
	lcd.setPos(8, 1).printf(B2BIN_PAT, B2BIN(b));
	mcp.writeB(b);

	lcd.setPos(lcd.getW() - 1, 0).printf("%d", mcp[MCP23017::F_Reverse]);
	
	if (p)
	{
		pause();
	}
}

void testA(int value)
{
	lcd.clear().printf("A: %d", value);
	lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(value));
	mcp.writeA(value);
	pause();
}

void testB(int value)
{
	lcd.clear().printf("B: %d", value);
	lcd.setPos(0, 1).printf(B2BIN_PAT, B2BIN(value));
	mcp.writeB(value);
	pause();
}

void testCounter()
{
	// Counter
	mcp[0] = data.dir;
	mcp.clearAB();
	lcd.clear().print("...");

	Timer timer;
	timer.start();

	for (int i = 0; i < 0xff; i++)
	{
		mcp.writeA(i).writeB(i);
		delay(50);
	}
	
	for (int i = 0, imax = 0xFFFF; i <= imax; i++)
	{
		mcp.writeAB(i);
	}

	timer.stop();
	lcd.clear().printf("Time: %d", timer.elapsed().millis());
	delay(1500);

	mcp.writeAB(0);
	delay(500);
	mcp.writeAB(0xFFFF);
	delay(500);
	mcp.writeAB(0);
	delay(500);
	mcp.writeAB(0xFFFF);
	delay(500);
	mcp.writeAB(0);
	delay(500);
	mcp.writeAB(0xFFFF);
	delay(500);
	mcp.writeAB(0);
	delay(1500);
	mcp.writeAB(0);
}

void testValues()
{
	lcd.clear().print("Testing Low/High");
	lcd.setPos(0, 1).printf("Speed: %dms", data.spd);
	mcp.clearAB();
	pause();

	// None
	mcp[MCP23017::F_Reverse] = 0;
	testAB(0b11100011, 0b11100011);

	// A
	mcp[MCP23017::F_Reverse] = 1;
	testAB(0b11100011, 0b11100011);

	// B
	mcp[MCP23017::F_Reverse] = 2;
	testAB(0b11100011, 0b11100011);

	// AB
	mcp[MCP23017::F_Reverse] = 4;
	testAB(0b11100011, 0b11100011);

}

void testKnight()
{
	std::vector<int> values = {
		//0x0000,
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
	
	int64_t ms;

	while ((ms = timer.elapsed().millis()) < 10000)
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

	testKnight();
	//testCounter();
	//testValues();

	//lcd.clear().print("    Goodbye!    ");
	//delay(1000);
	//mcp.clearA().clearB();
	//lcd.clear();

	return EXIT_SUCCESS;
}

// * * * * * * * * * * * * * * //
