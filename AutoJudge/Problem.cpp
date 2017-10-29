#include "stdafx.h"
#include "Problem.h"
using namespace std;
int Problem::Compile(const SubmitObject & s)
{
	const CString GppPath = config.GetData("COMPILE", "path");
	const CString CompileOption = config.GetData("COMPILE", "options");
	CString file = s.GetFileName();
	if (config.GetData("GENERAL", "cleanup", "0") != "0")
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
	CString CompileCommend = R"(cmd /C "set "PATH=)" + GppPath + R"(;%PATH%" && g++ )" + CompileOption + R"( ".\AJ_temp\)" + file + R"(" -o .\AJ_temp\a.exe -w")";
	while (!DeleteFile(TEXT(R"(.\AJ_temp\a.exe)")) && (GetLastError() == ERROR_ACCESS_DENIED));
	while (!DeleteFile(TEXT(R"(.\AJ_temp\a.exe)")) && (GetLastError() != ERROR_FILE_NOT_FOUND));
	return _tsystem(CompileCommend);
}
Problem::Problem(const CString& folder) :path(folder + R"(\)"), config(path+R"(config.ini)")
{

}

void Problem::ReadFile(const CString& casereg)
{
	CFileFind finder;
	BOOL found = finder.FindFile(path + R"(*.*)");
	type = config.GetData("GENERAL", "mode", "0") != "0" ? Header : Code;
	partscore = config.GetData("SCORE", "part-score", "0") == "0" ? false : true;
	CString main = config.GetData("HEADER", "main", "main.cpp");
	CString codereg = ((type == Code) ? config.GetData("CODE", "coderegex", R"((\.cpp)$)") : config.GetData("HEADER", "headerregex", R"((\.h)$)"));
	while (found)
	{
		found = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			CString& name = finder.GetFileName();
			if (type == Header&&name == main)
			{
				mainfile.SetFile(finder.GetFilePath(), Code);
			}
			else if (regex_search(string(CT2CA(name)), regex(CT2CA(codereg))))
			{
				submitarray.push_back(SubmitObject(finder.GetFilePath(), type));
			}
		}
	}

	finder.Close();

	found = finder.FindFile(path + R"(input\*.*)");
	while (found)
	{
		found = finder.FindNextFile();
		if (!finder.IsDirectory())
		{
			CString& name = finder.GetFileName();
			if (regex_search(string(CT2CA(name)), regex(CT2CA(casereg))))
			{
				CString input = finder.GetFilePath();
				CString output = input;

				output.Replace(TEXT(R"(\input\)"), TEXT(R"(\output\)"));
				TestCase t(input, output);

				t.SetToken(config.GetData("SEGMENT", "token", ""));

				testarray.push_back(t);
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

void Problem::Judge()
{
	if (submitarray.empty()) return;
	CFile c;
	c.SetFilePath(path);//TODO : i dont like to do this to get folder name
	cout << "Problem at " << CT2CA(c.GetFileName()) << ":" << endl;
	if (type == Header)
	{
		CopyMainFile();
	}
	for (SubmitObject& s : submitarray)
	{
		cout << CT2CA(s.GetFileTitle()) << endl;
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
		if (partscore == true && ACs > 0 && ACs < segment_count)
		{
			CString str;
			str.Format(TEXT(R"(Answer Correct %d/%d)"), ACs, segment_count);
			UI::ShowMessageNo(str);
		}
		else
		{
			switch (s.GetFinalReault())
			{
			case PE:
				if (config.GetData("SCORE", "allowPE", "0") == "0")
				{
					UI::ShowMessageNo("Presentation Error");
				}
				else
				{
					UI::ShowMessageYes("Answer Correct(PE)");
				}
				break;
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
			case NJ:
				UI::ShowMessageNo("Not Judged(no testcase or something wrong)");
				break;
			case UN:
			default:
				UI::ShowMessageNo("Something Wrong(not a valid judge)");
				break;
			}
		}
	}
}

CString Problem::GetPath() const
{
	return path;
}

void Problem::SetSetting(const CString & path)
{
	config.SetPath(path);
}
void Problem::SetGlobalSetting(const CString& path)
{
	Config::SetGlobalPath(path);
}

void Problem::AutoJudge()
{
	ReadFile();
	Judge();
}

JudgeResult Problem::Execute(SubmitObject& s, TestCase t)
{
	HANDLE job = CreateJobObject(NULL, NULL);
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit = { 0 };
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	DWORD time = _ttoi(config.GetData("LIMIT", "time"));
	DWORD memory = _ttoi(config.GetData("LIMIT", "memory"));
	PROCESS_INFORMATION pi;
	DWORD exitcode = 0;
	STARTUPINFO si = { sizeof(STARTUPINFO) };

	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = CreateFile(t.GetFilePath(), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	si.hStdOutput = CreateFile(TEXT(R"(.\AJ_temp\)" + t.GetFileName()), GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	limit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_PROCESS_MEMORY | JOB_OBJECT_LIMIT_JOB_TIME | JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;
	limit.ProcessMemoryLimit = memory * 1048576;///unit=MB
	limit.BasicLimitInformation.PerJobUserTimeLimit.QuadPart = time * 10000;
	SetInformationJobObject(job, JobObjectExtendedLimitInformation, &limit, sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION));

	CreateProcess(TEXT(R"(.\AJ_temp\a.exe)"), NULL, NULL, NULL, TRUE, CREATE_BREAKAWAY_FROM_JOB | CREATE_NO_WINDOW | CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	AssignProcessToJobObject(job, pi.hProcess);
	ResumeThread(pi.hThread);
	CloseHandle(si.hStdInput);
	CloseHandle(si.hStdOutput);
	CloseHandle(pi.hThread);

	switch (WaitForSingleObject(pi.hProcess, INFINITE))
	{
	case WAIT_OBJECT_0:
		GetExitCodeProcess(pi.hProcess, &exitcode);
		cout << exitcode << endl;
		if (exitcode == 0xC0000044|| exitcode==ERROR_NOT_ENOUGH_QUOTA)///should be ERROR_NOT_ENOUGH_QUOTA, not sure why
		{
			t.AddResult(TLE);
		}
		else if (exitcode == 3 || (exitcode >> 30) == 3)
		{
			t.AddResult(RE);
		}
		else
		{
			t.CompareAnswer(s);
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
bool Problem::IsOnSegmentEnd(const istream & in, const CString & str, const CString & token)
{
	return in.eof() || (token.IsEmpty() ? false : str == token);
}
void Problem::MoveSubmitFile(const SubmitObject& s)
{
	CString targetname;
	if (type == Header)
	{
		targetname = config.GetData("HEADER", "include");
	}
	else
	{
		targetname = s.GetFileName();
	}
	MoveFileEx(s.GetFilePath(), R"(.\AJ_temp\)" + targetname, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);//TODO:檢查是否成功
}
void Problem::CopySubmitFile(const SubmitObject& s)
{
	CString targetname;
	if (type == Header)
	{
		targetname = config.GetData("HEADER", "include");
	}
	else
	{
		targetname = s.GetFileName();
	}
	CopyFile(s.GetFilePath(), TEXT(R"(.\AJ_temp\)" + targetname), FALSE);//TODO:檢查是否成功
}

void Problem::CopyMainFile()
{
	CopyFile(mainfile.GetFilePath(), TEXT(R"(.\AJ_temp\main.cpp)"), FALSE);//TODO:檢查是否成功
}
