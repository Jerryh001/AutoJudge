#include "stdafx.h"
#include "Problem.h"
using namespace std;
int Problem::Compile(const SubmitObject & s)
{
	static const string GppPath = Config::GetData("compiler");
	static const string CompileOption = Config::GetData("compileoption");
	string file = s.Name;
	CopySubmitFile(s);
	if ( type == Header )
	{
		CopyMainFile();
		file = "main.cpp";
	}
	string CompileCommend = R"(cmd /C "set "PATH=)" + GppPath + R"(;%PATH%" && g++ ")" + path + R"(AJ_temp\)" + file + R"(" )" + CompileOption + R"( -o )" + path + R"(AJ_temp\a.exe >nul 2>nul")";
	return system(CompileCommend.c_str());
}
Problem::Problem(const string & folder)
{
	path = folder;
}

void Problem::ReadFile(const FileType& type, const string& casereg, string codereg, const string& main)
{
	CFileFind finder;
	BOOL found = finder.FindFile(CA2CT((path + R"(*.*)").c_str()));
	this->type = type;
	if ( codereg.empty() )
	{
		switch ( type )
		{
			case Header:
				codereg = R"((\.h)$)";
				break;
			case Code:
				codereg = R"((\.cpp)$)";
		}
	}
	//mainfile = finder;
	while ( found )
	{
		found = finder.FindNextFile();
		if ( !finder.IsDirectory() )
		{
			string name = CT2CA(finder.GetFileName());
			if ( regex_search(name, regex(codereg)) && !(this->type == Header&&name == main) )
			{
				submitarray.push_back(SubmitObject(finder, type));
			}
		}
	}
	finder.Close();
	found = finder.FindFile(CA2CT((path + R"(input\*.*)").c_str()));
	while ( found )
	{
		found = finder.FindNextFile();
		if ( !finder.IsDirectory() )
		{
			string name = CT2CA(finder.GetFileName());
			if ( regex_search(name, regex(casereg)) )
			{
				testarray.push_back(TestCase(finder));
			}
		}
	}
	finder.Close();
}

Problem::~Problem()
{
}

void Problem::Judge()
{
	if ( type == Header )
	{
		CopyMainFile();
	}
	for ( SubmitObject& s : submitarray )
	{
		if ( Compile(s) )
		{
			s.SetFinalResult(CE);
		}
		else
		{
			for ( TestCase& t : testarray )
			{
				JudgeResult ans = Result(s, t);
				s.SetResult(t, ans);
				if ( ans != AC &&!partscore)
				{
					break;
				}
			}
		}
		cout << s.Name << endl;
		if (s.GetFinalReault() == AC)
		{
			UI::ShowMessageYes("AC");
		}
		else
		{
			UI::ShowMessageNo("NOT AC");
		}
	}
}

JudgeResult Problem::Result(SubmitObject s, TestCase t)
{
	static SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };
	static DWORD time = stoi(Config::GetData("time"));
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.hStdInput = CreateFile(CA2CT(t.Path.c_str()), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	si.hStdOutput = CreateFile(CA2CT((path + R"(AJ_temp\)" + t.Name).c_str()), GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CreateProcess(CA2CT((path + R"(AJ_temp\a.exe)").c_str()), NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	JudgeResult ans=NJ;
	switch ( WaitForSingleObject(pi.hProcess, time) )
	{
		case WAIT_TIMEOUT:
			TerminateProcess(pi.hProcess, EXIT_FAILURE);
			if ( system("taskkill /F /T /IM WerFault.exe > nul 2> nul") == ERROR_SUCCESS )
			{
				ans = RE;
			}
			else
			{
				ans = TLE;
			}
			break;
		case WAIT_OBJECT_0:
			break;
		default:
			ans = UN;
			break;
	}
	CloseHandle(si.hStdInput);
	CloseHandle(si.hStdOutput);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if ( ans == NJ )
	{
		if ( Compare(t) )
		{
			ans = WA;
		}
		else
		{
			ans = AC;
		}
	}
	return ans;
}

unsigned int Problem::Compare(const TestCase & t)
{
	ifstream user(R"(.\AJ_temp\)" + t.Name);
	ifstream ans(string(t.Path).replace(t.Path.length()-t.Name.length()-6,2,"out"));
	string userstr, ansstr;
	for ( unsigned int line = 1;; line++ )
	{
		getline(user, userstr);
		getline(ans, ansstr);
		if ( user.eof() != ans.eof() || userstr != ansstr )
		{
			return line;
		}
		if ( user.eof() )
		{
			return 0;
		}
	}
}

void Problem::CopySubmitFile(const SubmitObject& s)
{
	system((R"(copy /y )" + s.Path + " " + path + R"(AJ_temp\)"+s.Name+R"( >nul )").c_str());
}

void Problem::CopyMainFile()
{
	system((R"(copy /y)" + path + mainfile.Name +" " + path + R"(AJ_temp\main.cpp >nul 2>nul)").c_str());
}
