/********************************************************************************
** Form generated from reading UI file 'signlanguagerecognition.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNLANGUAGERECOGNITION_H
#define UI_SIGNLANGUAGERECOGNITION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignLanguageRecognitionClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QWidget *widget_2;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SignLanguageRecognitionClass)
    {
        if (SignLanguageRecognitionClass->objectName().isEmpty())
            SignLanguageRecognitionClass->setObjectName(QStringLiteral("SignLanguageRecognitionClass"));
        SignLanguageRecognitionClass->setEnabled(true);
        SignLanguageRecognitionClass->resize(761, 610);
        SignLanguageRecognitionClass->setMinimumSize(QSize(761, 610));
        centralWidget = new QWidget(SignLanguageRecognitionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(0, 0, 401, 371));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(449, 29, 181, 231));
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 60, 181, 81));
        radioButton = new QRadioButton(groupBox_2);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(10, 20, 82, 17));
        radioButton_2 = new QRadioButton(groupBox_2);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(10, 50, 82, 17));

        horizontalLayout->addWidget(widget);

        SignLanguageRecognitionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SignLanguageRecognitionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 761, 21));
        SignLanguageRecognitionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SignLanguageRecognitionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SignLanguageRecognitionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(SignLanguageRecognitionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SignLanguageRecognitionClass->setStatusBar(statusBar);

        retranslateUi(SignLanguageRecognitionClass);

        QMetaObject::connectSlotsByName(SignLanguageRecognitionClass);
    } // setupUi

    void retranslateUi(QMainWindow *SignLanguageRecognitionClass)
    {
        SignLanguageRecognitionClass->setWindowTitle(QApplication::translate("SignLanguageRecognitionClass", "SignLanguageRecognition", 0));
        groupBox->setTitle(QApplication::translate("SignLanguageRecognitionClass", "GroupBox", 0));
        groupBox_2->setTitle(QApplication::translate("SignLanguageRecognitionClass", "GroupBox", 0));
        radioButton->setText(QApplication::translate("SignLanguageRecognitionClass", "RadioButton", 0));
        radioButton_2->setText(QApplication::translate("SignLanguageRecognitionClass", "RadioButton", 0));
    } // retranslateUi

};

namespace Ui {
    class SignLanguageRecognitionClass: public Ui_SignLanguageRecognitionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNLANGUAGERECOGNITION_H
