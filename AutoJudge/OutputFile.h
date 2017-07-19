#pragma once
#include "stdafx.h"
class OutputFile : public AJFile
{
	size_t segment_count;
	CString segment_token;
	vector<list<CString>> answer;
public:
	OutputFile(const CString& path);
	//OutputFile(const OutputFile&);
	~OutputFile();

	void Read();
	void SetToken(const CString& token);
	bool IsTokenEmpty() const;
	CString GetToken() const;
	size_t GetSegmentCount() const;
	list<CString> operator[] (size_t t);
};

