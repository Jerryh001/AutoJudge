#include "TestCase.h"
#include"stdafx.h"
using namespace std;




TestCase::TestCase(const CFileFind& find, const string& token) :AJFile(find), segment_token(token), segment_count(1)
{
	if (!segment_token.empty())
	{
		CountSegment();
	}
}

void TestCase::Clear()
{
	result.clear();
}

JudgeResult TestCase::GetResult()
{
	for (JudgeResult& r : result)
	{
		if (r != AC)
		{
			return r;
		}
	}
	return AC;
}

void TestCase::AddResult(const JudgeResult& r)
{
	result.push_back(r);
}

const vector<JudgeResult>& TestCase::GetResultVector()
{
	return result;
}

const size_t& TestCase::GetSegmentCount()
{
	return segment_count;
}

string TestCase::GetToken()
{
	return segment_token;
}

void TestCase::SetSegmentSizeError()
{
	result.back() = WA;
}



TestCase::~TestCase()
{
}

void TestCase::CountSegment()
{
	ifstream in(string(Path).replace(Path.length() - Name.length() - 6, 2, "out"));
	string s;
	while (getline(in, s))
	{
		if (s == segment_token)
		{
			segment_count++;
		}
	}
}
