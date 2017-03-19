#include "SubmitObject.h"
#include"stdafx.h"
using namespace std;


SubmitObject::SubmitObject(const CFileFind& find, const FileType & type):AJFile(find), result(NJ)
{

}

SubmitObject::~SubmitObject()
{
}


void SubmitObject::SetResult(const TestCase& t,const JudgeResult& j)
{
	testedcase[t]= j;
	UpdateResult();
	//result = j;
}

void SubmitObject::SetFinalResult(const JudgeResult & j)
{
	result = j;
}

JudgeResult SubmitObject::GetFinalReault()
{
	UpdateResult();
	return result;
}

void SubmitObject::UpdateResult()
{
	if (result == NJ&&testedcase.size()>0)
	{
		result = AC;
		for (auto t : testedcase)
		{
			if (t.second != AC)
			{
				result = t.second;
			}
		}
	}
}
