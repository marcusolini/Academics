/********************************************************************************
** Form generated from reading UI file 'progressdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSDIALOG_H
#define UI_PROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ProgressDialog
{
public:
    QLabel *RunTestsLabel;
    QProgressBar *RunTestsProgressBar;
    QPushButton *StopPushButton;
    QPushButton *PausePushButton;

    void setupUi(QDialog *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QStringLiteral("ProgressDialog"));
        ProgressDialog->resize(414, 94);
        ProgressDialog->setModal(true);
        RunTestsLabel = new QLabel(ProgressDialog);
        RunTestsLabel->setObjectName(QStringLiteral("RunTestsLabel"));
        RunTestsLabel->setGeometry(QRect(10, 10, 166, 20));
        RunTestsProgressBar = new QProgressBar(ProgressDialog);
        RunTestsProgressBar->setObjectName(QStringLiteral("RunTestsProgressBar"));
        RunTestsProgressBar->setGeometry(QRect(10, 40, 271, 23));
        RunTestsProgressBar->setValue(24);
        RunTestsProgressBar->setTextVisible(false);
        StopPushButton = new QPushButton(ProgressDialog);
        StopPushButton->setObjectName(QStringLiteral("StopPushButton"));
        StopPushButton->setGeometry(QRect(310, 10, 83, 28));
        PausePushButton = new QPushButton(ProgressDialog);
        PausePushButton->setObjectName(QStringLiteral("PausePushButton"));
        PausePushButton->setGeometry(QRect(310, 50, 83, 28));

        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "Resource Leak Tests~~~~~", 0));
        RunTestsLabel->setText(QApplication::translate("ProgressDialog", "Starting Leak Tests...~~~~", 0));
        StopPushButton->setText(QApplication::translate("ProgressDialog", "Stop~~~~", 0));
        PausePushButton->setText(QApplication::translate("ProgressDialog", "Pause~~~~", 0));
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSDIALOG_H
