#include "stdafx.h"
#include "UI.h"

namespace UI
{
	WORD defaultcolor;
	WORD defaultback;
	void Init()
	{
		//DisableClose();
		GetColor();
	}
	void DisableClose()
	{
		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
	}
	void EnableClose()
	{
		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_ENABLED);
	}
	void GetColor() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
		defaultcolor = info.wAttributes;
		defaultback = defaultcolor&(BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
	}
	void ShowMessageYes(const CString& message)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN| defaultback);
		cout << CT2CA(message) << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
	}
	void ShowMessageNo(const CString& message)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED| defaultback);
		cout << CT2CA(message) << endl << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
	}
};