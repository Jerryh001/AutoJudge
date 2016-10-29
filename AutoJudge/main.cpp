#ifndef _AFXDLL
#define _AFXDLL
#endif
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<afx.h>
#include<shellapi.h>
#include"Header.h"
using namespace std;
WORD defaultcolor;
bool GetColor(WORD &ret) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return false;
	ret = info.wAttributes;
	return true;
}
void ShowMessageYes(const string& message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << message << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
}
void ShowMessageNo(const string& message)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	cout << message << endl << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultcolor);
}
int main(int argc, char* argv[])
{
	string filename, infilename;
	CFileFind codefinder, infinder;
	BOOL bWorking = codefinder.FindFile(TEXT("*.cpp"));
	GetColor(defaultcolor);
	system("md temp >nul 2>nul");
	while (bWorking)
	{
		bWorking = codefinder.FindNextFile();
		filename = CT2A(codefinder.GetFileName());
		cout << filename << endl;
		if (Compile(filename))
		{
			ShowMessageNo("Compile Error");
			continue;
		}
		BOOL infile = infinder.FindFile(TEXT(R"(.\input\*.*)"));
		DWORD exitCode;
		bool AC = true;
		while (infile&&AC)
		{
			infile = infinder.FindNextFile();
			if (infinder.IsDots() || infinder.IsDirectory()) continue;
			infilename = CT2A(infinder.GetFileName());
			CA2T parameter = (R"( /c a.exe > .\temp\output.txt < .\input\)" + infilename).c_str();
			SHELLEXECUTEINFO info = { sizeof(SHELLEXECUTEINFO) ,SEE_MASK_NOCLOSEPROCESS ,NULL,TEXT("open"),TEXT("cmd"),parameter,NULL ,SW_SHOW };
			system(R"(del .\temp\*.* /Q >nul 2>nul)");
			ShellExecuteEx(&info);
			switch (WaitForSingleObject(info.hProcess, 1000))
			{
			case WAIT_TIMEOUT:
				system(("taskkill  /F /T /PID " + to_string(GetProcessId(info.hProcess)) + " >nul 2> nul").c_str());
				AC = false;
				ShowMessageNo("Time Limit Exceeded");
				break;
			case WAIT_ABANDONED:
			case WAIT_FAILED:
				AC = false;
				ShowMessageNo("Runtime Error");
				break;
			case WAIT_OBJECT_0:
				break;//is not work
				if ((!GetExitCodeProcess(info.hProcess, &exitCode)) || exitCode != 0)
				{
					AC = false;
					ShowMessageNo("Runtime Error");
				}
				break;
			default:
				cout << "WTF" << endl;
				break;
			}
			if (AC && !Compare(infilename))
			{
				AC = false;
				ShowMessageNo("Wrong Answer");
			}
		}
		if (AC)
		{
			ShowMessageYes("Answer Correct");
		}
	}
	return EXIT_SUCCESS;
}