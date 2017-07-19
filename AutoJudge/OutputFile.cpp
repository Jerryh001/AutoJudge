#include "stdafx.h"
#include "OutputFile.h"



OutputFile::OutputFile(const CString & path):AJFile(path,FileType::Output)
{
}

OutputFile::~OutputFile()
{

}
void OutputFile::Read()
{
	answer.clear();
	CStdioFile file;
	if (file.Open(filepath, CFile::modeRead | CFile::typeText))
	{
		CString s;
		list<CString> seg;
		while (file.ReadString(s))
		{
			if (!segment_token.IsEmpty() && s == segment_token)
			{
				answer.push_back(seg);
			}
			else
			{
				seg.push_back(s);
			}
		}
		answer.push_back(seg);
	}
	else
	{
		answer.resize(1);
	}

}
void OutputFile::SetToken(const CString& token)
{
	segment_token = token;
}
bool OutputFile::IsTokenEmpty() const
{
	return segment_token.IsEmpty();
}
CString OutputFile::GetToken()const
{
	return segment_token;
}
size_t OutputFile::GetSegmentCount() const
{
	return answer.size();
}

list<CString> OutputFile::operator[](size_t t)
{
	if (t >= answer.size()) return list<CString>();
	return answer[t];
}
