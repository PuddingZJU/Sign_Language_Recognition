/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/10/28                                                  */
/************************************************************************/

#include "Feature.h"

using namespace Swift;

Feature::Feature()
{

}

Feature::~Feature()
{

}

IndividualFeature::IndividualFeature()
{
	m_nType = 0;
}

IndividualFeature::~IndividualFeature()
{

}

RelationalFeature::RelationalFeature()
{
	m_nType = 1;
	m_nOperation = 0;
}

RelationalFeature::~RelationalFeature()
{

}