#include "TestCase.h"
#include"stdafx.h"
using namespace std;

TestCase::TestCase(const CString & input, const CString & output) :AJFile(input, Input), outputfile(output)
{
	outputfile.Read();
}

TestCase::TestCase(const TestCase & test) : AJFile(test), outputfile(test.outputfile), result(test.result)
{

}

unsigned int TestCase::CompareAnswer(SubmitObject & user)
{
	OutputFile userfile(R"(.\AJ_temp\)" + GetFileName());
	userfile.SetToken(GetToken());
	userfile.Read();
	for (size_t i = 0; i < outputfile.GetSegmentCount(); i++)
	{
		JudgeResult segresult = SegmentCompare(outputfile[i], userfile[i]);
		AddResult(segresult);
	}
	if (userfile.GetSegmentCount() > outputfile.GetSegmentCount())
	{
		SetSegmentSizeError();
	}
	return 0;//TODO:第一行不一樣的行數
}
bool TestCase::getline(CStdioFile& in, CString& str)
{
	return in.ReadString(str) && (outputfile.IsTokenEmpty() ? true : str != outputfile.GetToken());
}
JudgeResult TestCase::SegmentCompare(const list<CString>& ans, const list<CString>& user)
{
	struct TableCell
	{
		enum Direction { none, left, up, both } direction;
		int length;
	};
	JudgeResult result = (ans.size() == user.size() ? AC : PE);
	list<CString>::const_iterator itans, ituser;
	if (result == AC)
	{
		vector<vector<TableCell>> LCStable(ans.size(), vector<TableCell>(user.size(), TableCell{ TableCell::Direction::none,0 }));
		itans = ans.begin();
		for (size_t i = 0; i < ans.size(); i++, ++itans)
		{
			ituser = user.begin();
			for (size_t j = 0; j < user.size(); j++, ++ituser)
			{
				if (*itans == *ituser)
				{
					if (i == 0 || j == 0)
					{
						LCStable[i][j] = { TableCell::Direction::both,1 };
					}
					else
					{
						LCStable[i][j] = { TableCell::Direction::both,LCStable[i - 1][j - 1].length + 1 };
					}
				}
				else
				{
					int left = (j == 0 ? 0 : LCStable[i][j - 1].length);
					int up = (i == 0 ? 0 : LCStable[i - 1][j].length);
					LCStable[i][j] = { left > up ? TableCell::Direction::left : TableCell::Direction::up,max(left,up) };
				}
			}
		}
		if (ans.size()>0&&LCStable[ans.size() - 1][user.size() - 1].length != ans.size())
		{
			result = PE;
		}
	}
	if (result == PE)//TODO:也許可以重構把兩個合併
	{
		vector<vector<TableCell>> LCStable(ans.size(), vector<TableCell>(user.size(), TableCell{ TableCell::Direction::none,0 }));
		itans = ans.begin();
		size_t i,j;
		for (i=0; itans!=ans.end(); i++, ++itans)
		{
			CString ansstr = *itans;
			while (ansstr.Trim().IsEmpty())
			{
				if (++itans == ans.end()) goto endans;
				ansstr = *itans;
			}
			ituser = user.begin();
			for (j = 0;ituser!=user.end(); j++, ++ituser)
			{
				CString userstr = *ituser;
				while (userstr.Trim().IsEmpty())
				{
					if (++ituser == user.end()) goto enduser;
					userstr = *ituser;
				}
				if (ansstr.Trim() == userstr.Trim())
				{
					if (i == 0 || j == 0)
					{
						LCStable[i][j] = { TableCell::Direction::both,1 };
					}
					else
					{
						LCStable[i][j] = { TableCell::Direction::both,LCStable[i - 1][j - 1].length + 1 };
					}
				}
				else
				{
					int left = (j == 0 ? 0 : LCStable[i][j - 1].length);
					int up = (i == 0 ? 0 : LCStable[i - 1][j].length);
					LCStable[i][j] = { left > up ? TableCell::Direction::left : TableCell::Direction::up,max(left,up) };
				}
			enduser:;
			}
		endans:;
		}
		if (i==0||j==0||LCStable[i - 1][j - 1].length != i)
		{
			result = WA;
		}
	}
	return result;
}
void TestCase::Clear()
{
	result.clear();
}

JudgeResult TestCase::GetResult()
{
	bool foundpe = false;
	for (JudgeResult& r : result)
	{
		if (r != AC)
		{
			if (r == PE)
			{
				foundpe = true;
			}
			else
			{
				return r;
			}
		}
	}
	return foundpe ? PE : AC;
}

void TestCase::AddResult(const JudgeResult& r)
{
	result.push_back(r);
}

const vector<JudgeResult>& TestCase::GetResultVector() const
{
	return result;
}

size_t TestCase::GetSegmentCount() const
{
	return outputfile.GetSegmentCount();
}

void TestCase::SetToken(const CString& token)
{
	outputfile.SetToken(token);
	outputfile.Read();
}

CString TestCase::GetToken()const
{
	return outputfile.GetToken();
}

void TestCase::SetSegmentSizeError()
{
	if (result.empty()) result.push_back(WA);
	else result.back() = WA;
}

