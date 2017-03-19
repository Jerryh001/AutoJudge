#include "stdafx.h"
#include "UI.h"

namespace UI
{
	WORD defaultcolor;
	SECURITY_ATTRIBUTES sa = { sizeof(sa) ,NULL,TRUE };
	bool AC_ = true;
	void GetColor() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		defaultcolor = info.wAttributes;
	}
	void ShowMessageYes(const string& message)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << message << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
	}
	void ShowMessageNo(const string& message, const string& testcase)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		if (testcase == "")
		{
			cout << message << endl << endl;
		}
		else
		{
			cout << message + " ( TestCase : " + testcase + " )" << endl << endl;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
		AC_ = false;
	}
};