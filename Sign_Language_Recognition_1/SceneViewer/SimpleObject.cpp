#include "SimpleObject.h"
#include "Node.h"
#include "SceneManager.h"
#include <GL/GLAux.h>
#pragma comment( lib, "glaux.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "OpenGL32.lib" )
using namespace Swift;

Cube::Cube(AbstractSceneManager * p_mgr)
: SceneObject(p_mgr, ID_Entity)
{
	m_dLength = 20.0;
	m_dWidth = 20.0;
	m_dHeight = 20.0;
}

Cube::Cube(double dLength, double dWidth, double dHeight,
		   AbstractSceneManager * p_mgr)
	: SceneObject(p_mgr, ID_Entity)
{
	m_dLength = dLength;
	m_dWidth = dWidth;
	m_dHeight = dHeight;
}

Cube::~Cube()
{
}

void Cube::render()
{
	if (!m_pNode)
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
	m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
	vPosition = m_pNode->getAbsolutePosition();

	Color color = getColor();
	glColor4f(color.r,color.g,color.b,color.a);

	glPushMatrix();

	glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
	glRotatef(dAngle,vAxis.x(),vAxis.y(),vAxis.z());
	glutSolidCube(m_dLength);

	glPopMatrix();
}


Sphere::Sphere(AbstractSceneManager * p_mgr)
: SceneObject(p_mgr, ID_Entity)
{
	m_dRadius=3.0;
}

Sphere::Sphere(double dRadius, AbstractSceneManager * p_mgr)
: SceneObject(p_mgr, ID_Entity)
{
	m_dRadius = dRadius;
}

Sphere::~Sphere()
{
}

void Sphere::render()
{
	if (!m_pNode)
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
	//m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
	Quaternion q = m_pNode->getAbsoluteOrientation();
	//Quaternion q = Quaternion::IDENTITY;
	q.toAngleAxis(dAngle,vAxis);
	vPosition = m_pNode->getAbsolutePosition();

	Color color = getColor();
	glColor4f(color.r,color.g,color.b,color.a);

	glPushMatrix();

		glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
		glRotatef(dAngle,vAxis.x(),vAxis.y(),vAxis.z());
		//glLoadName(m_nID);
		glutSolidSphere(m_dRadius,10,20);

	glPopMatrix();
}

Cylinder::Cylinder(AbstractSceneManager * p_mgr)
: SceneObject(p_mgr, ID_Entity)
{
	m_dRadiusBase = 10;
	m_dRadiusTop = 10;
	m_dHeight = 50;
}

Cylinder::Cylinder(double dRBase, double dRTop, double dHeight,
				   AbstractSceneManager * p_mgr)
: SceneObject(p_mgr, ID_Entity)
{
	m_dRadiusBase = dRBase;
	m_dRadiusTop = dRTop;
	m_dHeight = dHeight;
}

Cylinder::~Cylinder()
{
}

void Cylinder::render()
{
	if (!m_pNode)
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
	m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
	vPosition = m_pNode->getAbsolutePosition();

	Color color = getColor();
	glColor4f(color.r,color.g,color.b,color.a);

	glPushMatrix();

	glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
	glRotatef(dAngle, vAxis.x(), vAxis.y(), vAxis.z());

	GLUquadricObj *cylinder;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder,GLU_FILL);
	gluCylinder(cylinder,m_dRadiusBase, m_dRadiusTop,m_dHeight/*-2*m_dRadiusBase*/,10,20);

	glutSolidSphere(m_dRadiusBase,10,20);
	glTranslatef(0,0,m_dHeight/*-2*m_dRadiusBase*/);
	glutSolidSphere(m_dRadiusBase,10,20);

	glPopMatrix();
}

TransformTool::SingleAxis::SingleAxis(Swift::TransformTool *vpParent, 
									  const Vector3 &vDir,
									  AbstractSceneManager * p_mgr,
									  IDType type)
 : SceneObject(p_mgr, type)
{
	m_pParent = vpParent;
	direction = vDir;
}

TransformTool *TransformTool::SingleAxis::getParent() const{
	return m_pParent;
}

Vector3 TransformTool::SingleAxis::getDir() const{
	return direction;
}

TransformTool::TransformTool(AbstractSceneManager * p_mgr,
							 IDType type)
: SceneObject(p_mgr, type)
{
	m_bRender = false;
	m_dRadius = 10.0f;
	direction.set_values(1.0f, 1.0f, 1.0f);	
	m_Orientation = Quaternion::IDENTITY;
}

TransformTool::TransformTool(double vRadius,
							 AbstractSceneManager * p_mgr,
							 IDType type)
: SceneObject(p_mgr, type)
{
	m_bRender = false;
	m_dRadius = vRadius;
	direction.set_values(1.0f, 1.0f, 1.0f);
	m_Orientation = Quaternion::IDENTITY;

}

TransformTool::~TransformTool(){

}

Vector3 TransformTool::getDir() const{
	return direction;
}

void TransformTool::setDir(const Vector3 &vDir){
	direction = vDir;
}

void TransformTool::setRender(bool vbRender){
	m_bRender = vbRender;
}

unsigned long TransformTool::getXID() const{
	return m_pX->getID();
}

unsigned long TransformTool::getYID() const{
	return m_pY->getID();
}

unsigned long TransformTool::getZID() const{
	return m_pZ->getID();
}

void TransformTool::renderX(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}

	// x axis
	glColor3f(1.0f,0.0f,0.0f);
	m_pX->render();
}

void TransformTool::renderY(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}

	// y axis
	glColor3f(0.0f,1.0f,0.0f);
	m_pY->render();

}

void TransformTool::renderZ(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}


	// z axis
	glColor3f(0.0f,0.0f,1.0f);
	m_pZ->render();

}

double TransformTool::getRadius(){
	return m_dRadius;
}

void TransformTool::setRadius(double vRadius){
	m_dRadius = vRadius;
}

TransformTool::TransformToolType TransformTool::getType(){
	return type;
}

const Quaternion &TransformTool::orientation() const{
	return m_Orientation;
}

void TransformTool::setOrientation(const Quaternion &o){
	m_Orientation = o;
}

void TransformTool::resetOrientation(){
	m_Orientation = Quaternion::IDENTITY;
}

TransferAxes::TransferAxis::TransferAxis(TransformTool *vpParent, 
										 const Vector3 &vDir,
										 AbstractSceneManager * p_mgr
										 ) 
: SingleAxis(vpParent, vDir, p_mgr, ID_Axes)
{
}

TransferAxes::TransferAxis::~TransferAxis(){
}

void TransferAxes::TransferAxis::render(){
	glPushMatrix();

	glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(m_pParent->getRadius()*direction.x(),m_pParent->getRadius()*direction.y(),m_pParent->getRadius()*direction.z());
	glEnd();

	glTranslatef(m_pParent->getRadius()*direction.x(),m_pParent->getRadius()*direction.y(),m_pParent->getRadius()*direction.z());
	if(direction.x() > 0.0f)
		glRotatef(90.0f,0.0f,1.0f,0.0f);
	if(direction.y() > 0.0f)
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	
	auxSolidCone(m_pParent->getRadius()/12.0f, m_pParent->getRadius()*3/10.0f);

	glPopMatrix();
}

TransferAxes::TransferAxes(AbstractSceneManager * p_mgr) 
: TransformTool(25.0f, p_mgr, ID_Axes)
{
	type = transfer;
	m_pX = new TransferAxis(this, Vector3(1.0f, 0.0f, 0.0f), p_mgr);
	m_pY = new TransferAxis(this, Vector3(0.0f, 1.0f, 0.0f), p_mgr);
	m_pZ = new TransferAxis(this, Vector3(0.0f, 0.0f, 1.0f), p_mgr);

}

TransferAxes::TransferAxes(double vdAxisLength, AbstractSceneManager * p_mgr)
: TransformTool(vdAxisLength, p_mgr, ID_Axes)
{
	type = transfer;
	m_pX = new TransferAxis(this, Vector3(1.0f, 0.0f, 0.0f), p_mgr);
	m_pY = new TransferAxis(this, Vector3(0.0f, 1.0f, 0.0f), p_mgr);
	m_pZ = new TransferAxis(this, Vector3(0.0f, 0.0f, 1.0f), p_mgr);

}

TransferAxes::~TransferAxes(){
	delete m_pX;
	delete m_pY;
	delete m_pZ;
}

void TransferAxes::render(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}
	
	glLineWidth(1.0f);
	glColor3f(1.0f,0.0f,0.0f);
//	glPushName(m_pX->getID());
	m_pX->render();
//	glPopName();
	// y axis
	glColor3f(0.0f,1.0f,0.0f);
//	glPushName(m_pY->getID());
	m_pY->render();
//	glPopName();
	// z axis
	glColor3f(0.0f,0.0f,1.0f);
//	glPushName(m_pZ->getID());
	m_pZ->render();
//	glPopName();

}

void TransferAxes::renderX(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}

	// x axis
	glColor3f(1.0f,0.0f,0.0f);
	m_pX->render();

}

void TransferAxes::renderY(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}


	// y axis
	glColor3f(0.0f,1.0f,0.0f);
	m_pY->render();


}

void TransferAxes::renderZ(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}


	// z axis
	glColor3f(0.0f,0.0f,1.0f);
	m_pZ->render();

}

RotateSphere::RotateCircle::RotateCircle(TransformTool *vpParent,
										 const Vector3 &vDir,
										 AbstractSceneManager * p_mgr) 
: SingleAxis(vpParent, vDir, p_mgr, ID_Axes)
{
}


RotateSphere::RotateCircle::~RotateCircle(){
}

void RotateSphere::RotateCircle::render(){
	// draw circle
	glPushMatrix();

	glLineWidth(2.0f);
	glRotatef(90.0f, direction.x(), direction.y(), direction.z());
	double radius = m_pParent->getRadius();
	for(int i=0; i<8; i++){
		glRotatef(45.0f, 0.0f ,0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
			
			/*double x = 0.0f;
			double y = m_pParent->getRadius();
			double p = 3 - 2 * m_pParent->getRadius();
			glVertex3f(x, y, 0.0f);
			while(x <= y){
				if(p < 0){
					x += 1;
					p = p + 4 * x + 6;
				}else{
					x += 1;
					y -= 1;
					p = p + 4 * (x - y) + 10;
				}
				glVertex3f(x, y, 0.0f);
			}*/
		for(int angle=0; angle <= 45; angle++){
			glVertex3f(radius * Math::CosD(angle), radius * Math::SinD(angle), 0.0f);
		}
			
		glEnd();
	}

	glLineWidth(1.0f);

	glPopMatrix();
}

RotateSphere::RotateSphere(AbstractSceneManager * p_mgr) 
: TransformTool(30.0, p_mgr, ID_Axes)
{
	type = rotate;
	m_pX = new RotateCircle(this, Vector3(1.0f, 0.0f, 0.0f), p_mgr);
	m_pY = new RotateCircle(this, Vector3(0.0f, 1.0f, 0.0f), p_mgr);
	m_pZ = new RotateCircle(this, Vector3(0.0f, 0.0f, 1.0f), p_mgr);
}

RotateSphere::RotateSphere(double vdRadius, AbstractSceneManager * p_mgr)
: TransformTool(vdRadius, p_mgr, ID_Axes)
{
	type = rotate;
	m_pX = new RotateCircle(this, Vector3(1.0f, 0.0f, 0.0f), p_mgr);
	m_pY = new RotateCircle(this, Vector3(0.0f, 1.0f, 0.0f), p_mgr);
	m_pZ = new RotateCircle(this, Vector3(0.0f, 0.0f, 1.0f), p_mgr);
}

RotateSphere::~RotateSphere()
{
	delete m_pX;
	delete m_pY;
	delete m_pZ;
}

void RotateSphere::render(){
	if (!m_pNode)
	{
		return;
	}

	if(!m_bRender){
		return;
	}
	
//	double dAngle;
//	Vector3 vAxis;
//	Vector3 vPosition;
//	m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
//	vPosition = m_pNode->getAbsolutePosition();
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
////	glLoadIdentity();
//
//	glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
//	glRotatef(dAngle,vAxis.x(),vAxis.y(),vAxis.z());
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
//	glDisable(GL_CULL_FACE);

	glLineWidth(1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
	glutSolidSphere(9 * m_dRadius / 10, 30.0f, 30.0f);
	glDisable(GL_BLEND);
	// x axis
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	m_pX->render();
	// y axis
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
//	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	m_pY->render();
	// z axis
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
//	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	m_pZ->render();

	
	
	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	glPopMatrix();*/
}


Arrow::Arrow(AbstractSceneManager * p_mgr)
	: SceneObject(p_mgr, ID_Entity),
	m_dRadius(1), m_Dir(Vector3::NEGATIVE_UNIT_Z){

}

Arrow::Arrow(AbstractSceneManager * p_mgr, double aRadius, const Vector3 &aDir)
	: SceneObject(p_mgr, ID_Entity),
	m_dRadius(aRadius), m_Dir(aDir){
	
}

const Vector3 &Arrow::getDir() const{
	return m_Dir;
}

void Arrow::setDir(const Vector3 &aDir){
	m_Dir = aDir;
}

double Arrow::getRadius(){
	return m_dRadius;
}

void Arrow::setRadius(double aRadius){
	m_dRadius = aRadius;
}

void Arrow::render(){
	if(!m_pNode || !m_bVisible || !m_pNode->getParent())
		return ;

	glDisable(GL_LIGHTING);

//	double dAngle;
//	Vector3 vAxis;
//	m_pNode->getAbsoluteOrientation().toAngleAxis(dAngle,vAxis);
	Vector3 vPosition = m_pNode->getParent()->getAbsolutePosition();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(vPosition.x(),vPosition.y(),vPosition.z());
//	glRotatef(dAngle,vAxis.x(),vAxis.y(),vAxis.z());

	double coneRadius = m_dRadius / 12.0f;
	double coneHeight = m_dRadius * 3 / 10.0f;
	double back = 4;

	glBegin(GL_LINES);
	glVertex3f(back * -m_Dir.x(), back * -m_Dir.y(), back * -m_Dir.z());
	glVertex3f(m_dRadius * -m_Dir.x(), m_dRadius * -m_Dir.y(), -m_dRadius * m_Dir.z());
	glEnd();

	double conepos = coneHeight + back;
	glTranslatef(conepos * -m_Dir.x(), conepos * -m_Dir.y(), conepos * -m_Dir.z());

	Euler e = getRotationTo(Vector3(0, 0, 1), m_Dir).toEuler(Euler::ZXY);
	glRotatef(e.z, 0.0f, 0.0f, 1.0f);
	glRotatef(e.x, 1.0f, 0.0f, 0.0f);
	glRotatef(e.y, 0.0f, 1.0f, 0.0f);
	/*if(m_Dir.x() > 0.0f)
		glRotatef(90.0f,0.0f,1.0f,0.0f);
	if(m_Dir.y() > 0.0f)
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);*/

	auxSolidCone(m_dRadius / 12.0f, m_dRadius * 3 / 10.0f);

	glPopMatrix();

	glEnable(GL_LIGHTING);
}
