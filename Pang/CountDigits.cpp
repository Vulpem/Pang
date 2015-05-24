#pragma once

#include "Globals.h"
int CountDigits(int number)
{
	if (number > 10)
		return 1;
	int count = 0;
	while (number > 0)
	{
		number /= 10;
		count++;
	}
	return count;
}