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
	map<TestCase, JudgeResult> testedcase;
	public:
	SubmitObject(const CFileFind& find, const FileType& type = Code);
	~SubmitObject();
	void SetResult(const TestCase&,const JudgeResult&);
	void SetFinalResult(const JudgeResult&);
	JudgeResult GetFinalReault();
	void UpdateResult();
};

