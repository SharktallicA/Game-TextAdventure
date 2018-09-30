#pragma once
/*
	C++ general utility functions
	Khalid Ali 2018
	http://khalidali.co.uk/
*/

#include <iostream>
#include <sstream>
#include <random>
#include <windows.h>
#include <conio.h>

using namespace std;

//Enumeration of possible foreground and background colours that the C++ console can be set to
enum Colour
{
	BLACK, BLUE, GREEN, AQUA, RED, PURPLE, YELLOW, DEFAULT, GREY, LIGHT_BLUE,
	LIGHT_GREEN, LIGHT_AQUA, LIGHT_RED, LIGHT_PURPLE, LIGHT_YELLOW, WHITE
};

//Class of static methods that provide utility functions for C++ console applications
class Utility
{
private:
	Utility() {}; //prevents class from being constructed

public:
	//Requests yes (true)/no (false) boolean input from the user
	static bool Utility::getYesNo(void)
	{
		string strInput = "";
		bool boolIsYes = true;

		cin >> strInput;
		while (strInput != "Y" && strInput != "Yes" && strInput != "YES" && strInput != "yes" && strInput != "N" && strInput != "No" && strInput != "NO" && strInput != "no" && strInput != "y" && strInput != "n")
		{
			cout << "ERROR: you can only enter 'Yes', 'No' or a similar variation.\nTry again: ";
			cin >> strInput;
		}

		if (strInput == "Y" || strInput == "Yes" || strInput == "YES" || strInput == "yes" || strInput == "y")
			boolIsYes = true;
		else if (strInput == "N" || strInput == "No" || strInput == "NO" || strInput == "no" || strInput == "n")
			boolIsYes = false;
		return boolIsYes;
	}
	//Requests yes (true)/no (false) boolean input from the user with a given input notice message
	static bool Utility::getYesNo(string strMessage)
	{
		string strInput = "";
		bool boolIsYes = true;

		cout << strMessage;
		cin >> strInput;
		cin.ignore();
		while (strInput != "Y" && strInput != "Yes" && strInput != "YES" && strInput != "yes" && strInput != "N" && strInput != "No" && strInput != "NO" && strInput != "no" && strInput != "y" && strInput != "n")
		{
			cout << "ERROR: you can only enter 'Yes', 'No' or a similar variation.\nTry again: ";
			cin >> strInput;
		}

		if (strInput == "Y" || strInput == "Yes" || strInput == "YES" || strInput == "yes" || strInput == "y")
			boolIsYes = true;
		else if (strInput == "N" || strInput == "No" || strInput == "NO" || strInput == "no" || strInput == "n")
			boolIsYes = false;
		return boolIsYes;
	}

	//Safely requests any string input from the user
	static string Utility::getString(void)
	{
		string strInput = "";
		getline(cin, strInput);
		return strInput;
	}
	//Safely requests any string input from the user with a given input notice message
	static string Utility::getString(string strMessage)
	{
		cout << strMessage;
		string strInput = "";
		getline(cin, strInput);
		return strInput;
	}

	//Safely requests integer input from the user
	static int Utility::getInteger(void)
	{
		string strInput = "";
		int intInputAsInt = -1024;

		cin >> strInput;
		cin.ignore();
		stringstream strStream(strInput);
		strStream >> intInputAsInt;
		return intInputAsInt;
	}
	//Safely requests integer input from the user with a given input notice message
	static int Utility::getInteger(string strMessage, int intMin, int intMax)
	{
		string strInput = "";
		int intInputAsInt = -1024;

		cout << strMessage;
		cin >> strInput;
		cin.ignore();
		stringstream strStream(strInput);
		strStream >> intInputAsInt;
		while (intInputAsInt < intMin || intInputAsInt > intMax)
		{
			cout << "ERROR: you can only enter a whole number between " << intMin << " and " << intMax << ".\nTry again: ";
			cin >> strInput;
			cin.ignore();
			stringstream strStream(strInput);
			strStream >> intInputAsInt;
		}
		return intInputAsInt;
	}

	//Generates a random number using 32-Bit Mersenne Twister 19937
	static int Utility::generateNumber32(int intMin, int intMax)
	{
		int intResult;
		random_device randGenerator;
		mt19937 mersenne(randGenerator());
		uniform_int_distribution<int> distInt(intMin, intMax);
		intResult = distInt(mersenne);
		return intResult;
	}
	//Generates a random number using 64-Bit Mersenne Twister 19937
	static int Utility::generateNumber64(int intMin, int intMax)
	{
		int intResult;
		random_device randGenerator;
		mt19937_64 mersenne(randGenerator());
		uniform_int_distribution<int> distInt(intMin, intMax);
		intResult = distInt(mersenne);
		return intResult;
	}

	//Clears the screen
	static void Utility::clearScreen(void)
	{
		system("cls");
	}

	//Waits for user input - good replacement for system("pause")
	static char Utility::pause(string strMessage = "Press any key to continue...")
	{
		cout << strMessage;
		char in = _getch();
		cout << endl;
		return in;
	}

	//Sets the C++ console window's title
	static void Utility::setWindowTitle(string strTitle)
	{
		SetConsoleTitle(strTitle.c_str());
	}

	//Sets the C++ console window's size
	static void Utility::setWindowSize(unsigned int uintWidth, unsigned int uintHeight)
	{
		//purpose: sets the window size
		//parametres: (uintWidth) specified window width, (uintHeight) specified window height

		HWND console = GetConsoleWindow();
		RECT rectWindow;

		GetWindowRect(console, &rectWindow);
		MoveWindow(console, rectWindow.left, rectWindow.top, uintWidth, uintHeight, TRUE);
	}

	//Sets the C++ console window's colours
	static void Utility::setColour(Colour clrFore = WHITE, Colour clrBack = BLACK)
	{
		//purpose: sets the console colour
		//parametres: (clrFore) enumeration for foreground colour, (clrBack) enumeration for background colour (defaulted as black)

		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		int intColour = clrBack * 16 + clrFore;

		SetConsoleTextAttribute(handle, intColour);
	}

	//Moves the C++ console window's cursor
	static void Utility::moveCursor(SHORT shrtX, SHORT shrtY)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD point = { shrtX, shrtY };
		SetConsoleCursorPosition(handle, point);
	}
};