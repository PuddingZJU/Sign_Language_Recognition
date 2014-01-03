/************************************************************************/
/* By xiubo.liang@gmail.com                                             */
/* Created: 2010/11/23                                                  */
/************************************************************************/

#include <string>
#include <map>
#include "../SwiftModel/SwiftModel.h"
#include "FeatureParser.h"

using namespace Swift;

FeatureParser::FeatureParser()
{

}

FeatureParser::~FeatureParser()
{

}

bool FeatureParser::calFeatureValue(int nID, PriFeatureClip* ppfc, double& dValue)
{
	if (!ppfc)
		return false;

	std::map<int, Feature*>::iterator it=SwiftModel::instance().m_pfm->m_mFeatureSet.find(nID);
	if (it == SwiftModel::instance().m_pfm->m_mFeatureSet.end())
		return false;

	IndividualFeature* pif;
	RelationalFeature* prf;
	switch(it->second->m_nSubset)
	{
	case FLEXION:
		{
			if (it->second->m_nType == 0)
			{
				pif = (IndividualFeature*)it->second;
				std::map<std::string, Flexion>::iterator it2=ppfc->m_mFlexion.find(pif->m_strName);
				if (it2 == ppfc->m_mFlexion.end())
					return false;
				switch(pif->m_nIdentity)
				{
				case MEAN:
					dValue = it2->second.dFlexMean;
					break;
				case VARIANCE:
					dValue = it2->second.dFlexVar;
					break;
				case MARGIN:
					dValue = it2->second.dFlexMar;
				    break;
				case TURNINGPOINTS:
					dValue = it2->second.nFlexTurn;
				    break;
				}
			}
			else if (it->second->m_nType == 1)
			{
				prf = (RelationalFeature*)it->second;
				dValue = 0.0;
				for (int i=0; i<prf->m_vGroup2.size(); i++)
				{
					std::map<std::string, Flexion>::iterator it3=ppfc->m_mFlexion.find(prf->m_vGroup2.at(i));
					if (it3 == ppfc->m_mFlexion.end())
						return false;
					switch(prf->m_nIdentity)
					{
					case MEAN:
						dValue += it3->second.dFlexMean;
						break;
					case VARIANCE:
						dValue += it3->second.dFlexVar;
						break;
					case MARGIN:
						dValue += it3->second.dFlexMar;
						break;
					case TURNINGPOINTS:
						dValue += it3->second.nFlexTurn;
						break;
					}
				}
				for (int i=0; i<prf->m_vGroup1.size(); i++)
				{
					std::map<std::string, Flexion>::iterator it4=ppfc->m_mFlexion.find(prf->m_vGroup1.at(i));
					if (it4 == ppfc->m_mFlexion.end())
						return false;
					if (prf->m_nOperation == 0)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue += it4->second.dFlexMean;
							break;
						case VARIANCE:
							dValue += it4->second.dFlexVar;
							break;
						case MARGIN:
							dValue += it4->second.dFlexMar;
							break;
						case TURNINGPOINTS:
							dValue += it4->second.nFlexTurn;
							break;
						}
					}
					else if (prf->m_nOperation == 1)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue -= it4->second.dFlexMean;
							break;
						case VARIANCE:
							dValue -= it4->second.dFlexVar;
							break;
						case MARGIN:
							dValue -= it4->second.dFlexMar;
							break;
						case TURNINGPOINTS:
							dValue -= it4->second.nFlexTurn;
							break;
						}
					}
				}
			}
		}
		break;
	case ABDUCTION:
		{
			if (it->second->m_nType == 0)
			{
				pif = (IndividualFeature*)it->second;
				std::map<std::string, Abduction>::iterator it2=ppfc->m_mAbduction.find(pif->m_strName);
				if (it2 == ppfc->m_mAbduction.end())
					return false;
				switch(pif->m_nIdentity)
				{
				case MEAN:
					dValue = it2->second.dAbdMean;
					break;
				case VARIANCE:
					dValue = it2->second.dAbdVar;
					break;
				case MARGIN:
					dValue = it2->second.dAbdMar;
					break;
				case TURNINGPOINTS:
					dValue = it2->second.nAbdTurn;
					break;
				}
			}
			else if (it->second->m_nType == 1)
			{
				prf = (RelationalFeature*)it->second;
				dValue = 0.0;
				for (int i=0; i<prf->m_vGroup2.size(); i++)
				{
					std::map<std::string, Abduction>::iterator it3=ppfc->m_mAbduction.find(prf->m_vGroup2.at(i));
					if (it3 == ppfc->m_mAbduction.end())
						return false;
					switch(prf->m_nIdentity)
					{
					case MEAN:
						dValue += it3->second.dAbdMean;
						break;
					case VARIANCE:
						dValue += it3->second.dAbdVar;
						break;
					case MARGIN:
						dValue += it3->second.dAbdMar;
						break;
					case TURNINGPOINTS:
						dValue += it3->second.nAbdTurn;
						break;
					}
				}
				for (int i=0; i<prf->m_vGroup1.size(); i++)
				{
					std::map<std::string, Abduction>::iterator it4=ppfc->m_mAbduction.find(prf->m_vGroup1.at(i));
					if (it4 == ppfc->m_mAbduction.end())
						return false;
					if (prf->m_nOperation == 0)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue += it4->second.dAbdMean;
							break;
						case VARIANCE:
							dValue += it4->second.dAbdVar;
							break;
						case MARGIN:
							dValue += it4->second.dAbdMar;
							break;
						case TURNINGPOINTS:
							dValue += it4->second.nAbdTurn;
							break;
						}
					}
					else if (prf->m_nOperation == 1)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue -= it4->second.dAbdMean;
							break;
						case VARIANCE:
							dValue -= it4->second.dAbdVar;
							break;
						case MARGIN:
							dValue -= it4->second.dAbdMar;
							break;
						case TURNINGPOINTS:
							dValue -= it4->second.nAbdTurn;
							break;
						}
					}
				}
			}
		}
		break;
	case POSITION:
		{
			if (it->second->m_nType == 0)
			{
				pif = (IndividualFeature*)it->second;
				std::map<std::string, Position>::iterator it2=ppfc->m_mPosition.find(pif->m_strName);
				if (it2 == ppfc->m_mPosition.end())
					return false;
				switch(pif->m_nIdentity)
				{
				case MEAN:
					dValue = it2->second.dDistMean;
					break;
				case VARIANCE:
					dValue = it2->second.dDistVar;
					break;
				case MARGIN:
					dValue = it2->second.dDistMar;
					break;
				case TURNINGPOINTS:
					dValue = it2->second.nDistTurn;
					break;
				}
			}
			else if (it->second->m_nType == 1)
			{
				prf = (RelationalFeature*)it->second;
				dValue = 0.0;
				for (int i=0; i<prf->m_vGroup2.size(); i++)
				{
					std::map<std::string, Position>::iterator it3=ppfc->m_mPosition.find(prf->m_vGroup2.at(i));
					if (it3 == ppfc->m_mPosition.end())
						return false;
					switch(prf->m_nIdentity)
					{
					case MEAN:
						dValue += it3->second.dDistMean;
						break;
					case VARIANCE:
						dValue += it3->second.dDistVar;
						break;
					case MARGIN:
						dValue += it3->second.dDistMar;
						break;
					case TURNINGPOINTS:
						dValue += it3->second.nDistTurn;
						break;
					}
				}
				for (int i=0; i<prf->m_vGroup1.size(); i++)
				{
					std::map<std::string, Position>::iterator it4=ppfc->m_mPosition.find(prf->m_vGroup1.at(i));
					if (it4 == ppfc->m_mPosition.end())
						return false;
					if (prf->m_nOperation == 0)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue += it4->second.dDistMean;
							break;
						case VARIANCE:
							dValue += it4->second.dDistVar;
							break;
						case MARGIN:
							dValue += it4->second.dDistMar;
							break;
						case TURNINGPOINTS:
							dValue += it4->second.nDistTurn;
							break;
						}
					}
					else if (prf->m_nOperation == 1)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							dValue -= it4->second.dDistMean;
							break;
						case VARIANCE:
							dValue -= it4->second.dDistVar;
							break;
						case MARGIN:
							dValue -= it4->second.dDistMar;
							break;
						case TURNINGPOINTS:
							dValue -= it4->second.nDistTurn;
							break;
						}
					}
				}
			}
		}
	    break;
	case ORIENTATION:
		{
			if (it->second->m_nType == 0)
			{
				pif = (IndividualFeature*)it->second;
				std::map<std::string, Orientation>::iterator it2=ppfc->m_mOrientation.find(pif->m_strName);
				if (it2 == ppfc->m_mOrientation.end())
					return false;
				switch(pif->m_nIdentity)
				{
				case MEAN:
					{
						if (pif->m_strAddInfo == "X")
							dValue = it2->second.dXOriMean;
						else if (pif->m_strAddInfo == "Y")
							dValue = it2->second.dYOriMean;
						else if (pif->m_strAddInfo == "Z")
							dValue = it2->second.dZOriMean;
					}
					break;
				case VARIANCE:
					{
						if (pif->m_strAddInfo == "X")
							dValue = it2->second.dXOriVar;
						else if (pif->m_strAddInfo == "Y")
							dValue = it2->second.dYOriVar;
						else if (pif->m_strAddInfo == "Z")
							dValue = it2->second.dZOriVar;
					}
					break;
				case MARGIN:
					{
						if (pif->m_strAddInfo == "X")
							dValue = it2->second.dXOriMar;
						else if (pif->m_strAddInfo == "Y")
							dValue = it2->second.dYOriMar;
						else if (pif->m_strAddInfo == "Z")
							dValue = it2->second.dZOriMar;
					}
					break;
				case TURNINGPOINTS:
					{
						if (pif->m_strAddInfo == "X")
							dValue = it2->second.nXOriTurn;
						else if (pif->m_strAddInfo == "Y")
							dValue = it2->second.nYOriTurn;
						else if (pif->m_strAddInfo == "Z")
							dValue = it2->second.nZOriTurn;
					}
					break;
				}
			}
			else if (it->second->m_nType == 1)
			{
				prf = (RelationalFeature*)it->second;
				dValue = 0.0;
				for (int i=0; i<prf->m_vGroup2.size(); i++)
				{
					std::map<std::string, Orientation>::iterator it3=ppfc->m_mOrientation.find(prf->m_vGroup2.at(i));
					if (it3 == ppfc->m_mOrientation.end())
						return false;
					switch(prf->m_nIdentity)
					{
					case MEAN:
						{
							if (prf->m_strAddInfo == "X")
								dValue += it3->second.dXOriMean;
							else if (prf->m_strAddInfo == "Y")
								dValue += it3->second.dYOriMean;
							else if (prf->m_strAddInfo == "Z")
								dValue += it3->second.dZOriMean;
						}
						break;
					case VARIANCE:
						{
							if (prf->m_strAddInfo == "X")
								dValue += it3->second.dXOriVar;
							else if (prf->m_strAddInfo == "Y")
								dValue += it3->second.dYOriVar;
							else if (prf->m_strAddInfo == "Z")
								dValue += it3->second.dZOriVar;
						}
						break;
					case MARGIN:
						{
							if (prf->m_strAddInfo == "X")
								dValue += it3->second.dXOriMar;
							else if (prf->m_strAddInfo == "Y")
								dValue += it3->second.dYOriMar;
							else if (prf->m_strAddInfo == "Z")
								dValue += it3->second.dZOriMar;
						}
						break;
					case TURNINGPOINTS:
						{
							if (prf->m_strAddInfo == "X")
								dValue += it3->second.nXOriTurn;
							else if (prf->m_strAddInfo == "Y")
								dValue += it3->second.nYOriTurn;
							else if (prf->m_strAddInfo == "Z")
								dValue += it3->second.nZOriTurn;
						}
						break;
					}
				}
				for (int i=0; i<prf->m_vGroup1.size(); i++)
				{
					std::map<std::string, Orientation>::iterator it4=ppfc->m_mOrientation.find(prf->m_vGroup1.at(i));
					if (it4 == ppfc->m_mOrientation.end())
						return false;
					if (prf->m_nOperation == 0)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							{
								if (prf->m_strAddInfo == "X")
									dValue += it4->second.dXOriMean;
								else if (prf->m_strAddInfo == "Y")
									dValue += it4->second.dYOriMean;
								else if (prf->m_strAddInfo == "Z")
									dValue += it4->second.dZOriMean;
							}
							break;
						case VARIANCE:
							{
								if (prf->m_strAddInfo == "X")
									dValue += it4->second.dXOriVar;
								else if (prf->m_strAddInfo == "Y")
									dValue += it4->second.dYOriVar;
								else if (prf->m_strAddInfo == "Z")
									dValue += it4->second.dZOriVar;
							}
							break;
						case MARGIN:
							{
								if (prf->m_strAddInfo == "X")
									dValue += it4->second.dXOriMar;
								else if (prf->m_strAddInfo == "Y")
									dValue += it4->second.dYOriMar;
								else if (prf->m_strAddInfo == "Z")
									dValue += it4->second.dZOriMar;
							}
							break;
						case TURNINGPOINTS:
							{
								if (prf->m_strAddInfo == "X")
									dValue += it4->second.nXOriTurn;
								else if (prf->m_strAddInfo == "Y")
									dValue += it4->second.nYOriTurn;
								else if (prf->m_strAddInfo == "Z")
									dValue += it4->second.nZOriTurn;
							}
							break;
						}
					}
					else if (prf->m_nOperation == 1)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							{
								if (prf->m_strAddInfo == "X")
									dValue -= it4->second.dXOriMean;
								else if (prf->m_strAddInfo == "Y")
									dValue -= it4->second.dYOriMean;
								else if (prf->m_strAddInfo == "Z")
									dValue -= it4->second.dZOriMean;
							}
							break;
						case VARIANCE:
							{
								if (prf->m_strAddInfo == "X")
									dValue -= it4->second.dXOriVar;
								else if (prf->m_strAddInfo == "Y")
									dValue -= it4->second.dYOriVar;
								else if (prf->m_strAddInfo == "Z")
									dValue -= it4->second.dZOriVar;
							}
							break;
						case MARGIN:
							{
								if (prf->m_strAddInfo == "X")
									dValue -= it4->second.dXOriMar;
								else if (prf->m_strAddInfo == "Y")
									dValue -= it4->second.dYOriMar;
								else if (prf->m_strAddInfo == "Z")
									dValue -= it4->second.dZOriMar;
							}
							break;
						case TURNINGPOINTS:
							{
								if (prf->m_strAddInfo == "X")
									dValue -= it4->second.nXOriTurn;
								else if (prf->m_strAddInfo == "Y")
									dValue -= it4->second.nYOriTurn;
								else if (prf->m_strAddInfo == "Z")
									dValue -= it4->second.nZOriTurn;
							}
							break;
						}
					}
				}
			}
		}
	    break;
	case MOVEMENT:
		{
			if (it->second->m_nType == 0)
			{
				pif = (IndividualFeature*)it->second;
				std::map<std::string, Movement>::iterator it2=ppfc->m_mMovement.find(pif->m_strName);
				if (it2 == ppfc->m_mMovement.end())
					return false;
				switch(pif->m_nIdentity)
				{
				case MEAN:
					{
						if (pif->m_strAddInfo == "Speed")
							dValue = it2->second.dSpeedMean;
						else if (pif->m_strAddInfo == "Acceleration")
							dValue = it2->second.dAccMean;
					}
					break;
				case VARIANCE:
					{
						if (pif->m_strAddInfo == "Speed")
							dValue = it2->second.dSpeedVar;
						else if (pif->m_strAddInfo == "Acceleration")
							dValue = it2->second.dAccVar;
					}
					break;
				case MARGIN:
					{
						if (pif->m_strAddInfo == "Speed")
							dValue = it2->second.dSpeedMar;
						else if (pif->m_strAddInfo == "Acceleration")
							dValue = it2->second.dAccMar;
					}
					break;
				case TURNINGPOINTS:
					{
						if (pif->m_strAddInfo == "Speed")
							dValue = it2->second.nSpeedTurn;
						else if (pif->m_strAddInfo == "Acceleration")
							dValue = it2->second.nAccTurn;
					}
					break;
				}
			}
			else if (it->second->m_nType == 1)
			{
				prf = (RelationalFeature*)it->second;
				dValue = 0.0;
				for (int i=0; i<prf->m_vGroup2.size(); i++)
				{
					std::map<std::string, Movement>::iterator it3=ppfc->m_mMovement.find(prf->m_vGroup2.at(i));
					if (it3 == ppfc->m_mMovement.end())
						return false;
					switch(prf->m_nIdentity)
					{
					case MEAN:
						{
							if (prf->m_strAddInfo == "Speed")
								dValue += it3->second.dSpeedMean;
							else if (prf->m_strAddInfo == "Acceleration")
								dValue += it3->second.dAccMean;
						}
						break;
					case VARIANCE:
						{
							if (prf->m_strAddInfo == "Speed")
								dValue += it3->second.dSpeedVar;
							else if (prf->m_strAddInfo == "Acceleration")
								dValue += it3->second.dAccVar;
						}
						break;
					case MARGIN:
						{
							if (prf->m_strAddInfo == "Speed")
								dValue += it3->second.dSpeedMar;
							else if (prf->m_strAddInfo == "Acceleration")
								dValue += it3->second.dAccMar;
						}
						break;
					case TURNINGPOINTS:
						{
							if (prf->m_strAddInfo == "Speed")
								dValue += it3->second.nSpeedTurn;
							else if (prf->m_strAddInfo == "Acceleration")
								dValue += it3->second.nAccTurn;
						}
						break;
					}
				}
				for (int i=0; i<prf->m_vGroup1.size(); i++)
				{
					std::map<std::string, Movement>::iterator it4=ppfc->m_mMovement.find(prf->m_vGroup1.at(i));
					if (it4 == ppfc->m_mMovement.end())
						return false;
					if (prf->m_nOperation == 0)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue += it4->second.dSpeedMean;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue += it4->second.dAccMean;
							}
							break;
						case VARIANCE:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue += it4->second.dSpeedVar;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue += it4->second.dAccVar;
							}
							break;
						case MARGIN:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue += it4->second.dSpeedMar;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue += it4->second.dAccMar;
							}
							break;
						case TURNINGPOINTS:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue += it4->second.nSpeedTurn;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue += it4->second.nAccTurn;
							}
							break;
						}
					}
					else if (prf->m_nOperation == 1)
					{
						switch(prf->m_nIdentity)
						{
						case MEAN:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue -= it4->second.dSpeedMean;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue -= it4->second.dAccMean;
							}
							break;
						case VARIANCE:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue -= it4->second.dSpeedVar;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue -= it4->second.dAccVar;
							}
							break;
						case MARGIN:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue -= it4->second.dSpeedMar;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue -= it4->second.dAccMar;
							}
							break;
						case TURNINGPOINTS:
							{
								if (prf->m_strAddInfo == "Speed")
									dValue -= it4->second.nSpeedTurn;
								else if (prf->m_strAddInfo == "Acceleration")
									dValue -= it4->second.nAccTurn;
							}
							break;
						}
					}
				}
			}
		}
	    break;
	}

	return true;
}

Feature* FeatureParser::findFeature(int nID)
{
	std::map<int, Feature*>::iterator it=SwiftModel::instance().m_pfm->m_mFeatureSet.find(nID);
	if (it != SwiftModel::instance().m_pfm->m_mFeatureSet.end())
	{
		return it->second;
	}
	
	return NULL;
}