#include "BasicException.h"

BasicException::BasicException(void)
{
}

BasicException::BasicException( const std::wstring& msg ) :_msg(msg)
{

}

BasicException::BasicException( const BasicException& rhv ) :_msg(rhv._msg)
{

}
BasicException::~BasicException(void)
{
}

BasicException& BasicException::operator=(const BasicException& rhv)
{
	if(this != &rhv)
	{
		_msg = rhv._msg;
	}
	return *this;
}

const std::wstring& BasicException::getMessage() const
{
	return _msg;
}