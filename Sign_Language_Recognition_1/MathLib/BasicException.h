#pragma once

#include <string>

class BasicException
{
private:
	std::wstring _msg;
public:
	BasicException(void);
	BasicException(const std::wstring& msg);
	BasicException(const BasicException& rhv);
	BasicException& operator=(const BasicException& rhv);
	const std::wstring& getMessage() const;
public:
	~BasicException(void);
};
