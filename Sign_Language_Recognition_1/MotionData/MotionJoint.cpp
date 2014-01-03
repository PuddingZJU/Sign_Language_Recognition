/* by ywpg */
/* Last Modified: 2007/08/31 05:52:56 . */

#include <exception>
#include <fstream>

#include "MotionJoint.h"

using namespace Swift;


MotionJoint::MotionJoint()
{
	m_pParent = NULL;
	m_pRotationData = NULL;
	m_pQuaternionData = NULL;
	m_pPositionData = NULL;
	m_pScaleData = NULL;
	m_strName = "";

	m_JointType = NORMAL_JOINT;
	m_currentFrame = 0;

	m_bSelected = false;
	m_bNeedUpdate= true;
	m_bInheritOrientation = true;
	m_bInheritScale = false; 

	m_RotationOrder = Euler::ZXY;

}

MotionJoint::~MotionJoint()
{
	// safe delete
	if (m_pRotationData)
	{
		delete m_pRotationData;
	}

	if (m_pQuaternionData)
	{
		delete m_pQuaternionData;
	}

	if (m_pScaleData)
	{
		delete m_pScaleData;
	}

	if (m_pPositionData)
	{
		delete m_pPositionData;
	}

}

MotionJoint* MotionJoint::CloneAll()
{
	MotionJoint * pmj = new MotionJoint();

	pmj->m_strName = this->m_strName;
	pmj->m_nIndex  = this->m_nIndex;  // updateIndex() is called when copy MotionClip.
	//pmj->m_bDataChanged = this->m_bDataChanged;
	pmj->m_currentFrame = 0;
	pmj->m_Position = this->m_Position;
	//pmj->m_currentQuaternion = this->m_currentQuaternion;
	//pmj->m_currentRotation = this->m_currentRotation;
	pmj->m_fXOffset = this->m_fXOffset;
	pmj->m_fYOffset = this->m_fYOffset;
	pmj->m_fZOffset = this->m_fZOffset;
	pmj->m_JointType = this->m_JointType;
	//pmj->m_bNeedUpdate = this ->m_bNeedUpdate;
	pmj->m_bNeedUpdate = true;
	pmj->m_RotationOrder = this->m_RotationOrder;

	if (this->m_pRotationData)
	{
		pmj->setRotationDataPointer( new std::vector<Vector3> (*(this->m_pRotationData)));
	}

	if (this->m_pQuaternionData)
	{
		pmj->setQuaternionDataPointer(new std::vector<Quaternion> (*(this->m_pQuaternionData)));
	}

	if (this->m_pPositionData)
	{
		pmj->setPositionDataPointer(new std::vector<Vector3> (*(this->m_pPositionData)));
	}

	if (this->m_pScaleData)
	{
		pmj->setScaleDataPointer(new std::vector<Vector3> (*(this->m_pScaleData)));
	}

	return pmj;
}

MotionJoint* MotionJoint::CloneSketelon()
{
	MotionJoint * pmj = new MotionJoint();

	pmj->m_strName = this->m_strName;
	pmj->m_nIndex  = this->m_nIndex;  // TODO: consider it
	//pmj->m_bDataChanged = this->m_bDataChanged;
	// !!current frame is 0 for new skeleton!!
	pmj->m_currentFrame = 0;
	pmj->m_Position = this->m_Position;
	//pmj->m_currentQuaternion = this->m_currentQuaternion;
	//pmj->m_currentRotation = this->m_currentRotation;
	pmj->m_fXOffset = this->m_fXOffset;
	pmj->m_fYOffset = this->m_fYOffset;
	pmj->m_fZOffset = this->m_fZOffset;
	pmj->m_JointType = this->m_JointType;
	pmj->m_bNeedUpdate = true;

	if (this->m_pRotationData)
	{
		pmj->setRotationDataPointer(new std::vector<Vector3>);
	}

	if (this->m_pQuaternionData)
	{
		pmj->setQuaternionDataPointer(new std::vector<Quaternion> );
	}

	if (this->m_pPositionData)
	{
		pmj->setPositionDataPointer(new std::vector<Vector3>);
	}

	if (this->m_pScaleData)
	{
		pmj->setScaleDataPointer(new std::vector<Vector3>);
	}

	return pmj;
}

std::string MotionJoint::getName() const
{
	return m_strName;
}


void MotionJoint::setName(std::string name) // called by friend class MotionClip, setJointName
{
	m_strName = name;
	return ;
}


Vector3 MotionJoint::getRelativePosition(int frameno) 
{
	_updateDataFromParent(frameno);
	return m_RelativePosition;
}


void MotionJoint::setRelativePosition(Vector3 pos, int frameno)
{
	this->setCurrentFrame(frameno); 
	m_Position = pos;
	return ;
}

Vector3 MotionJoint::getAbsolutePosition(int frameno) 
{
	return _getInheritPosition(frameno);
}


void MotionJoint::setAbsolutePosition(Vector3 pos, int frameno)
{
	if (this->m_JointType == ROOT_JOINT)
	{
		m_bNeedUpdate = true;
		(*(this->getPositionDataPointer())).at(frameno) = pos;
	}
	return ; 
}

void MotionJoint::setAbsolutePositionX(double posX, int frameno)
{
	if (this->m_JointType == ROOT_JOINT)
	{
		m_bNeedUpdate = true;
		Vector3 pos = (*(this->getPositionDataPointer())).at(frameno);
		pos.set_x(posX);

		(*(this->getPositionDataPointer())).at(frameno) = pos;
	}
	return ; 
}

void MotionJoint::setAbsolutePositionY(double posY, int frameno)
{
	if (this->m_JointType == ROOT_JOINT)
	{
		m_bNeedUpdate = true;
		Vector3 pos = (*(this->getPositionDataPointer())).at(frameno);
		pos.set_y(posY);

		(*(this->getPositionDataPointer())).at(frameno) = pos;
	}
	return ; 
}

void MotionJoint::setAbsolutePositionZ(double posZ, int frameno)
{
	if (this->m_JointType == ROOT_JOINT)
	{
		m_bNeedUpdate = true;
		Vector3 pos = (*(this->getPositionDataPointer())).at(frameno);
		pos.set_z (posZ);

		(*(this->getPositionDataPointer())).at(frameno) = pos;
	}
	return ; 

}

Quaternion MotionJoint::getOrientation(int frameno) 
{
	if (this->m_JointType != END_SITE)
		return (*(this->m_pQuaternionData)).at(frameno);
	else
		return Quaternion::IDENTITY;
}


void MotionJoint::setOrientation(Quaternion quat, int frameno)
{
	if (m_JointType != END_SITE)
	{
		//this->m_bNeedUpdate = true;
		_setUpdateFlagRecursively(true);
		(*(this->m_pQuaternionData)).at(frameno) = quat;
		(*(this->m_pRotationData)).at(frameno) = Vector3(quat.toEuler(m_RotationOrder));
		return ; //TODO
	}
}



double MotionJoint::getXOffset() 
{
	return m_fXOffset;
}


void MotionJoint::setXOffset(double off)
{
	m_fXOffset = off;
}


double MotionJoint::getYOffset() 
{
	return m_fYOffset;
}


void MotionJoint::setYOffset(double off)
{
	m_fYOffset = off;
}


double MotionJoint::getZOffset() 
{
	return m_fZOffset;
}


void MotionJoint::setZOffset(double off)
{
	m_fZOffset = off;
}

Vector3 MotionJoint::getXYZOffset() 
{
	return Vector3(m_fXOffset, m_fYOffset, m_fZOffset);
}

void MotionJoint::setXYZOffset(Vector3& vec)
{
	m_fXOffset = vec.x();
	m_fYOffset = vec.y();
	m_fZOffset = vec.z();
}

int MotionJoint::getIndex() const
{
	return m_nIndex;
}

double MotionJoint::getXRotation(int frameno) 
{
	if (this->m_JointType == END_SITE)
	{
		return 0.0f;
	}
	else
	{
		return (*(this->m_pRotationData)).at(frameno).x();
	}
}

double MotionJoint::getYRotation(int frameno) 
{
	//setCurrentFrame(frameno); ///TODO:!!!未必要切换?
	//return m_currentRotation.y;

	//checkFrameNumberRange(frameno);
	if (this->m_JointType == END_SITE)
	{
		return 0.0f;
	}
	else
	{
		return (*(this->m_pRotationData)).at(frameno).y();
	}
}

double MotionJoint::getZRotation(int frameno) 
{
	//setCurrentFrame(frameno); ///TODO:!!!未必要切换?
	//return m_currentRotation.z;

	//checkFrameNumberRange(frameno);
	if (this->m_JointType == END_SITE)
	{
		return 0.0f;
	}
	else
	{
		return (*(this->m_pRotationData)).at(frameno).z();
	}
}

Vector3 MotionJoint::getRotation(int frameno) 
{
	//setCurrentFrame(frameno); ///TODO:!!!未必要切换?
	//return Vector3(m_currentRotation);

	//checkFrameNumberRange(frameno);
	if (this->m_JointType == END_SITE)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		return (*(this->m_pRotationData)).at(frameno);
	}
}


void MotionJoint::setRotation(double x, double y, double z, int frameno)
{
	//this->setCurrentFrame(frameno); ///TODO:!!!未必要切换?/
	//m_currentRotation.x = x;
	//m_currentRotation.y = y;
	//m_currentRotation.z = z;
	//this->m_bDataChanged = true;

	//checkFrameNumberRange(frameno);
	if (frameno == m_currentFrame)
		this->m_bNeedUpdate = true;

	Quaternion q;
	(*(this->m_pRotationData)).at(frameno) = Vector3(x, y, z);
	//	(*(this->m_pQuaternionData)).at(frameno) = q.FromEulerAngleZXY(z, x, y);
	//q.fromEuler(x, y, z, Euler::ZXY);
	q.fromEuler(x, y, z, m_RotationOrder);
	(*(this->m_pQuaternionData)).at(frameno) = q;
}


void MotionJoint::setRotation(Vector3 rot, int frameno)
{
	if(m_JointType == END_SITE)
		return ;

	if (frameno == m_currentFrame)
		this->m_bNeedUpdate = true;

	Quaternion q;
	(*(this->m_pRotationData)).at(frameno) = rot;
	//q.fromEuler(rot.x(), rot.y(), rot.z(), Euler::ZXY);
	q.fromEuler(rot.x(), rot.y(), rot.z(), m_RotationOrder);
	(*(this->m_pQuaternionData)).at(frameno) = q;
}

void MotionJoint::setRotationX(double x, int frameno)
{
	if (frameno == m_currentFrame)
		this->m_bNeedUpdate = true;
	Vector3 temp;
	temp = (*(this->m_pRotationData)).at(frameno);  
	temp.set_x (x);

	Quaternion q;
	(*(this->m_pRotationData)).at(frameno) = temp;
	//q.fromEuler(temp.z(), temp.x(), temp.y(), Euler::ZXY);
	q.fromEuler(temp.x(), temp.y(), temp.z(), m_RotationOrder);
	(*(this->m_pQuaternionData)).at(frameno) = q;
}

void MotionJoint::setRotationY(double y, int frameno)
{
	if (frameno == m_currentFrame)
		this->m_bNeedUpdate = true;
	Vector3 temp;
	temp = (*(this->m_pRotationData)).at(frameno);  
	temp.set_y (y);

	Quaternion q;
	(*(this->m_pRotationData)).at(frameno) = temp;
	//q.fromEuler(temp.z(), temp.x(), temp.y(), Euler::ZXY);
	q.fromEuler(temp.x(), temp.y(), temp.z(), m_RotationOrder);
	(*(this->m_pQuaternionData)).at(frameno) = q;
}

void MotionJoint::setRotationZ(double z, int frameno)
{
	if (frameno == m_currentFrame)
		this->m_bNeedUpdate = true;
	Vector3 temp;
	temp = (*(this->m_pRotationData)).at(frameno);  
	temp.set_z (z);

	Quaternion q;
	(*(this->m_pRotationData)).at(frameno) = temp;
	//q.fromEuler(temp.z(), temp.x(), temp.y(), Euler::ZXY);
	q.fromEuler(temp.x(), temp.y(), temp.z(), m_RotationOrder);
	(*(this->m_pQuaternionData)).at(frameno) = q;
}

void MotionJoint::setCurrentFrame(int frameno)
{
	if (frameno != this->m_currentFrame )
		this->m_bNeedUpdate = true;
	this->m_currentFrame = frameno;
}

std::vector<Vector3>* MotionJoint::getRotationDataPointer()
{
	return m_pRotationData;
}

void MotionJoint::setRotationDataPointer(std::vector<Vector3>* pvec)
{
	m_pRotationData = pvec;
}

std::vector<Vector3>* MotionJoint::getPositionDataPointer()
{
	return m_pPositionData;
}

void MotionJoint::setPositionDataPointer(std::vector<Vector3>* pvec)
{
	m_pPositionData = pvec;
}

std::vector<Quaternion>* MotionJoint::getQuaternionDataPointer()
{
	return m_pQuaternionData;
}

void MotionJoint::setQuaternionDataPointer(std::vector<Quaternion>* pvec)
{
	m_pQuaternionData = pvec;
}

std::vector<Vector3>* MotionJoint::getScaleDataPointer()
{
	return m_pScaleData;
}


void MotionJoint::setScaleDataPointer(std::vector<Vector3>* pvec)
{
	m_pScaleData = pvec;
}

void MotionJoint::updateEulerAngleToQuaternion()
{
	(*(this->m_pQuaternionData)).clear();
	(*(this->m_pQuaternionData)).reserve( (*(this->m_pRotationData)).size() );

	Vector3 vec;
	Quaternion quat;

	for ( std::vector<Vector3>::iterator it = (*(this->m_pRotationData)).begin();
		it != (*(this->m_pRotationData)).end(); ++it)
	{
		vec = *it;
//		quat.FromEulerAngle(vec.x, vec.y, vec.z, this->getEulerAnglesOrder());
		quat.fromEuler(vec.x(), vec.y(), vec.z(), this->getEulerAnglesOrder());
		(*(this->m_pQuaternionData)).push_back(quat);
	}
}

void MotionJoint::updateQuaternionToEulerAngle()
{
	(*(this->m_pRotationData)).clear();
	(*(this->m_pRotationData)).reserve( (*(this->m_pQuaternionData)).size() );

	Vector3 vec;
	Quaternion quat;

	for (std::vector<Quaternion>::iterator it = (*(this->m_pQuaternionData)).begin();
		it != (*(this->m_pQuaternionData)).end(); ++it)
	{
		quat = *it;
		(*(this->m_pRotationData)).push_back(Vector3(quat.toEuler(this->getEulerAnglesOrder())));
	}
}

//void MotionJoint::checkFrameNumberRange(int frameno)
//{
//	if (this->m_JointType != END_SITE )
//	{
//		if (frameno<0 || frameno > static_cast<int>((*(this->m_pRotationData)).size() + 1 ))
//		{
//			throw std::out_of_range("Frame number out of range in MotionJoint::setCurrentFrame() .");
//		}
//		else if (frameno == (*(this->m_pRotationData)).size() + 1)
//		{
//			//expand data vector
//			if (this->m_JointType == ROOT_JOINT)
//			{	
//				(*(this->m_pPositionData)).push_back((*(this->m_pPositionData)).at((*(this->m_pPositionData)).size()));
//			}
//			(*(this->m_pRotationData)).push_back(Vector3::ZERO);
//			(*(this->m_pQuaternionData)).push_back(Quaternion::ZERO);
//
//		}
//	}
//}

void MotionJoint::setInheritOrientation(bool inherit)
{
	this->m_bInheritOrientation = inherit;
}

bool MotionJoint::getInheritOrientation() 
{
	return this->m_bInheritOrientation;
}

void MotionJoint::setInheritScale(bool inherit)
{
	this->m_bInheritScale = inherit;
}

bool MotionJoint::getInheritScale() 
{
	return this->m_bInheritScale;
}

const Vector3& MotionJoint::_getInheritScale(int frameno)
{
	_updateDataFromParent(frameno);
	return m_DerivedScale;
}

const Vector3& MotionJoint::_getInheritPosition(int frameno) 
{
	_updateDataFromParent(frameno);
	return m_DerivedPosition;
}

const Quaternion& MotionJoint::_getInheritOrientation(int frameno) 
{
	_updateDataFromParent(frameno);
	return m_DerivedOrientation;
}

void MotionJoint::_updateDataFromParent(int frameno) 
{
	setCurrentFrame(frameno);
	if (this->m_JointType == ROOT_JOINT)
	{
		//if (!this->m_bNeedUpdate && this->m_currentFrame == frameno )
		if (!this->m_bNeedUpdate  )
			return; // nothing to update

		m_DerivedOrientation = m_Orientation = (*(this->m_pQuaternionData)).at(frameno);
		m_RelativePosition = m_DerivedPosition = m_Position = (*(this->m_pPositionData)).at(frameno);

		if (m_pScaleData)
		{
			m_DerivedScale = m_Scale = (*(this->m_pScaleData)).at(frameno);
		}
		else
		{
			m_DerivedScale = m_Scale = Vector3::UNIT_SCALE;
		}
	}
	else if (this->m_JointType == NORMAL_JOINT)
	{
		if (!this->m_bNeedUpdate && !this->m_pParent->m_bNeedUpdate 
			&& this->m_currentFrame == frameno )
			return; // nothing to update

		// Orientation
		m_Orientation = (*(this->m_pQuaternionData)).at(frameno);
		const Quaternion& parentOrientation = m_pParent->_getInheritOrientation(frameno);
		if (m_bInheritOrientation)
		{
			m_DerivedOrientation = parentOrientation * m_Orientation;
		}
		else
		{
			m_DerivedOrientation = m_Orientation;
		}

		// Scale
		if (this->m_pScaleData)
		{
			m_Scale = (*(this->m_pScaleData)).at(frameno);
		}
		else
		{
			m_Scale = Vector3::UNIT_SCALE;
		}

		const Vector3& parentScale = m_pParent -> _getInheritScale(frameno);
		if (m_bInheritScale)
		{
			m_DerivedScale = m_Scale.scaling(parentScale);
		}
		else 
		{
			m_DerivedScale = m_Scale;
		}

		// Position
		m_Position = Vector3(m_fXOffset, m_fYOffset, m_fZOffset);
	//	m_RelativePosition = m_DerivedPosition = parentOrientation * (m_Position.scaling(m_DerivedScale));
		m_RelativePosition = m_DerivedPosition = parentOrientation * m_Position;
		m_DerivedPosition = m_RelativePosition.scaling(m_DerivedScale);
		//m_DerivedPosition = parentOrientation.QVRotation(parentOrientation, m_Position);
		m_DerivedPosition += m_pParent -> _getInheritPosition(frameno);
	}
	else // END_SITE
	{
		if (!this->m_bNeedUpdate && !this->m_pParent->m_bNeedUpdate 
			&& this->m_currentFrame == frameno )
			return; // nothing to update

		const Quaternion& parentOrientation = m_pParent->_getInheritOrientation(frameno);
		m_DerivedOrientation = parentOrientation;
		/*const Vector3& parentScale = m_pParent -> _getInheritScale(frameno);
		m_DerivedScale = parentScale;*/
		// Scale
		if (this->m_pScaleData)
		{
			m_Scale = (*(this->m_pScaleData)).at(frameno);
		}
		else
		{
			m_Scale = Vector3::UNIT_SCALE;
		}

		const Vector3& parentScale = m_pParent -> _getInheritScale(frameno);
		if (m_bInheritScale)
		{
			m_DerivedScale = m_Scale.scaling(parentScale);
		}
		else 
		{
			m_DerivedScale = m_Scale;
		}

		m_Position = Vector3(m_fXOffset, m_fYOffset, m_fZOffset);
	//	m_RelativePosition = m_DerivedPosition = parentOrientation * (m_Position.scaling(m_DerivedScale));
		m_RelativePosition = m_DerivedPosition = parentOrientation * m_Position;
		m_DerivedPosition = m_RelativePosition.scaling(m_DerivedScale);
		m_DerivedPosition += m_pParent -> _getInheritPosition(frameno);

	}

	this->m_bNeedUpdate = false;
	this->m_currentFrame = frameno;

	for (std::vector<MotionJoint*>::iterator it = this->m_pChildren.begin();
		it != this->m_pChildren.end(); it++ )
	{
		(*it)->m_bNeedUpdate = true;
	}
}

void MotionJoint::_setUpdateFlagRecursively(bool bNeedUpdate)
{
	this->m_bNeedUpdate = bNeedUpdate;
	for (int i=0; i<m_pChildren.size(); i++)
	{
		m_pChildren.at(i)->_setUpdateFlagRecursively(bNeedUpdate);
	}
}

void MotionJoint::update(int frameno)
{
	_updateDataFromParent(frameno);
}

Quaternion MotionJoint::OrientationTillParent(int frameno)
{
	if (this->m_JointType != ROOT_JOINT)
	{
		return m_pParent -> _getInheritOrientation(frameno);
	}
	else
	{
		return Quaternion::IDENTITY;
	}
}

void MotionJoint::NeedUpdate(void)
{
	m_bNeedUpdate = true;
}

//void MotionJoint::setRotationAndQuaternion(Vector3 v,int frameno)
//{
//	setRotation(v,frameno);
//	Quaternion resultQuat;
//	resultQuat.FromEulerAngleZXY(v.z,v.x,v.y);
//	setOrientation(resultQuat, frameno);
//}

Vector3 MotionJoint::getUpdatedAbsolutePosition(int frameno)
{
	this->NeedUpdate();
	return this->getAbsolutePosition(frameno);
}

void MotionJoint::setEulerAnglesOrder(Euler::RotateOrder order)
{
	m_RotationOrder = order;
	return;
}

Euler::RotateOrder MotionJoint::getEulerAnglesOrder(void) const
{
	return m_RotationOrder;
}

