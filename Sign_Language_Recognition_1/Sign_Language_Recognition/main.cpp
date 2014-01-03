#include "sign_language_recognition.h"
#include <QtWidgets/QApplication>
using namespace Swift;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Sign_Language_Recognition w;
	w.show();
	return a.exec();
}
