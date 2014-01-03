#include "Ellipse.h"
#include "SimpleObject.h"
#include "Node.h"
#include "SceneManager.h"

namespace Swift{

	Ellipse::Ellipse(AbstractSceneManager * p_mgr)
		: SceneObject(p_mgr, ID_Entity)
	{
		m_dRadius=3.0;
		Color c = {0.988, 0.722, 0.318, 1.0};
		setColor(c);
	}

	Ellipse::Ellipse(double dRadius, AbstractSceneManager * p_mgr)
		: SceneObject(p_mgr, ID_Entity)
	{
		m_dRadius = dRadius;
		Color c = {0.988, 0.722, 0.318, 1.0};
		setColor(c);
	}

	Ellipse::~Ellipse()
	{
	}

	void Ellipse::render()
	{
		if (!m_pNode || !(m_pNode->getParent()) || !(m_pNode->getParent()->isBoneNode()))
		{
			return;
		}
		if (!m_bVisible)
		{
			return;
		}

		double dAngle;
		Vector3 vAxis;
		Vector3 vPosition;
		Vector3 parPosition;
		//m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
	//	Quaternion q = m_pNode->getParent()->getAbsoluteOrientation();
		//Quaternion q = Quaternion::IDENTITY;
	//	q.toAngleAxis(dAngle,vAxis);
		vPosition = m_pNode->getAbsolutePosition();
		parPosition = m_pNode->getParent()->getAbsolutePosition();
		Vector3 dir = parPosition - vPosition;

		double b = dir.getNorm() / 2;
		if(Math::IsZero(b))
			return ;

		double a = 2.0;
		QString name = m_pNode->getName();
		QString parName = m_pNode->getParent()->getName();
		if (name.contains("pinky") || name.contains("ring") || name.contains("middle") || name.contains("index") || name.contains("thumb")
			|| parName.contains("pinky") || parName.contains("ring") || parName.contains("middle") || parName.contains("index") || parName.contains("thumb"))
			a = 1.0;
		//if(name.contains("Knee") || name.contains("Ankle")){
		//	double scale = m_pNode->getScale().x();
		//	if(Math::IsZero(scale))
		//		return ;
		//	a = 96.0 / (b * scale);
		//}else if(name.contains("Elbow") || name.contains("Wrist")){
		//	a = 48.0 / b;
		//}else{
		//	a = 32.0 / b;
		//}
	//	double a = 32.0 / b; //m_dRadius;
		/*double a = m_dRadius;
		if(scale > 1)
			a = m_dRadius / (10 * scale);
		else if(scale < 1)
			a = m_dRadius * 10 / scale;*/

		Quaternion q = getRotationTo(Vector3(0, 1, 0), dir);
		q.toAngleAxis(dAngle,vAxis);

		Color color = getColor();
		glColor4f(color.r,color.g,color.b,color.a);
		/*static Skeleton *sk = 0;
		static Vector3 color1(0.988, 0.722, 0.318);
		static Vector3 color2(0.604, 0.984, 0.510);
		static Vector3 color = Vector3::ZERO;
		if(((BoneNode*)m_pNode)->getSkeleton() != sk){
			sk = ((BoneNode*)m_pNode)->getSkeleton();
			color = color1;
			color1 = color2;
			color2 = color;
		}
		glColor3dv(color.getDataPointer());*/
	//	glColor3f(0.988f, 0.722f, 0.318f);
		glPushAttrib(GL_ENABLE_BIT);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_NORMALIZE);
		
		glPushMatrix();

		glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
		glRotatef(dAngle,vAxis.x(),vAxis.y(),vAxis.z());
		
	
		/*glBegin(GL_LINES);
		glVertex3f(0, -b, 0);
		glVertex3f(0, b, 0);
		glEnd();*/
		drawEllipse(a, b, 20, 20);
		
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
		glPopAttrib();
	}

	void Ellipse::drawEllipse(double a, double b, int h, int v){
		/*if(Math::IsZero(b))
			return ;*/

		double ax2 = a * a;
		double bx2 = b * b;

		double dy = 2 * b / (v + 1);
		Quaternion dq;
		dq.fromAngleAxis(360 / h, Vector3(0.0, 1.0, 0.0));

		double y = -b;
		double r = 0.0;
		std::vector<Vector3> vertices, normals;
		vertices.reserve((v + 2) * h);
		normals.reserve((v + 2) * h);
		Vector3 trough(0.0, 0.0, 0.0);
		Vector3 tn(0.0, -1.0, 0.0);
		for(int j=0; j<h; ++j){
			vertices.push_back(trough);
			normals.push_back(tn);
		}
		y += dy;

		for(int i=0; i<v/*+2*/; ++i){
			r = sqrt(ax2 * (1 - y * y / bx2));	
			Vector3 o(0.0, y + b, r);
			Vector3 n(0.0, (y/* + b*/) / bx2, r / ax2);
		//	n.normalise();
			for(int j=0; j<h; ++j){
				vertices.push_back(o);
				normals.push_back(n);
				o = dq * o;
				n.set_values(o.x() / ax2, (o.y() - b) / bx2, o.z() / ax2);
			//	n.normalise();
			}

			y += dy;
		}

		Vector3 crest(0.0, b + b, 0.0);
		Vector3 cn(0.0, 1.0, 0.0);
		for(int j=0; j<h; ++j){
			vertices.push_back(crest);
			normals.push_back(cn);
		}



		int facenum = h * (v + 1);

		
		glBegin(GL_QUADS);
		for(int i=0; i<v+1; ++i){
			for(int j=0; j<h; ++j){
				int curr = i * h + j;
				glNormal3dv(normals[curr].getDataPointer());
				glVertex3dv(vertices[curr].getDataPointer());
				if(j == h - 1)
					curr -= j;
				else
					curr += 1;
				glNormal3dv(normals[curr].getDataPointer());
				glVertex3dv(vertices[curr].getDataPointer());
				curr += h;
				glNormal3dv(normals[curr].getDataPointer());
				glVertex3dv(vertices[curr].getDataPointer());
				if(j == h - 1)
					curr += h - 1;
				else
					curr -= 1;
				glNormal3dv(normals[curr].getDataPointer());
				glVertex3dv(vertices[curr].getDataPointer());
			}
		}

		glEnd();

		
	}

}