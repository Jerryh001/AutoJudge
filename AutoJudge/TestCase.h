#pragma once
#include"stdafx.h"
using namespace std;
class TestCase :public AJFile
{
	size_t segment_count;
	string segment_token;
	vector<JudgeResult> result;
	void CountSegment();
public:
	TestCase(const CFileFind& find,const string& token="");
	void Clear();
	JudgeResult GetResult();
	void AddResult(const JudgeResult& result);
	const vector<JudgeResult>& GetResultVector();
	const size_t& GetSegmentCount();
	string GetToken();
	void SetSegmentSizeError();
	~TestCase();

};
