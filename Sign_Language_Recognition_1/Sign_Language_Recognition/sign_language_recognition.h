#ifndef SIGN_LANGUAGE_RECOGNITION_H
#define SIGN_LANGUAGE_RECOGNITION_H

#include <QtWidgets/QMainWindow>
#include "ui_sign_language_recognition.h"

namespace Swift{
	class Sign_Language_Recognition : public QMainWindow
	{
		Q_OBJECT

	public:
		Sign_Language_Recognition(QWidget *parent = 0);
		~Sign_Language_Recognition();

	private:
		Ui::Sign_Language_RecognitionClass ui;
	};
}

#endif // SIGN_LANGUAGE_RECOGNITION_H
