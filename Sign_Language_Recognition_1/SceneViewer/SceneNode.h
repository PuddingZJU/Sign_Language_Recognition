/************************************************************************/
/* By xiubo.liang                                                       */
/* Created: 2007/08/15                                                  */
/* Last Modified: 2007/09/05                                            */
/************************************************************************/

#ifndef _SceneNode_H_
#define _SceneNode_H_

#include "Node.h"

namespace Swift
{
	class SceneNode : public Node
	{
	public:
		/// Constructor
		SceneNode(const QString& strName, AbstractSceneManager * p_mgr);
		/// Destructor
		~SceneNode();
		
	};	//SceneNode

}	//Swift

#endif	//_SceneNode_H_