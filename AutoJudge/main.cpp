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
long __stdcall callback(_EXCEPTION_POINTERS* excp)
{
	printf("Error   address   %x/n", excp->ExceptionRecord->ExceptionAddress);
	printf("CPU   register:/n");
	printf("eax   %x   ebx   %x   ecx   %x   edx   %x/n", excp->ContextRecord->Eax, excp->ContextRecord->Ebx, excp->ContextRecord->Ecx, excp->ContextRecord->Edx);
	return EXCEPTION_EXECUTE_HANDLER;
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
	CFileFind finder, infinder;
	BOOL bWorking = finder.FindFile(_T("*.cpp"));
	GetColor(defaultcolor);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = CT2A(finder.GetFileName());
		cout << filename << endl;
		if (Compile(filename))
		{
			ShowMessageNo("Compile Error");
			continue;
		}
		BOOL infile = infinder.FindFile(TEXT(".\\input\\*.*"));
		DWORD exitCode;
		bool AC = true;
		while (infile&&AC)
		{
			infile = infinder.FindNextFile();
			if (infinder.IsDots() || infinder.IsDirectory()) continue;
			infilename = CT2A(infinder.GetFileName());
			CA2T parameter = ("/c a > .\\temp\\output.txt < .\\input\\" + infilename).c_str();
			SHELLEXECUTEINFO info = { sizeof(SHELLEXECUTEINFO) ,SEE_MASK_NOCLOSEPROCESS ,NULL,TEXT("open"),TEXT("cmd"),parameter,NULL ,SW_HIDE };
			system("del temp\\*.* /Q");
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