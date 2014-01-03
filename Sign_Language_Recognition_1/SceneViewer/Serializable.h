/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/08/20                                            */
/************************************************************************/

#ifndef _Serializable_H_
#define _Serializable_H_


namespace Swift
{
	/// Abstract base class of every serializable thing in the scene.
	/**
	 * Using the interfaces of this class, the client programmers can serialize class objects, 
	 * so that the data and state of the scene can be saved and reload as the same later.
	*/
	class Serializable
	{
	public:
		/// Constructor of the class.
		Serializable(){};
		///Destructor, which is pure virtual, make this class not have an instance.
		virtual ~Serializable() = 0{};
	
		/// Serialize the object to a stream.
		/**
		 * @remarks
		 * Any inherited class should implement this interface.
		*/
		virtual void serializeToStream(){};
		/// Deserialize the object from a stream.
		/**
		* @remarks
		* Any inherited class should implement this interface.
		*/
		virtual void deserializeFromStream(){};
		/// Serialize the object to XML.
		/**
		* @remarks
		* Any inherited class should implement this interface.
		*/
		virtual void serializeToXML(){};
		/// Deserialize the object from XML.
		/**
		* @remarks
		* Any inherited class should implement this interface.
		*/
		virtual void deserializeFromXML(){};
	};	//Serializable
}

#endif	//_Serializable_H_