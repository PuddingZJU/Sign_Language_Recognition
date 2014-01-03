/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/08/20                                            */
/************************************************************************/


#ifndef	_Traceable_H_
#define _Traceable_H_

#include "../../CommonHeader.h"
#include <QtCore/QString>


namespace Swift
{
	/// Abstract base class of every traceable things in the scene.
	/**
	 *  Every class that need to be traceable should inherit from this abstract class.
	 *	@remarks
	 *	Traceable objects must have a global unique id and a specified or auto-generated name.
	 *	Objects of this kind can be traced by their ids.
	*/
	class Traceable
	{
	public:
		///Constructor of the class.
		Traceable(){};
		///Destructor, which is pure virtual, make this class not have an instance.
		virtual ~Traceable() = 0{};

	protected:
		/// Unique ID of the traceable, 
		unsigned long m_nID;
		/// Friendly name of the traceable, can be automatically generated if you don't care
		QString m_strName;

	public:
		/// Return the ID of the traceable thing.
		inline const unsigned long& getID() const
		{
			return m_nID;
		};
		/// Change the ID of the traceable thing.
		inline void setID(unsigned long nID)
		{
			m_nID = nID;
		};
		/// Return the name of the traceable thing.
		inline const QString& getName() const
		{
			return m_strName;
		};
		/// Change the name of the traceable thing.
		inline void setName(QString strName)
		{
			m_strName = strName;
		};	

	};	//class Traceable

}	//Swift

#endif	//_Traceable_H_
