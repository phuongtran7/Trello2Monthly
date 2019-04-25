// Trello2Montly.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "monthly.h"

int main(int argc, char* argv[])
{
	monthly new_month;
	new_month.run();
	new_month.shutdown();
	std::getchar();
	return 0;
}