#include "IDManager.h"
using namespace Swift;

unsigned long IDManager::m_nIndex = 0;

IDManager::IDManager()
{

}

IDManager::~IDManager()
{

}

unsigned long IDManager::getID(IDType type)
{
	unsigned long newID;
	newID = type << 24;
	m_nIndex++;
	newID += m_nIndex;

	return newID;
}

IDType IDManager::getIDType(unsigned long nID)
{
	return static_cast<IDType>(nID >> 24);
}