#pragma once
#include"stdafx.h"
#include"SubmitObject.h"
#include"TestCase.h"
using namespace std;
class Problem
{
	vector<SubmitObject> submitarray;
	vector<TestCase> testarray;
	CString path;
	FileType type;
	AJFile mainfile;
	bool partscore;
	int segment_count;
	Config config;
	int Compile(const SubmitObject& s);
	JudgeResult Execute(SubmitObject& s, TestCase t);

	bool IsOnSegmentEnd(const istream& in, const CString& str, const CString& token);
	void CopySubmitFile(const SubmitObject&);
	void MoveSubmitFile(const SubmitObject&);
	void CopyMainFile();
public:
	Problem(const CString& folder=R"(.\)");
	void ReadFile(const CString& casereg = R"([\s\S])");
	void Judge();
	CString GetPath()const;
	void SetSetting(const CString& path);
	static void SetGlobalSetting(const CString& path);
	void AutoJudge();
	
	
};

