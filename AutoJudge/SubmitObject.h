#pragma once
#include"stdafx.h"
using namespace std;
class TestCase;
enum JudgeResult
{
	NJ, UN, AC ,PE, WA, CE, TLE, RE
};
class SubmitObject:public AJFile
{
	JudgeResult result;
	vector<TestCase> testedcase;
	public:
	SubmitObject(const CString& path, const FileType& type = Code);
	void AddTestedCase(const TestCase&);
	void SetFinalResult(const JudgeResult&);
	JudgeResult GetFinalReault();
	const vector<TestCase>& GetTestedCase();
	void UpdateResult();
	int GetACCount()const;
};

