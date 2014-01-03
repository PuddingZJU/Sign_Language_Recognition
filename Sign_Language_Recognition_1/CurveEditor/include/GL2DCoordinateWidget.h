#ifndef GL2DCOORDINATEWIDGET_H
#define GL2DCOORDINATEWIDGET_H

#include <QGLWidget>
#include "Curve.h"

const float MAXWIDTH = 10000.0;
const float MAXHEIGHT = 10000.0;
const float HALFMAXWIDTH = MAXWIDTH / 2;
const float HALFMAXHEIGHT = MAXHEIGHT / 2;
//const float CONTROLLENGTH = 10.0;

class GL2DCoordinateWidget : public QGLWidget
{
	Q_OBJECT

public:
	GL2DCoordinateWidget(QWidget *parent = 0);
	~GL2DCoordinateWidget();

private:
	int halfWinWidth, halfWinHeight;
	float transX, transY;
	float scale;
	QPoint startDragPos, leftTopPos, rightBottomPos;

public:
	ComplexCurve *curve;

protected:
	enum PointType{
		typeKeyPoint,
		typeControlPoint
	};

protected:
//	float curveStartX, curveStartY;
//	float factorX, factorY, factorX_1, factorY_1;

//	std::vector<PointControl> keyPoints;
//	std::vector<PointControl> controlPoints;

	std::vector<CurveKeyPoint> keyPoints;

	enum PointType currentPointType;
	int currentPointIndex;
	int currentControlPoint;	// 0-left, 1-right

	bool dragControl;
	bool dragKey;

protected:
	void InitializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	
	virtual void drawGrid();
	virtual void drawAxis();
	virtual void showCoorValues();

	// events
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	bool event(QEvent *event);

public:
	void drawCurve();
	void drawKeyPoints();
	void drawControlPoints();

public:
	// tools
	bool inBox(int a, int b);
	bool pointGreater(const CurveKeyPoint &a, const CurveKeyPoint &b) const;
};

#endif // GL2DCOORDINATEWIDGET_H
