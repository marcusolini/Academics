/********************************************************************************
** Form generated from reading UI file 'leakdialogqtui.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEAKDIALOGQTUI_H
#define UI_LEAKDIALOGQTUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LeakDialogQtUi
{
public:
    QLabel *Testslabel;
    QLabel *IterationsLabel;
    QLabel *BytesPerLabel;
    QCheckBox *newOperatorCheckBox;
    QLineEdit *NewOperIterationsLineEdit;
    QLineEdit *NewOperBytesPerLineEdit;
    QPushButton *RunTestsButton;
    QPushButton *CloseButton;

    void setupUi(QDialog *LeakDialogQtUi)
    {
        if (LeakDialogQtUi->objectName().isEmpty())
            LeakDialogQtUi->setObjectName(QStringLiteral("LeakDialogQtUi"));
        LeakDialogQtUi->resize(542, 300);
        Testslabel = new QLabel(LeakDialogQtUi);
        Testslabel->setObjectName(QStringLiteral("Testslabel"));
        Testslabel->setGeometry(QRect(10, 10, 78, 20));
        Testslabel->setScaledContents(false);
        IterationsLabel = new QLabel(LeakDialogQtUi);
        IterationsLabel->setObjectName(QStringLiteral("IterationsLabel"));
        IterationsLabel->setGeometry(QRect(10, 50, 117, 20));
        BytesPerLabel = new QLabel(LeakDialogQtUi);
        BytesPerLabel->setObjectName(QStringLiteral("BytesPerLabel"));
        BytesPerLabel->setGeometry(QRect(10, 80, 107, 20));
        newOperatorCheckBox = new QCheckBox(LeakDialogQtUi);
        newOperatorCheckBox->setObjectName(QStringLiteral("newOperatorCheckBox"));
        newOperatorCheckBox->setGeometry(QRect(140, 10, 147, 26));
        NewOperIterationsLineEdit = new QLineEdit(LeakDialogQtUi);
        NewOperIterationsLineEdit->setObjectName(QStringLiteral("NewOperIterationsLineEdit"));
        NewOperIterationsLineEdit->setEnabled(true);
        NewOperIterationsLineEdit->setGeometry(QRect(160, 50, 113, 28));
        NewOperIterationsLineEdit->setClearButtonEnabled(true);
        NewOperBytesPerLineEdit = new QLineEdit(LeakDialogQtUi);
        NewOperBytesPerLineEdit->setObjectName(QStringLiteral("NewOperBytesPerLineEdit"));
        NewOperBytesPerLineEdit->setEnabled(true);
        NewOperBytesPerLineEdit->setGeometry(QRect(160, 80, 113, 28));
        NewOperBytesPerLineEdit->setClearButtonEnabled(true);
        RunTestsButton = new QPushButton(LeakDialogQtUi);
        RunTestsButton->setObjectName(QStringLiteral("RunTestsButton"));
        RunTestsButton->setGeometry(QRect(300, 250, 104, 28));
        CloseButton = new QPushButton(LeakDialogQtUi);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(420, 250, 83, 28));

        retranslateUi(LeakDialogQtUi);

        QMetaObject::connectSlotsByName(LeakDialogQtUi);
    } // setupUi

    void retranslateUi(QDialog *LeakDialogQtUi)
    {
        LeakDialogQtUi->setWindowTitle(QApplication::translate("LeakDialogQtUi", "Resource Leak Testing", 0));
        Testslabel->setText(QApplication::translate("LeakDialogQtUi", "TESTS:~~~~", 0));
        IterationsLabel->setText(QApplication::translate("LeakDialogQtUi", "ITERATIONS:~~~~", 0));
        BytesPerLabel->setText(QApplication::translate("LeakDialogQtUi", "BYTES PER:~~~~", 0));
        newOperatorCheckBox->setText(QApplication::translate("LeakDialogQtUi", "new operator~~~~", 0));
        NewOperIterationsLineEdit->setInputMask(QString());
        NewOperIterationsLineEdit->setPlaceholderText(QString());
        RunTestsButton->setText(QApplication::translate("LeakDialogQtUi", "Run Tests~~~~", 0));
        CloseButton->setText(QApplication::translate("LeakDialogQtUi", "Close~~~~", 0));
    } // retranslateUi

};

namespace Ui {
    class LeakDialogQtUi: public Ui_LeakDialogQtUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAKDIALOGQTUI_H
