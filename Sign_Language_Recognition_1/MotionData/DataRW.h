/* by ywpg */
/* Last Modified: 2008/04/15 21:21:03 . */

#ifndef DATA_READER_WRITER_H_INCLUDED
#define DATA_READER_WRITER_H_INCLUDED

#if _MSC_VER > 1000
// something that microsoft c compiler ignored.
// we ignore the warning, too.
#pragma warning(disable: 4290) 

#endif //_MSC_VER > 1000

#include	<exception>
#include	<fstream>
#include	<sstream>
#include 	<QtCore/QString>
//#include	<string>

#include "./MotionJoint.h"
#include "./MotionClip.h"
//#include "./MotionCharacter.h"

namespace Swift
{

	/*
	 * ===========================================================================
	 *        Class:  DataReader
	 *  Description:  
	 * ===========================================================================
	 */
	class   DataReader
	{
	public:

		DataReader ();                  /* constructor      */
		DataReader ( const DataReader &other );  /* copy constructor */
		~DataReader ();                 /* destructor       */

		const DataReader& operator = ( const DataReader &other );  /* assignment operator */
		
		virtual void read(void) = 0;
	    
	}; /* -----  end of class  DataReader  ----- */


	/*
	 * ===========================================================================
	 *        Class:  DataWriter
	 *  Description:  
	 * ===========================================================================
	 */
	class   DataWriter
	{
	public:

		DataWriter ();                  /* constructor      */
		DataWriter ( const DataWriter &other );  /* copy constructor */
		~DataWriter ();                 /* destructor       */

		const DataWriter& operator = ( const DataWriter &other );  /* assignment operator */

		virtual void write(void) = 0;

	}; /* -----  end of class  DataWriter  ----- */


	/*
	 * ===========================================================================
	 *        Class:  TextDataReader
	 *  Description:  
	 * ===========================================================================
	 */
	class   TextDataReader : public DataReader
	{
	public:

		TextDataReader ();                  /* constructor      */
		TextDataReader ( const TextDataReader &other );  /* copy constructor */
		~TextDataReader ();                 /* destructor       */

		const TextDataReader& operator = ( const TextDataReader &other );  /* assignment operator */

		virtual void read(void);

	}; /* -----  end of class  TextDataReader  ----- */


	/*
	 * ===========================================================================
	 *        Class:  TextDataWriter
	 *  Description:  
	 * ===========================================================================
	 */
	class   TextDataWriter : public DataWriter
	{

	  public:

		TextDataWriter ();                  /* constructor      */
		TextDataWriter ( std::ostream * pstream );

		TextDataWriter ( const TextDataWriter &other );  /* copy constructor */
		~TextDataWriter ();                 /* destructor       */

		const TextDataWriter& operator = ( const TextDataWriter &other );  /* assignment operator */

		virtual void write(void);

	  protected:
		std::ostream* m_pstream;

	}; /* -----  end of class  TextDataWriter  ----- */



	/* ======================================================================== */

	class   DataReaderError : public std::exception
	{
	public:
		DataReaderError(const std::string& str) : std::exception(str.c_str())
		{

		}

		DataReaderError(const char* pmsg) : std::exception(pmsg)
		{

		}

	};

	class   DataWriterError : public std::exception
	{
	public:
		DataWriterError(const std::string& str) : std::exception(str.c_str())
		{

		}

		DataWriterError(const char* pmsg) : std::exception(pmsg)
		{

		}

	};



	class   TextDataReaderError : public DataReaderError
	{
	public:
		TextDataReaderError(const std::string& str) : DataReaderError(str)
		{

		}

		TextDataReaderError(const char* pmsg) : DataReaderError(pmsg)
		{

		}

	};

	class   TextDataWriterError : public DataWriterError
	{
	public:
		TextDataWriterError(const std::string& str) : DataWriterError(str)
		{

		}

		TextDataWriterError(const char* pmsg) : DataWriterError(pmsg)
		{

		}

	};
}
#endif //DATA_READER_WRITER_H_INCLUDED

