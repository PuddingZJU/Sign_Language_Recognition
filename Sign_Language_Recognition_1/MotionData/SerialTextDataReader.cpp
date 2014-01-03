/* by ywpg */
/* Last Modified: 2007/08/31 05:49:17 . */

#include "SerialTextDataReader.h"

#include <string>
using namespace Swift;
using namespace std;


SerialTextDataReader::SerialTextDataReader( std::istream* pstream) : m_pstream(pstream)
{
	this->m_pLine = NULL;
	this->m_currentline = 0;
}

SerialTextDataReader::SerialTextDataReader()
{
	m_pstream = NULL;
	this->m_pLine = NULL;
	this->m_currentline = 0;
}

SerialTextDataReader::~SerialTextDataReader()
{
	if (m_pLine)
	{
		delete m_pLine;
	}
	if (m_pstream)
	{
		delete m_pstream;
	}

}

int SerialTextDataReader::nextInt()
{
	int result;
	
	(*m_pLine) >> result;
	if (this->m_pLine->fail())
	{
		fillBuf();
		(*m_pLine) >> result;
	}

	return result;
	//int result;
	//*(this->m_pstream) >> result;
	//if (this->m_pstream->fail())
	//{
	//	throw std::runtime_error("Error when excepting an int-type data.");
	//}
	//return result;
}

double SerialTextDataReader::nextFloat()
{
	double result;
	
	(*m_pLine) >> result;

	if (this->m_pLine->fail())
	{
		fillBuf();
		(*m_pLine) >> result;
	}

	return result;
	//double result;
	//*(this->m_pstream) >> result;
	//if (this->m_pstream->fail())
	//{
	//	throw std::runtime_error("Error when excepting a double-type data.");
	//}

	//return result;
}

char SerialTextDataReader::nextChar()
{
	char result;
	(*m_pLine) >> result;

	if (this->m_pLine->fail())
	{
		fillBuf();
		(*m_pLine) >> result;
	}

	return result;
}

std::string SerialTextDataReader::nextWord()
{
	std::string result;
	
	(*m_pLine) >> result;
	
	if (this->m_pLine->fail())
	{
		fillBuf();
		(*m_pLine) >> result;
	}

	return result;
	//std::string result;
	//*(this->m_pstream) >> result;
	//if (this->m_pstream->fail())
	//{
	//	throw std::runtime_error("Error when excepting a string-type data.");
	//}

	//return result;
}

void SerialTextDataReader::SetSkipWriteSpace(bool b)
{
	if (b)
	{
		(*m_pstream) >> skipws;
		(*m_pLine) >> skipws;
	}
	else
	{
		(*m_pstream) >> noskipws;
		(*m_pLine) >> noskipws;
	}
}

bool SerialTextDataReader::checkToken(std::string strToken)
{
	//std::string str = this->nextWord(); 
	//return ( 0 ==str.compare(strToken) );
	return ( 0 == strToken.compare(this->nextWord()) );
}

long SerialTextDataReader::getCurrentLineNo()
{
	return m_currentline;
}

void SerialTextDataReader::fillBuf()
{
	if(!m_pLine || m_pLine->fail() || m_pLine->eof())
	{
		// try to fill the buf again.
		std::getline((*m_pstream), m_strLine, '\n');
		m_currentline ++;
		if (m_pLine) 
			delete m_pLine;
		m_pLine = new std::istringstream(m_strLine);
		
	}

}

