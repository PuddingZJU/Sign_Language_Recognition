#include "TraceTable.h"
using namespace Swift;

TraceTable::TraceTable()
{

}

TraceTable::~TraceTable()
{
	if (!m_mIDPointer.empty())
	{
		m_mIDPointer.clear();
	}
	if (!m_mNamePointer.empty())
	{
		m_mNamePointer.clear();
	}
}

Traceable* TraceTable::getPointerByID(const unsigned long nID)
{
	std::map<unsigned long, Traceable*>::iterator it;
	it = m_mIDPointer.find(nID);
	if (it != m_mIDPointer.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

Traceable* TraceTable::getPointerByName(const QString& strName)
{
	std::map<QString, Traceable*>::iterator it;
	it = m_mNamePointer.find(strName);
	if (it != m_mNamePointer.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void TraceTable::addTraceRecord(Traceable* pTraceable)
{
	if (pTraceable)
	{
		m_mIDPointer.insert(std::pair<int, Traceable*>(pTraceable->getID(), pTraceable));
		m_mNamePointer.insert(std::pair<QString, Traceable*>(pTraceable->getName(), pTraceable));
	}
	else
	{
		return;
	}
}

bool TraceTable::deleteTraceRecord(const unsigned long nID)
{
	std::map<unsigned long, Traceable*>::iterator it;
	it = m_mIDPointer.find(nID);
	if (it != m_mIDPointer.end())
	{
		m_mIDPointer.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

bool TraceTable::deleteTraceRecord(const QString& strName)
{
	std::map<QString, Traceable*>::iterator it;
	it = m_mNamePointer.find(strName);
	if (it != m_mNamePointer.end())
	{
		m_mNamePointer.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

void TraceTable::deleteAllTraceRecords()
{
	m_mIDPointer.clear();
	m_mNamePointer.clear();
}