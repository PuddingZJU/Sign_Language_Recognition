#include <cassert>
#include "Ground.h"

std::string int2str(int n)
{
	std::string result, tmp;
	char c, *ch;
	int i, k, r, m;
	int a = std::abs(n);
	for (k=0, r=a/10; r!=0; k++)
	{
		a = r;
		r = a/10;
	}
	ch = new char[k+1];

	a = std::abs(n);
	for (i=0; i<=k; i++)
	{
		r=a/10;
		m=a%10;
		c = m+48;
		ch[k-i] = c;
		a=r;
	}
	ch[k+1] = '\0';

	if (n < 0)
		result = "-";
	result += std::string(ch);

	return result;
}

namespace Swift
{
;

Ground::Ground()
{	
	m_viewMode = PERSPECTIVE;

	GLfloat x,y,z;
	y = 0.0f;

	for(int nCol=0; nCol<51; nCol++)
	{
		x = -500.0 + nCol * 20;

		for(int nRow=0; nRow<51; nRow++)
		{
			z = -500.0 + nRow * 20;

			m_gridLines[nCol][nRow][0] = x;
			m_gridLines[nCol][nRow][1] = 0.0f;
			m_gridLines[nCol][nRow][2] = z;

		}
	}
	m_strHorizontal = "X: ";
	m_strVertical = "Z: ";
	m_nHorizontalSign = 1;
	m_nVertitalSign = 1;
}

Ground::~Ground()
{
}

GLViewMode Ground::getViewMode()
{
	return m_viewMode;
}

void Ground::setViewMode(GLViewMode viewMode)
{
	m_viewMode = viewMode;
}

void Ground::render()
{
	GLboolean bFlag = glIsEnabled(GL_LIGHTING);
	if (bFlag)
		glDisable(GL_LIGHTING);

	//// Save viewing transformation
	glPushMatrix();

	//mode transformation according to view mode
	switch(m_viewMode)
	{
	case PERSPECTIVE:
		drawTextPerspective();
		break;
	case ORTHO_FRONT:
		glTranslatef(0.0f,0.0f,-180.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		m_strHorizontal = "X: ";
		m_nHorizontalSign = 1;
		m_strVertical = "Y: ";
		m_nVertitalSign = -1;
		drawTextOrthoFront();
		break;
	case ORTHO_BACK: 
		glTranslatef(0.0f,0.0f,180.0f);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		m_strHorizontal = "X: ";
		m_nHorizontalSign = -1;
		m_strVertical = "Y: ";
		m_nVertitalSign = -1;
		drawTextOrthoBack();
		break;
	case ORTHO_RIGHT:
		glTranslatef(-180.0f,0.0f,0.0f);
		glRotatef(-90.0f,0.0f,0.0f,1.0f);
		m_strHorizontal = "Z: ";
		m_nHorizontalSign = -1;
		m_strVertical = "Y: ";
		m_nVertitalSign = -1;
		drawTextOrthoRight();
		break;
	case ORTHO_LEFT: 
		glTranslatef(180.0f,0.0f,0.0f);
		glRotatef(90.0f,0.0f,0.0f,1.0f); 
		m_strHorizontal = "Z: ";
		m_nHorizontalSign = 1;
		m_strVertical = "Y: ";
		m_nVertitalSign = -1;
		drawTextOrthoLeft();
		break;
	case ORTHO_TOP: 
		m_strHorizontal = "X: ";
		m_nHorizontalSign = 1;
		m_strVertical = "Z: ";
		m_nVertitalSign = 1;
		drawTextOrthoTop();
		break;
	case ORTHO_BOTTOM: 
		glTranslatef(0.0f,180.0f,0.0f);
		glRotatef(180.0f,1.0f,0.0f,0.0f); 
		m_strHorizontal = "X: ";
		m_nHorizontalSign = 1;
		m_strVertical = "Z: ";
		m_nVertitalSign = -1;
		drawTextOrthoBottom();
		break;
	}

	drawGrid();

	// Restore the viewing transformation
	glPopMatrix();

	//GLfloat lightAmbient[] = {0.5f,0.5f,0.5f,1.0f};
	if (bFlag)
		glEnable(GL_LIGHTING);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//glEnable(GL_LIGHT0);
}

void Ground::drawGrid()
{
	glPushMatrix();

	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int col = 0; col < 51; col++)
	{
		if(col == 25)
		{
			glColor3f(0.3f,0.3f,0.3f);
		}
		else if(col % 5 == 0)
		{
			glColor3f(0.4f,0.4f,0.4f);
		}
		else
		{
			glColor3f(0.5f,0.5f,0.5f);
		}

		glVertex3f(m_gridLines[col][0][0],m_gridLines[col][0][1],m_gridLines[col][0][2]);
		glVertex3f(m_gridLines[col][50][0],m_gridLines[col][50][1],m_gridLines[col][50][2]);
	}

	for(int row = 0; row < 51; row++)
	{
		if(row == 25)
		{
			glColor3f(0.3f,0.3f,0.3f);
		}
		else if(row % 5 == 0)
		{
			glColor3f(0.4f,0.4f,0.4f);
		}
		else
		{
			glColor3f(0.5f,0.5f,0.5f);
		}
		glVertex3f(m_gridLines[0][row][0],m_gridLines[0][row][1],m_gridLines[0][row][2]);
		glVertex3f(m_gridLines[50][row][0],m_gridLines[50][row][1],m_gridLines[50][row][2]);
	}

	glEnd();

	glPopMatrix();
}

void Ground::drawTextPerspective()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = -2;
	const int xoffset = 2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "X: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Z: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoFront()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = -2;
	const int xoffset = 2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "X: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Y: ";
		scoord += int2str(-i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoBack()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = 2;
	const int xoffset = -2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glRotatef(180,0,0,1);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "X: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(180,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Y: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(180,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoLeft()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = 2;
	const int xoffset = 2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glRotatef(-90,0,0,1);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "Y: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(-90,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Z: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(-90,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoRight()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = -2;
	const int xoffset = -2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glRotatef(90,0,0,1);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "Y: ";
		scoord += int2str(-i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(90,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Z: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glRotatef(90,0,0,1);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoTop()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = -2;
	const int xoffset = 2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "X: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Z: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

void Ground::drawTextOrthoBottom()
{
	glColor3f(0.3f,0.3f,0.3f);

	const int zoffset = -2;
	const int xoffset = 2;
	for (int i=-500; i<=500; i+=100) 
	{
		std::string scoord;
		int is, lCount;

		// Don't display origin
		if (i == 0) 
		{
			scoord = "0";
			lCount = (int)scoord.size();
			glPushMatrix();
			glTranslatef(i+xoffset,0,zoffset);
			glRotatef(-90,1,0,0);
			glScalef(.03,.03,.03);
			for (is = 0; is < lCount; is++) 
			{
				char lC = scoord.at(is);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
			}
			glPopMatrix();

			continue;
		}

		// Horizontal coordinates
		scoord = "X: ";
		scoord += int2str(i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(i+xoffset,0,zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();

		// Vertical coordinates
		scoord = "Z: ";
		scoord += int2str(-i);
		lCount = (int)scoord.size();

		glPushMatrix();
		glTranslatef(xoffset,0,i+zoffset);
		glRotatef(-90,1,0,0);
		glScalef(.03,.03,.03);
		for (is = 0; is < lCount; is++) 
		{
			char lC = scoord.at(is);
			glutStrokeCharacter(GLUT_STROKE_ROMAN, lC);
		}
		glPopMatrix();
	}
}

} // namespace Swift
