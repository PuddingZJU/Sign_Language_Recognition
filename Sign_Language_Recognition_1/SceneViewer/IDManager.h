/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/21                                                  */
/* Last Modified: 2007/08/22                                            */
/************************************************************************/

#ifndef _IDManager_H_
#define _IDManager_H_

#include "TypeDef.h"
#include "../Util/Singleton.h"

namespace Swift
{
	/// This class generate IDs of objects.
	/**
	 * Give the type of the ID you requested, you will get an unique ID by interface getID.
	 * And you can get the type of ID if given the ID by interface getIDType.
	*/
	class IDManager : public Singleton<IDManager>
	{
	public:
		/// Constructor.
		IDManager();
		/// Destructor.
		~IDManager();
	
	private:
		/// A static member which will plus 1 if you call getID function.
		static unsigned long m_nIndex;

	public:
		/// Return an global unique ID of the given type.
		/**
		 * @param type The type of the ID, which is one of enum IDType.
		 * @return A global unique ID.
		 * @see IDType.
		*/
		unsigned long getID(IDType type);
		/// Return the type of the given ID.
		/**
		 * @param nID The given ID of an object.
		 * @return The type of the ID, which is one of enum IDType.
		 * @see IDType.
		*/
		IDType getIDType(unsigned long nID);


	};	//IDManager
}	//Swift
#endif	//_IDManager_H_