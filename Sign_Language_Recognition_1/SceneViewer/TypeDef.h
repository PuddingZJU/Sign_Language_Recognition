/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/22                                                  */
/* Last Modified: 2007/11/25                                            */
/************************************************************************/

#ifndef _TypeDef_H_
#define _TypeDef_H_

namespace Swift
{
	/// This Enum the type of IDs.
	/** 
	 * @remarks
	 * ID is of a unsigned long type which usually has 4 bytes.
	 * The highest 8 bits of an ID stand the type of the ID.
	 * @see IDManager.
	*/
	typedef enum 
	{
		ID_Unknown = 0,
		ID_Node = 1,
		ID_Ground = 2,
		ID_Entity = 3,
		ID_Camera = 4,
		ID_Light = 5,
		ID_3DText = 6,
		ID_Texture = 7,
		ID_Mesh = 8,
		ID_BoundingBox = 9,
		ID_Music = 10,
		ID_Axes = 11,
	}IDType;

	/// Enumeration of queue groups.
	/** 
	* By this enum the application may group queued renderable objects,
	* so that they are rendered together with events in between.
	*/
	typedef enum
	{
		/// Unspecified render group.
		RENDER_QUEUE_UNKNOWN = -1,
		/// Use this queue for objects which must be rendered first e.g. backgrounds
		RENDER_QUEUE_BACKGROUND = 0,
		/// The default render queue 
		RENDER_QUEUE_MAIN = 50,
		/// Use this queue for objects which must be rendered last e.g. overlays
		RENDER_QUEUE_OVERLAY = 100,
	}RenderGroupID;


	/// Enumeration of view mode.
	/** 
	* The view mode can be perspective, ortho.
	*/
	typedef enum 
	{
		PERSPECTIVE=0,
		//ORTHOGRAPHIC,
		ORTHO_FRONT,
		ORTHO_BACK,
		ORTHO_RIGHT,
		ORTHO_LEFT,
		ORTHO_TOP,
		ORTHO_BOTTOM,
	}GLViewMode; 

	/// Enumeration of motion state.
	/** 
	* The state can be Inactive(including Visible and Invisible) and Active.
	*/
	typedef enum 
	{
		Invisible,
		Visible,
		Active,
	}MFileState;

	typedef struct  
	{
		float r;
		float g;
		float b;
		float a;
	}Color;

}	//Swift

#endif	//_TypeDef_H_