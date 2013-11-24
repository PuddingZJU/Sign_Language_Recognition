#ifndef SIGNLANGUAGERECOGNITION_H
#define SIGNLANGUAGERECOGNITION_H

#include <QtWidgets/QMainWindow>
#include "ui_signlanguagerecognition.h"

class SignLanguageRecognition : public QMainWindow
{
	Q_OBJECT

public:
	SignLanguageRecognition(QWidget *parent = 0);
	~SignLanguageRecognition();


private:
	Ui::SignLanguageRecognitionClass ui;
};

#endif // SIGNLANGUAGERECOGNITION_H
