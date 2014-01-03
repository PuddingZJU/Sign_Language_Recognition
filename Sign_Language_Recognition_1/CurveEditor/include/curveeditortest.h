#ifndef CURVEEDITORTEST_H
#define CURVEEDITORTEST_H

#include <QtGui/QMainWindow>
#include "ui_curveeditortest.h"

class GL2DCoordinateWidget;

class CurveEditorTest : public QMainWindow
{
	Q_OBJECT

public:
	CurveEditorTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CurveEditorTest();

private:
	Ui::CurveEditorTestClass ui;

	GL2DCoordinateWidget *curveView;
};

#endif // CURVEEDITORTEST_H
