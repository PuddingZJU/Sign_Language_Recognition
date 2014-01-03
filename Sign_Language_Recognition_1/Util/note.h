#ifndef ZZ_SHOW_A_NOTE_TO_THE_USER_MAYBE_A_FAILURE_2008031110__H___
#define ZZ_SHOW_A_NOTE_TO_THE_USER_MAYBE_A_FAILURE_2008031110__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */
#include <QMessageBox>
#include <cassert>

namespace Swift
{
;

inline void ZZ_NOTE(const QString & msg)
{
	QMessageBox::warning(0, "Note", msg);
}


#define ZZ_Recognition_NOT_IMPLEMENTED \
	ZZ_NOTE(QString("Sorry, This has not been implemented. o_o \n @ [%1 , %2]") \
				.arg(__FILE__).arg(__LINE__) );

#define ZZ_Recognition_CRITICAL_ERROR \
	{ \
		ZZ_NOTE( QString("A Critical Error Found. [%1: %2].") \
					.arg(__FILE__).arg(__LINE__) ); \
		assert(0); \
	}

} // namespace Recognition
#endif
