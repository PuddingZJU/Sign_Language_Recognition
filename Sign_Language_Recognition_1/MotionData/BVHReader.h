/* by ywpg */
/* Last Modified: 2007/08/31 05:56:51 . */

#ifndef _BVH_READER_H_INCLUDED_
#define _BVH_READER_H_INCLUDED_

#include "SerialTextDataReader.h"

#include "../MathLib/Math.h"

namespace Swift
{
	class   BVHReader : public SerialTextDataReader
	{
	private:
		BVHReader( const BVHReader& )
		{
		}

		BVHReader operator= ( const BVHReader& )
		{
			return NULL;
		}

	protected:
		void readSkeleton();

		MotionJoint* readSkeletonRelatively();

		void readData();

		void exceptToken(std::string strToken) throw (TextDataReaderError, std::exception);

		void exceptDataF(double f) throw (TextDataReaderError, std::exception);

		void exceptDataI(int i) throw (TextDataReaderError, std::exception);

		void reserveDataSpace(const int count);

		void reserveDataSpaceRecursively(MotionJoint* pmj, const int count);

		void open () throw (std::exception);

		void close () throw (std::exception);

		void updateDataRecursively(MotionJoint* pmj);

	public:
		BVHReader (std::string filepathname);

		BVHReader();

		virtual ~BVHReader();

		void read() throw (TextDataReaderError);

		MotionClip* result(void);

		void setFilePathName(std::string filepathname);

		void updateData();

		void setStream(std::istream& is);
	
	protected:
		std::string m_filepathname;

		std::ifstream m_ifstream;

		MotionClip * m_pmc;

		std::vector< std::vector<Vector3> * > m_channels;

		Euler::RotateOrder m_EulerAngleOrder;
//		EulerAnglesSeq m_EulerAngleSeq;

		int m_tempEndSiteIndex;
	};
}
#endif //_BVH_READER_H_INCLUDED_

