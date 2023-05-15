#include "Autoclick.h"


int main()
{
	AutoclickSetup setup;
	setup.howManyClicks = 1000;
	setup.sleepBetweenClicks = 500;
	Autoclick clicker(setup);
	clicker.RunAutoclicker();
}
