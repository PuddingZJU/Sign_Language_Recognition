/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/08/22                                            */
/************************************************************************/

#ifndef _TraceTable_H_
#define _TraceTable_H_

#include <map>
#include "Traceable.h"
#include "../../Util/Singleton.h"


namespace Swift
{
	/// This class holds records of traceable objects.
	/**
	* Using the interfaces of this class, the client programmers can get pointer to traceable object by its id or name.
	* @remarks
	* This class provides two interfaces to get the pointer,getPointerByID and getPointerByName,
	* If you want to get name by ID, you should first get pointer, then call pointer->getName() function.
	* It's the same case if you want to get ID by name.
	* Since you can get pointer by name, the name of a traceable object should be unique.
	* If you create an object which name is already exists, some index will be added to the end of the name to make it unique.
	* @see Traceable.
	*/
	class TraceTable : public Singleton<TraceTable>
	{
	public:
		/// Constructor.
		TraceTable();
		/// Destructor, if the maps is not empty, this will clear them.
		~TraceTable();

	private:
		/// Store the ID and pointer of objects.
		std::map<unsigned long, Traceable*> m_mIDPointer;
		/// Store the name and pointer of objects.
		std::map<QString, Traceable*> m_mNamePointer;

	public:
		/// Get the pointer to Traceable object by its ID.
		/**
		 * @return The pointer to a traceable object.
		*/
		Traceable* getPointerByID(const unsigned long nID);

		/// Get the pointer to Traceable object by its name.
		/**
		 * @param strName It is a const reference to an object's name.
		 * @return The pointer to a traceable object.
		*/
		Traceable* getPointerByName(const QString& strName);

		/// Add a record to this table.
		void addTraceRecord(Traceable* pTraceable);

		/// Delete a record by ID.
		bool deleteTraceRecord(const unsigned long nID);

		/// Delete a record by name.
		/**
		* @param strName It is a const reference to an object's name.
		*/
		bool deleteTraceRecord(const QString& strName);

		/// Delete all records in the table.
		void deleteAllTraceRecords();
	};	//TraceTable
}	//Swift


#endif	// _TraceTable_H_