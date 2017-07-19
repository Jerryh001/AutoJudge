#pragma once
#include"stdafx.h"
#include"OutputFile.h"
using namespace std;
class TestCase :public AJFile
{
	vector<JudgeResult> result;
	OutputFile outputfile;
	bool getline(CStdioFile& in, CString& str);
	JudgeResult SegmentCompare(const list<CString>&,const list<CString>&);
public:
	TestCase(const CString& input, const CString& output);
	TestCase(const TestCase& test);
	unsigned int CompareAnswer(SubmitObject& user);
	void Clear();
	JudgeResult GetResult();
	void AddResult(const JudgeResult& result);
	const vector<JudgeResult>& GetResultVector()const;
	size_t GetSegmentCount()const;
	void SetToken(const CString& token = "");
	CString GetToken()const;
	void SetSegmentSizeError();

};
