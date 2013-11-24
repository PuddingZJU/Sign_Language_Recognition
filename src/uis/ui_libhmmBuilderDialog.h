/********************************************************************************
** Form generated from reading UI file 'libhmmBuilderDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIBHMMBUILDERDIALOG_H
#define UI_LIBHMMBUILDERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_libhmmBuilderDialogClass
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_InputFilePath;
    QLineEdit *lineEdit_InputFilePath;
    QHBoxLayout *hboxLayout1;
    QLabel *label_OutputFilePath;
    QLineEdit *lineEdit_OutputFilePath;
    QHBoxLayout *hboxLayout2;
    QPushButton *pushButton_Generate;
    QPushButton *pushButton_Save;
    QHBoxLayout *hboxLayout3;
    QPushButton *pushButton_Test1;
    QPushButton *pushButton_Test2;

    void setupUi(QWidget *libhmmBuilderDialogClass)
    {
        if (libhmmBuilderDialogClass->objectName().isEmpty())
            libhmmBuilderDialogClass->setObjectName(QStringLiteral("libhmmBuilderDialogClass"));
        libhmmBuilderDialogClass->resize(326, 133);
        vboxLayout = new QVBoxLayout(libhmmBuilderDialogClass);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(9, 9, 9, 9);
        vboxLayout->setObjectName(QStringLiteral("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        label_InputFilePath = new QLabel(libhmmBuilderDialogClass);
        label_InputFilePath->setObjectName(QStringLiteral("label_InputFilePath"));

        hboxLayout->addWidget(label_InputFilePath);

        lineEdit_InputFilePath = new QLineEdit(libhmmBuilderDialogClass);
        lineEdit_InputFilePath->setObjectName(QStringLiteral("lineEdit_InputFilePath"));

        hboxLayout->addWidget(lineEdit_InputFilePath);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QStringLiteral("hboxLayout1"));
        label_OutputFilePath = new QLabel(libhmmBuilderDialogClass);
        label_OutputFilePath->setObjectName(QStringLiteral("label_OutputFilePath"));

        hboxLayout1->addWidget(label_OutputFilePath);

        lineEdit_OutputFilePath = new QLineEdit(libhmmBuilderDialogClass);
        lineEdit_OutputFilePath->setObjectName(QStringLiteral("lineEdit_OutputFilePath"));

        hboxLayout1->addWidget(lineEdit_OutputFilePath);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QStringLiteral("hboxLayout2"));
        pushButton_Generate = new QPushButton(libhmmBuilderDialogClass);
        pushButton_Generate->setObjectName(QStringLiteral("pushButton_Generate"));

        hboxLayout2->addWidget(pushButton_Generate);

        pushButton_Save = new QPushButton(libhmmBuilderDialogClass);
        pushButton_Save->setObjectName(QStringLiteral("pushButton_Save"));

        hboxLayout2->addWidget(pushButton_Save);


        vboxLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QStringLiteral("hboxLayout3"));
        pushButton_Test1 = new QPushButton(libhmmBuilderDialogClass);
        pushButton_Test1->setObjectName(QStringLiteral("pushButton_Test1"));

        hboxLayout3->addWidget(pushButton_Test1);

        pushButton_Test2 = new QPushButton(libhmmBuilderDialogClass);
        pushButton_Test2->setObjectName(QStringLiteral("pushButton_Test2"));

        hboxLayout3->addWidget(pushButton_Test2);


        vboxLayout->addLayout(hboxLayout3);


        retranslateUi(libhmmBuilderDialogClass);

        QMetaObject::connectSlotsByName(libhmmBuilderDialogClass);
    } // setupUi

    void retranslateUi(QWidget *libhmmBuilderDialogClass)
    {
        libhmmBuilderDialogClass->setWindowTitle(QApplication::translate("libhmmBuilderDialogClass", "libhmmBuilderDialog", 0));
        label_InputFilePath->setText(QApplication::translate("libhmmBuilderDialogClass", "InputFilePath: ", 0));
        lineEdit_InputFilePath->setText(QApplication::translate("libhmmBuilderDialogClass", "D:\\test", 0));
        label_OutputFilePath->setText(QApplication::translate("libhmmBuilderDialogClass", "OutputFilePath:", 0));
        lineEdit_OutputFilePath->setText(QApplication::translate("libhmmBuilderDialogClass", "D:\\test", 0));
        pushButton_Generate->setText(QApplication::translate("libhmmBuilderDialogClass", "Generate", 0));
        pushButton_Save->setText(QApplication::translate("libhmmBuilderDialogClass", "Save", 0));
        pushButton_Test1->setText(QApplication::translate("libhmmBuilderDialogClass", "Test1", 0));
        pushButton_Test2->setText(QApplication::translate("libhmmBuilderDialogClass", "Test2", 0));
    } // retranslateUi

};

namespace Ui {
    class libhmmBuilderDialogClass: public Ui_libhmmBuilderDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIBHMMBUILDERDIALOG_H
