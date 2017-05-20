#include "SubmitObject.h"
#include"stdafx.h"
using namespace std;


SubmitObject::SubmitObject(const CFileFind& find, const FileType & type):AJFile(find), result(NJ)
{

}

SubmitObject::~SubmitObject()
{
}


void SubmitObject::AddTestedCase(const TestCase& t)
{
	testedcase.push_back(t);
}

void SubmitObject::SetFinalResult(const JudgeResult & j)
{
	result = j;
}

JudgeResult SubmitObject::GetFinalReault()
{
	if (result == NJ)
	{
		UpdateResult();
	}
	return result;
}

const vector<TestCase>& SubmitObject::GetTestedCase()
{
	return testedcase;
}

void SubmitObject::UpdateResult()
{
	if (testedcase.empty())
	{
		result = NJ;
		return;
	}
	for (auto t : testedcase)
	{
		for (auto s : t.GetResultVector())
		{
			if (s != AC)
			{
				result = s;
				return;
			}
		}
	}
	result = AC;
	return;
}

int SubmitObject::GetACCount()
{
	int ans = 0;
	for (TestCase& t :testedcase)
	{
		for (JudgeResult r : t.GetResultVector())
		{
			if (r == AC)
			{
				ans++;
			}
		}
	}
	return ans;
}
