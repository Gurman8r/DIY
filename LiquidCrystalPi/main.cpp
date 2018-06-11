#include <iostream>
#include <wiringPi.h>
#include "include/LiquidCrystal.h"

using namespace pi;

LiquidCrystal lcd(0x3f, 16, 2);

int main(void)
{
	if (wiringPiSetup() < 0)
	{
		std::cerr << "Failed WiringPi Setup" << std::endl;
		return EXIT_FAILURE;
	}

	if (!lcd.setup())
	{
		std::cerr << "Failed LCD Setup" << std::endl;
		return EXIT_FAILURE;
	}

	lcd.clear();
	lcd.print("Hello, World!");
	delay(2500);
	lcd.clear();

	return EXIT_SUCCESS;
}