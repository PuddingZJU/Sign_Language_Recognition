#ifndef _BVH_WRITER_H_INCLUDED_
#define _BVH_WRITER_H_INCLUDED_

#include "DataRW.h"

#include <map>
#include <exception>

namespace Swift
{
	class   BVHWriter : public TextDataWriter
	{
	protected:
		/**
		 * Copy constructor, forbidden.
		 */
		BVHWriter( const BVHWriter& )
		{
		}

		/**
		 * Copy constructor, forbidden.
		 */
		BVHWriter operator= ( const BVHWriter& );
	
		
		void open() throw (std::exception);

		void close() throw (std::exception);

		void setStream(std::ostream& os);

		void writeSkeleton();

		void writeSkeletonRecursively(MotionJoint* pmj, int tabs);

		void writeData();

		void writeDataReverse();

	public:
		/**
		 * Constructor.
		 * @param filepathname The full path of the file we want to deal with.
		 */
		BVHWriter( std::string filepathname );

		BVHWriter();

		/**
		 * Cleans up.
		 */
		virtual ~BVHWriter();

		virtual void write(void);

		void writeToStream (std::ostream* pos);

		void setFilePathName(std::string filepathname);

		void setSource (MotionClip* pmc);

		void setSpacing(std::string strSpace);

		void setSpaceBeforeEndline(bool b);

		void setChannelsRecursively( MotionJoint* pmj);

		void setEulerAnglesOrder(Euler::RotateOrder order);

		void updateData(void);

	protected:
		void updateDataRecursively(MotionJoint * pmj);

	protected:

		std::string m_filepathname;

		MotionClip* m_pmc;

		std::ofstream m_ofstream;

		std::vector<std::vector<Vector3>* > m_channels;

		std::string m_space;
		
		bool m_bSpaceBeforeEndline;

		bool m_bChangeEulerAnglesOrder;
		Euler::RotateOrder m_EulerAnglesOrder;

		Euler::RotateOrder ordernow; //temp
	};
}
#endif //_BVH_WRITER_H_INCLUDED_

