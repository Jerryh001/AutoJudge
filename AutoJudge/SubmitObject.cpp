#include "SubmitObject.h"
#include"stdafx.h"
using namespace std;


SubmitObject::SubmitObject(const CString& path, const FileType & type):AJFile(path,type), result(NJ)
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
	for (auto& t : testedcase)
	{
		for (auto& s : t.GetResultVector())
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

int SubmitObject::GetACCount()const
{
	int ans = 0;
	for (const TestCase& t :testedcase)
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
