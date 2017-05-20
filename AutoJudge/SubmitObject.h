#pragma once
#include"stdafx.h"
using namespace std;
class TestCase;
enum JudgeResult
{
	NJ, UN, AC, WA, CE, TLE, RE
};
class SubmitObject:public AJFile
{
	JudgeResult result;
	vector<TestCase> testedcase;
	public:
	SubmitObject(const CFileFind& find, const FileType& type = Code);
	~SubmitObject();
	//void SetResult(const TestCase&,const JudgeResult&);
	void AddTestedCase(const TestCase&);
	void SetFinalResult(const JudgeResult&);
	JudgeResult GetFinalReault();
	const vector<TestCase>& GetTestedCase();
	void UpdateResult();
	int GetACCount();
};

