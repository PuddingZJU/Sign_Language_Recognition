/* by ywpg */
/* Last Modified: 2007/08/31 05:48:49 . */

#ifndef _SERIAL_TEXT_DATA_READER_H_INCLUDED_
#define _SERIAL_TEXT_DATA_READER_H_INCLUDED_

#include "DataRW.h"

namespace Swift
{
	class   SerialTextDataReader : public TextDataReader
	{
	protected:
		/**
		 * Copy constructor, forbidden.
		 */
		SerialTextDataReader( const SerialTextDataReader& mdr)
		{
		}

		/**
		 * Copy constructor, forbidden.
		 */
		SerialTextDataReader operator= ( const SerialTextDataReader& )
		{

			return NULL;
		}
		
		void fillBuf();


	public:
		/**
		 * Constructor.
		 */
		SerialTextDataReader( std::istream* pstream );

		SerialTextDataReader ();

		/**
		 * Cleans up.
		 */
		virtual ~SerialTextDataReader();
		

		int nextInt();
		
		double nextFloat();

		char nextChar();

		std::string nextWord();

		bool checkToken (std::string strToken);

		long getCurrentLineNo();

		void SetSkipWriteSpace(bool b);

	protected:
		std::istream* m_pstream;

		std::istringstream * m_pLine;

		std::string m_strLine; // temp. use

		long m_currentline;
		
	};
}
#endif //_SERIAL_TEXT_DATA_READER_H_INCLUDED_

