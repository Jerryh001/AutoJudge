#pragma once
#include"stdafx.h"
#include"SubmitObject.h"
#include"TestCase.h"
using namespace std;
class Problem
{
	vector<SubmitObject> submitarray;
	vector<TestCase> testarray;
	string path;
	FileType type;
	AJFile mainfile;
	bool partscore;
	int segment_count;
	Config config;
	int Compile(const SubmitObject& s);
	JudgeResult Execute(SubmitObject& s, TestCase t);

	unsigned int Compare(SubmitObject&, TestCase&);
	bool IsOnSegmentEnd(const istream& in, const string& str, const string& token);
	void CopySubmitFile(const SubmitObject&);
	void MoveSubmitFile(const SubmitObject&);
	void CopyMainFile();
	//void GetTargetName();
public:
	Problem(const string& folder=R"(.\)", const bool& part=false);
	void ReadFile(const string& main = "main.cpp", string codereg = "", const string& casereg = R"([\s\S])");
	~Problem();
	void Judge();
	
	
};

