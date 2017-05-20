#include "stdafx.h"
#include "Problem.h"
using namespace std;
int Problem::Compile(const SubmitObject & s)
{
	const string GppPath = config.GetData("COMPILE","path");
	const string CompileOption = config.GetData("COMPILE","options");
	string file = s.Name;
	if (config.GetData("GENERAL","cleanup", "0") != "0")
	{
		MoveSubmitFile(s);
	}
	else
	{
		CopySubmitFile(s);
	}
	if (type == Header)
	{
		CopyMainFile();
		file = "main.cpp";
	}
	string CompileCommend = R"(cmd /C "set "PATH=)" + GppPath + R"(;%PATH%" && g++ ".\AJ_temp\)" + file + R"(" -o .\AJ_temp\a.exe -w )" + CompileOption + R"(")";
	while (!DeleteFile(CA2CT(R"(.\AJ_temp\a.exe)")) && (GetLastError() == ERROR_ACCESS_DENIED));
	return system(CompileCommend.c_str());
}
Problem::Problem(const string & folder, const bool& part) :path(folder + R"(\)"), config(path), partscore(part)
{

}

void Problem::ReadFile(const string& main, string codereg, const string& casereg)
{
	CFileFind finder;
	BOOL found = finder.FindFile(CA2CT((path + R"(*.*)").c_str()));
	type = config.GetData("GENERAL","mode","0") != "0" ? Header : Code;
	partscore = config.GetData("SCORE","part-score", "0") == "0" ? false : true;
	if (codereg.empty())
	{
		switch (type)
		{
		case Header:
			codereg = config.GetData("HEADER","headerregex",R"((\.h)$)");
			break;
		case Code:
			codereg = config.GetData("CODE", "coderegex", R"((\.cpp)$)");
		}
	}
	while (found)
	{
		found = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			string name = CT2CA(finder.GetFileName());
			if (type == Header&&name == main)
			{
				mainfile = finder;
			}
			else if (regex_search(name, regex(codereg)))
			{
				submitarray.push_back(SubmitObject(finder, type));
			}
		}
	}
	finder.Close();
	found = finder.FindFile(CA2CT((path + R"(input\*.*)").c_str()));
	while (found)
	{
		found = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			string name = CT2CA(finder.GetFileName());
			if (regex_search(name, regex(casereg)))
			{
				testarray.push_back(TestCase(finder, config.GetData("SEGMENT","token", "")));
			}
		}
	}
	segment_count = 0;
	for (TestCase& t : testarray)
	{
		segment_count += t.GetSegmentCount();
	}
	finder.Close();
}

Problem::~Problem()
{
}

void Problem::Judge()
{
	if (submitarray.empty()) return;
	cout << "Problem at" << endl << path << ":" << endl;
	if (type == Header)
	{
		CopyMainFile();
	}
	for (SubmitObject& s : submitarray)
	{
		cout << s.Name << endl;
		if (Compile(s))
		{
			s.SetFinalResult(CE);
		}
		else
		{
			for (TestCase& t : testarray)
			{
				if (Execute(s, t) != AC && !partscore)
				{
					break;
				}
			}
		}
		int ACs = s.GetACCount();
		if (partscore == true && ACs > 0)
		{
			if (ACs == segment_count)
			{
				UI::ShowMessageYes("Answer Correct");
			}
			else
			{
				UI::ShowMessageNo("Answer Correct " + to_string(ACs) + "/" + to_string(segment_count));
			}
		}
		else
		{
			switch (s.GetFinalReault())
			{
			case AC:
				UI::ShowMessageYes("Answer Correct");
				break;
			case WA:
				UI::ShowMessageNo("Wrong Answer");
				break;
			case CE:
				UI::ShowMessageNo("Compiltion Error");
				break;
			case TLE:
				UI::ShowMessageNo("Time Limit Exceeded");
				break;
			case RE:
				UI::ShowMessageNo("Runtime Error");
				break;
			default:
			//case NJ:
			//case UN:
				UI::ShowMessageNo("Something Wrong(not a valid judge)");
				break;
			}
		}
	}
}

JudgeResult Problem::Execute(SubmitObject& s, TestCase t)
{
	HANDLE job = CreateJobObject(NULL, NULL);
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit = { 0 };
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	DWORD time = stoi(config.GetData("LIMIT","time"));
	DWORD memory = stoi(config.GetData("LIMIT", "memory"));
	PROCESS_INFORMATION pi;
	DWORD exitcode;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	limit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_PROCESS_TIME | JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
	limit.ProcessMemoryLimit = memory*1048576 ;///unit=MB
	limit.BasicLimitInformation.PerProcessUserTimeLimit.QuadPart = time * 1000;
	SetInformationJobObject(job, JobObjectExtendedLimitInformation, &limit, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION));
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = CreateFile(CA2CT(t.Path.c_str()), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	si.hStdOutput = CreateFile(CA2CT((R"(.\AJ_temp\)" + t.Name).c_str()), GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CreateProcess(TEXT(R"(.\AJ_temp\a.exe)"), NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	AssignProcessToJobObject(job, pi.hProcess);
	CloseHandle(si.hStdInput);
	CloseHandle(si.hStdOutput);
	CloseHandle(pi.hThread);
	switch (WaitForSingleObject(pi.hProcess, INFINITE))
	{
	case WAIT_OBJECT_0:
		GetExitCodeProcess(pi.hProcess, &exitcode);
		if (exitcode == 0xC0000044)///should be ERROR_NOT_ENOUGH_QUOTA, not sure why
		{
			t.AddResult(TLE);
		}
		else if (exitcode == 3 || (exitcode >> 30) == 3)
		{
			t.AddResult(RE);
		}
		else
		{
			Compare(s, t);
		}
		break;
	default:
		t.AddResult(UN);
		break;
	}
	TerminateJobObject(job, exitcode);
	CloseHandle(pi.hProcess);
	CloseHandle(job);
	s.AddTestedCase(t);
	return t.GetResult();
}

unsigned int Problem::Compare(SubmitObject& s, TestCase & t)
{
	ifstream user(R"(.\AJ_temp\)" + t.Name);
	ifstream ans(string(t.Path).replace(t.Path.length() - t.Name.length() - 6, 2, "out"));
	string userstr, ansstr;
	string token = t.GetToken();
	unsigned int firstdiff = 0;
	bool segac = true;
	for (unsigned int line = 0; t.GetResultVector().size() < t.GetSegmentCount(); )
	{
		getline(user, userstr); ++line;
		getline(ans, ansstr);
		if (userstr != ansstr)
		{
			if (firstdiff == 0)
			{
				firstdiff = line;
			}
			segac = false;
		}
		bool userend = IsOnSegmentEnd(user, userstr, token);
		bool ansend = IsOnSegmentEnd(ans, ansstr, token);
		if (userend || ansend)
		{
			if (!(userend&&ansend))
			{
				while (!IsOnSegmentEnd(user, userstr, token)) { getline(user, userstr); ++line; }
				while (!IsOnSegmentEnd(ans, ansstr, token)) getline(ans, ansstr);
				segac = false;
			}
			if (segac)
			{
				t.AddResult(AC);
			}
			else
			{
				t.AddResult(WA);
			}
			segac = true;
		}
	}
	if (!user.eof() || !ans.eof())
	{
		t.SetSegmentSizeError();
	}
	return firstdiff;
}
bool Problem::IsOnSegmentEnd(const istream & in, const string & str, const string & token)
{
	return in.eof() || (token.empty() ? false : str == token);
}
void Problem::MoveSubmitFile(const SubmitObject& s)
{
	string targetname;
	if (type == Header)
	{
		targetname = config.GetData("HEADER","include");
	}
	else
	{
		targetname = s.Name;
	}
	MoveFileEx(CA2CT(s.Path.c_str()), CA2CT((R"(.\AJ_temp\)" + targetname).c_str()), MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);//TODO:檢查是否成功
}
void Problem::CopySubmitFile(const SubmitObject& s)
{
	string targetname;
	if (type == Header)
	{
		targetname = config.GetData("HEADER", "include");
	}
	else
	{
		targetname = s.Name;
	}
	CopyFile(CA2CT(s.Path.c_str()), CA2CT((R"(.\AJ_temp\)" + targetname).c_str()), FALSE);//TODO:檢查是否成功
}

void Problem::CopyMainFile()
{
	CopyFile(CA2CT((path + mainfile.Name).c_str()), CA2CT(R"(.\AJ_temp\main.cpp)"), FALSE);//TODO:檢查是否成功
}
