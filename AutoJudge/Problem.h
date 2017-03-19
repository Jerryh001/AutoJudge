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
	int Compile(const SubmitObject& s);
public:
	Problem(const string& folder=R"(.\)");
	void ReadFile(const FileType& type = Code, const string& casereg = R"([\s\S])", string codereg = "", const string& main = "main.cpp");
	~Problem();
	void Judge();
	
	JudgeResult Result(SubmitObject, TestCase);

	unsigned int Compare(const TestCase& t);
	void CopySubmitFile(const SubmitObject&);
	void CopyMainFile();
};

