#ifndef _AFXDLL
#define _AFXDLL
#endif
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<afx.h>
#include<shellapi.h>
#include<Windows.h>
#include"Header.h"
using namespace std;
WORD defaultcolor;
SECURITY_ATTRIBUTES sa = { sizeof(sa) ,NULL,TRUE };
bool AC = true;
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
void ShowMessageNo(const string& message, const string& testcase = "")
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
	AC = false;
}
void judue()
{
	CFileFind infinder;
	BOOL infileexist = infinder.FindFile(TEXT(R"(.\input\*.*)"));
	AC = true;
	DWORD time = stoi(Config::GetData("time"));
	while (infileexist&&AC)
	{
		infileexist = infinder.FindNextFile();
		string infilename = CT2A(infinder.GetFileName());
		CString useroutfile = (R"(.\AJ_temp\)" + infilename).c_str();
		if (infinder.IsDots() || infinder.IsDirectory()) continue;
		PROCESS_INFORMATION pi;
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdInput = CreateFile(infinder.GetFilePath(), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		si.hStdOutput = CreateFile(useroutfile, GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CreateProcess(TEXT(R"(.\AJ_temp\a.exe)"), NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		switch (WaitForSingleObject(pi.hProcess, time))//CREATE_NO_WINDOW
		{
		case WAIT_TIMEOUT:
			TerminateProcess(pi.hProcess, EXIT_FAILURE);
			if (system("taskkill /F /T /IM WerFault.exe > nul 2> nul") == ERROR_SUCCESS)
			{
				ShowMessageNo("Runtime Error", infilename);
			}
			else
			{
				ShowMessageNo("Time Limit Exceeded", infilename);
			}
			break;
		case WAIT_OBJECT_0:
			break;
		default:
			ShowMessageNo("Unknown Error", infilename);
			break;
		}
		CloseHandle(si.hStdInput);
		CloseHandle(si.hStdOutput);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		int firstdiff = Compare(infilename);
		if (AC && firstdiff)
		{
			ShowMessageNo("Wrong Answer at Line " + to_string(firstdiff), infilename);
		}
	}
	if (AC)
	{
		ShowMessageYes("Answer Correct");
	}
}
int main(int argc, char* argv[])
{
	Config::Init();
	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
	string codefilename, hfilename;
	CFileFind codefinder, hfinder;
	int compileresult;
	BOOL iscodeexist = codefinder.FindFile(TEXT("*.cpp"));
	GetColor();
	system("rd /s /q AJ_temp >nul 2>nul");
	system("md AJ_temp >nul 2>nul");
	if (stoi(Config::GetData("headermode"))!=0)
	{
		BOOL ishexist = hfinder.FindFile(TEXT("*.h"));
		cout << R"(!HEADER FILE MODE!)" << endl;
		codefinder.FindNextFile();
		codefilename = CT2A(codefinder.GetFileName());
		system((R"(copy .\)" + codefilename + R"( .\AJ_temp\main.cpp >nul 2>nul)").c_str());
		codefilename = R"(.\AJ_temp\main.cpp)";
		string hjudgename = Config::GetData("header");
		while (ishexist)
		{
			ishexist = hfinder.FindNextFile();
			hfilename = CT2A(hfinder.GetFileName());
			system((R"(copy ".\)" + hfilename + R"(" .\AJ_temp\)" + hjudgename + R"( >nul 2>nul)").c_str());
			compileresult = Compile(codefilename);
			cout << hfilename << endl;
			if (compileresult)
			{
				ShowMessageNo("Compile Error");
				continue;
			}
			judue();
		}
	}
	else
	{
		cout << R"(!CODE FILE MODE!)" << endl;
		while (iscodeexist)
		{
			iscodeexist = codefinder.FindNextFile();
			codefilename = CT2A(codefinder.GetFileName());
			compileresult = Compile(codefilename);
			cout << codefilename << endl;
			if (compileresult)
			{
				ShowMessageNo("Compile Error");
				continue;
			}
			judue();
		}
	}
	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_ENABLED);
	return EXIT_SUCCESS;
}