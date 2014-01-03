/********************************************************************************
** Form generated from reading UI file 'sign_language_recognition.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGN_LANGUAGE_RECOGNITION_H
#define UI_SIGN_LANGUAGE_RECOGNITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Sign_Language_RecognitionClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Sign_Language_RecognitionClass)
    {
        if (Sign_Language_RecognitionClass->objectName().isEmpty())
            Sign_Language_RecognitionClass->setObjectName(QStringLiteral("Sign_Language_RecognitionClass"));
        Sign_Language_RecognitionClass->resize(600, 400);
        menuBar = new QMenuBar(Sign_Language_RecognitionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        Sign_Language_RecognitionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Sign_Language_RecognitionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Sign_Language_RecognitionClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Sign_Language_RecognitionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Sign_Language_RecognitionClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Sign_Language_RecognitionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Sign_Language_RecognitionClass->setStatusBar(statusBar);

        retranslateUi(Sign_Language_RecognitionClass);

        QMetaObject::connectSlotsByName(Sign_Language_RecognitionClass);
    } // setupUi

    void retranslateUi(QMainWindow *Sign_Language_RecognitionClass)
    {
        Sign_Language_RecognitionClass->setWindowTitle(QApplication::translate("Sign_Language_RecognitionClass", "Sign_Language_Recognition", 0));
    } // retranslateUi

};

namespace Ui {
    class Sign_Language_RecognitionClass: public Ui_Sign_Language_RecognitionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGN_LANGUAGE_RECOGNITION_H
