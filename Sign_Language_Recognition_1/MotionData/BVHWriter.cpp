/* by ywpg */
/* Last Modified: 2007/08/31 06:02:24 . */

#include "BVHWriter.h"

#include <fstream>
#include <sstream>
#include <exception>
#include <QtCore/QString>

using namespace Swift;

BVHWriter::BVHWriter(std::string filepathname)
	: m_filepathname(filepathname)
{
	m_pmc = NULL;
	m_space = "\t";
	m_bSpaceBeforeEndline = true;
	m_bChangeEulerAnglesOrder = false;
}

BVHWriter::BVHWriter()
{
	m_pmc = NULL;
	m_space = "\t";
	m_bSpaceBeforeEndline = true;
	m_bChangeEulerAnglesOrder = false;
}

BVHWriter::~BVHWriter()
{
	this->m_pstream = NULL;
}

void BVHWriter::setSource(MotionClip* pmc)
{
	this->m_pmc = pmc;
}

void BVHWriter::setFilePathName(std::string filepathname)
{
	this->m_filepathname = filepathname;
}

void BVHWriter::setSpacing(std::string strSpace)
{
	this->m_space = strSpace;
}

void BVHWriter::setEulerAnglesOrder(Euler::RotateOrder order)
{
	this->m_bChangeEulerAnglesOrder = true;
	this->m_EulerAnglesOrder = order;
}

void BVHWriter::open() throw (std::exception)
{
	this->m_ofstream.open(this->m_filepathname.c_str() , std::ios::out);

	if (m_ofstream.fail())
	{
		throw std::runtime_error("CANNOT open file \"" + this->m_filepathname + "\" for writing.");
	}
	else
	{
		this->m_pstream = &m_ofstream;
	}
}

void BVHWriter::setStream(std::ostream& os)
{
	this->m_pstream = &os;
}

void BVHWriter::close() throw (std::exception)
{
	this->m_ofstream.close();
}

void BVHWriter::write()
{
	if (! this->m_pmc)
	{
		throw TextDataWriterError("No data source given.");
	}

	if (0 == this->m_filepathname.size())
	{
		if ( 0 == this->m_pmc->getFileName().size() )
		{
			throw TextDataWriterError("No filename given.");
		}
		else
		{
			// use the name given in MotionClip object.
			this->m_filepathname = this->m_pmc->getFileName();
		}		
	}

	this->open();

	try
	{
		this->writeSkeleton();

		this->updateData();

		this->writeData();
	}
	catch (...)
	{
		throw;
	}

	this->close();

	return;

}

void BVHWriter::writeToStream(std::ostream* pos)
{
	try
	{
		this->setStream(*pos);

		this->writeSkeleton();
		this->writeData();
	}
	catch (...)
	{
		throw;
	}
}

void BVHWriter::writeSkeleton()
{
	(*(this->m_pstream)) << "HIERARCHY" << std::endl;
	
	if (m_bChangeEulerAnglesOrder)
	{
		ordernow = m_EulerAnglesOrder;
	}
	else
	{
		ordernow = this->m_pmc->getRoot(0)->getEulerAnglesOrder();
	}

	writeSkeletonRecursively(this->m_pmc->getRoot(0), 0);
}

void BVHWriter::writeSkeletonRecursively(MotionJoint* pmj, int tabs)
{
	std::ostream& out ( (*(this->m_pstream)) );

	int c;
	std::string str;
	str.reserve(10);
	for (int k = 0; k < tabs; k++)
	{
		str += this->m_space;
	}

	switch(pmj->m_JointType)
	{
	case MotionJoint::ROOT_JOINT:
		
		out << str << "ROOT " << pmj->getName() << std::endl
			<< str << "{" << std::endl
			<< str << this->m_space << "OFFSET " << pmj->getXOffset() << " " << pmj->getYOffset() << " " << pmj->getZOffset() << std::endl;

		if (ordernow == Euler::ZXY)
		{
			out << str << this->m_space << "CHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation" << std::endl;
		}
		else if (ordernow == Euler::XYZ)
		{
			out << str << this->m_space << "CHANNELS 6 Xposition Yposition Zposition Xrotation Yrotation Zrotation" << std::endl;
		}

		c = this->m_pmc->getChildrenCount(pmj);
		for (int i = 0; i < c ; i++ )
		{
			writeSkeletonRecursively(this->m_pmc->getChild(pmj, i), tabs+1);
		}

		out << str << "}" << std::endl;		

		break;
	case MotionJoint::NORMAL_JOINT:

		out << str << "JOINT " << pmj->getName() << std::endl
			<< str << "{" << std::endl
			<< str << this->m_space << "OFFSET " << pmj->getXOffset() << " " << pmj->getYOffset() << " " << pmj->getZOffset() << std::endl;
			
		if (ordernow == Euler::ZXY)
		{
			out << str << this->m_space << "CHANNELS 3 Zrotation Xrotation Yrotation" << std::endl;
		}
		else if (ordernow == Euler::XYZ)
		{
			out << str << this->m_space << "CHANNELS 3 Xrotation Yrotation Zrotation" << std::endl;
		}

		c = this->m_pmc->getChildrenCount(pmj);
		for (int i = 0; i < c ; i++ )
		{
			writeSkeletonRecursively(this->m_pmc->getChild(pmj, i), tabs+1);
		}

		out << str << "}" << std::endl;

		break;
	case MotionJoint::END_SITE:

		out << str << "End Site" << std::endl
		//out  << str << pmj->getName() << std::endl
			<< str << "{" << std::endl
			<< str << this->m_space << "OFFSET " << pmj->getXOffset() << " " << pmj->getYOffset() << " " << pmj->getZOffset() << std::endl
			<< str << "}" << std::endl;

		break;
	default:
		throw TextDataWriterError("Unknown joint type encountered.");
	}
}

void BVHWriter::setChannelsRecursively(MotionJoint* pmj)
{
	if (pmj->m_JointType == MotionJoint::ROOT_JOINT)
	{
		this->m_channels.push_back(pmj->getPositionDataPointer());
		this->m_channels.push_back(pmj->getRotationDataPointer());
	}
	else if (pmj->m_JointType == MotionJoint::NORMAL_JOINT)
	{
		this->m_channels.push_back(pmj->getRotationDataPointer());
	}

	int c = this->m_pmc->getChildrenCount(pmj);
	for (int i = 0; i < c; i++)
	{
		setChannelsRecursively(this->m_pmc->getChild(pmj, i));
	}
}

void BVHWriter::writeData()
{
	this->m_channels.clear();
	setChannelsRecursively(this->m_pmc->getRoot(0));

	std::ostream& out ( (*(this->m_pstream)) );

	out << "MOTION" << std::endl
		<< "Frames:	" << this->m_pmc->getFrameCount() << std::endl
		<< "Frame Time:	" << this->m_pmc->getFrameTime() << std::endl;

	int c = static_cast<int>(this->m_channels.size());
	int cc = this->m_pmc->getFrameCount();
	for (int i = 0; i < cc; i++)
	{
		Vector3 vec;
		// position of root joint
		vec = (*(this->m_channels[0])).at(i);
		out << vec.x() << " " << vec.y() << " " << vec.z() << " ";

		//out << ( (*(this->m_channels[0]))[i] ).x << " " 
		//	<< ( (*(this->m_channels[0]))[i] ).y << " "
		//	<< ( (*(this->m_channels[0]))[i] ).z << " ";
		if (ordernow == Euler::ZXY)
		{
			for (int j = 1; j < c - 1; j++)
			{
				out << ( (*(this->m_channels[j]))[i] ).z() << " "
					<< ( (*(this->m_channels[j]))[i] ).x() << " "
					<< ( (*(this->m_channels[j]))[i] ).y() << " ";
			}

			// last one
			out << ( (*(this->m_channels[c-1]))[i] ).z() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).x() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).y() << (m_bSpaceBeforeEndline?" ":"") 
				<< std::endl;
		}
		else if (ordernow == Euler::XYZ)
		{
			for (int j = 1; j < c - 1; j++)
			{
				out << ( (*(this->m_channels[j]))[i] ).x() << " "
					<< ( (*(this->m_channels[j]))[i] ).y() << " "
					<< ( (*(this->m_channels[j]))[i] ).z() << " ";
			}

			// last one
			out << ( (*(this->m_channels[c-1]))[i] ).x() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).y() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).z() << (m_bSpaceBeforeEndline?" ":"") 
				<< std::endl;
		}
	}
}


void BVHWriter::writeDataReverse()
{
	this->m_channels.clear();
	setChannelsRecursively(this->m_pmc->getRoot(0));

	std::ostream& out ( (*(this->m_pstream)) );

	out << "MOTION" << std::endl
		<< "Frames:	" << this->m_pmc->getFrameCount() << std::endl
		<< "Frame Time:	" << this->m_pmc->getFrameTime() << std::endl;

	int c = static_cast<int>(this->m_channels.size());
	int cc = this->m_pmc->getFrameCount();
	for (int i = cc - 1; i >= cc; i--)
	{
		Vector3 vec;
		// position of root joint
		vec = (*(this->m_channels[0])).at(i);
		out << vec.x() << " " << vec.y() << " " << vec.z() << " ";

		//out << ( (*(this->m_channels[0]))[i] ).x << " " 
		//	<< ( (*(this->m_channels[0]))[i] ).y << " "
		//	<< ( (*(this->m_channels[0]))[i] ).z << " ";
		if (ordernow == Euler::ZXY)
		{
			for (int j = 1; j < c - 1; j++)
			{
				out << ( (*(this->m_channels[j]))[i] ).z() << " "
					<< ( (*(this->m_channels[j]))[i] ).x() << " "
					<< ( (*(this->m_channels[j]))[i] ).y() << " ";
			}

			// last one
			out << ( (*(this->m_channels[c-1]))[i] ).z() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).x() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).y() << (m_bSpaceBeforeEndline?" ":"") 
				<< std::endl;
		}
		else if (ordernow == Euler::XYZ)
		{
			for (int j = 1; j < c - 1; j++)
			{
				out << ( (*(this->m_channels[j]))[i] ).x() << " "
					<< ( (*(this->m_channels[j]))[i] ).y() << " "
					<< ( (*(this->m_channels[j]))[i] ).z() << " ";
			}

			// last one
			out << ( (*(this->m_channels[c-1]))[i] ).x() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).y() << " "
				<< ( (*(this->m_channels[c-1]))[i] ).z() << (m_bSpaceBeforeEndline?" ":"") 
				<< std::endl;
		}
	}
}
void BVHWriter::setSpaceBeforeEndline(bool b)
{
	m_bSpaceBeforeEndline = b;
}

void BVHWriter::updateData()
{
	updateDataRecursively(m_pmc->getRoot(0));
}

void BVHWriter::updateDataRecursively(MotionJoint * pmj)
{
	if (pmj->m_JointType != MotionJoint::END_SITE)
	{
		pmj->updateQuaternionToEulerAngle();

		int c = m_pmc->getChildrenCount(pmj);
		for (int i = 0; i < c; i++)
		{
			updateDataRecursively(m_pmc->getChild(pmj, i));
		}
	}
}

