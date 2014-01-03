#ifndef __ELLIPSE_H__
#define __ELLIPSE_H__

#include "SceneObject.h"
#include "../MathLib/Math.h"

namespace Swift
{

	class Ellipse :public SceneObject
	{
	public:
		explicit Ellipse(AbstractSceneManager * p_mgr);
		Ellipse(double dRadius, AbstractSceneManager * p_mgr);
		virtual ~Ellipse();

		void render();

		void drawEllipse(double a, double b, int h, int v);

	private:
		double m_dRadius;
	};	// Ellipse
}

#endif