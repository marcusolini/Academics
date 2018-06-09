/********************************************************************************
** Form generated from reading UI file 'leakdialogqtui.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEAKDIALOGQTUI_H
#define UI_LEAKDIALOGQTUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LeakDialogQtUi
{
public:
    QLabel *Testslabel;
    QLabel *IterationsLabel;
    QLabel *BytesPerLabel;
    QPushButton *RunTestsButton;
    QPushButton *CloseButton;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QCheckBox *newOperatorCheckBox;
    QLineEdit *newOperIterationsLineEdit;
    QLineEdit *newOperBytesPerLineEdit;
    QWidget *widget1;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *mallocCheckBox;
    QLineEdit *mallocIterationsLineEdit;
    QLineEdit *mallocBytesPerLineEdit;
    QWidget *widget2;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *callocCheckBox;
    QLineEdit *callocIterationsLineEdit;
    QLineEdit *callocBytesPerLineEdit;

    void setupUi(QDialog *LeakDialogQtUi)
    {
        if (LeakDialogQtUi->objectName().isEmpty())
            LeakDialogQtUi->setObjectName(QStringLiteral("LeakDialogQtUi"));
        LeakDialogQtUi->resize(657, 156);
        Testslabel = new QLabel(LeakDialogQtUi);
        Testslabel->setObjectName(QStringLiteral("Testslabel"));
        Testslabel->setGeometry(QRect(11, 10, 78, 20));
        Testslabel->setScaledContents(false);
        IterationsLabel = new QLabel(LeakDialogQtUi);
        IterationsLabel->setObjectName(QStringLiteral("IterationsLabel"));
        IterationsLabel->setGeometry(QRect(11, 44, 117, 20));
        BytesPerLabel = new QLabel(LeakDialogQtUi);
        BytesPerLabel->setObjectName(QStringLiteral("BytesPerLabel"));
        BytesPerLabel->setGeometry(QRect(11, 80, 107, 20));
        RunTestsButton = new QPushButton(LeakDialogQtUi);
        RunTestsButton->setObjectName(QStringLiteral("RunTestsButton"));
        RunTestsButton->setGeometry(QRect(440, 120, 104, 28));
        CloseButton = new QPushButton(LeakDialogQtUi);
        CloseButton->setObjectName(QStringLiteral("CloseButton"));
        CloseButton->setGeometry(QRect(560, 120, 83, 28));
        widget = new QWidget(LeakDialogQtUi);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(140, 11, 147, 96));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        newOperatorCheckBox = new QCheckBox(widget);
        newOperatorCheckBox->setObjectName(QStringLiteral("newOperatorCheckBox"));

        verticalLayout->addWidget(newOperatorCheckBox);

        newOperIterationsLineEdit = new QLineEdit(widget);
        newOperIterationsLineEdit->setObjectName(QStringLiteral("newOperIterationsLineEdit"));
        newOperIterationsLineEdit->setEnabled(true);
        newOperIterationsLineEdit->setMouseTracking(true);
        newOperIterationsLineEdit->setFocusPolicy(Qt::StrongFocus);
        newOperIterationsLineEdit->setMaxLength(999999999);
        newOperIterationsLineEdit->setClearButtonEnabled(true);

        verticalLayout->addWidget(newOperIterationsLineEdit);

        newOperBytesPerLineEdit = new QLineEdit(widget);
        newOperBytesPerLineEdit->setObjectName(QStringLiteral("newOperBytesPerLineEdit"));
        newOperBytesPerLineEdit->setEnabled(true);
        newOperBytesPerLineEdit->setMaxLength(999999999);
        newOperBytesPerLineEdit->setClearButtonEnabled(true);

        verticalLayout->addWidget(newOperBytesPerLineEdit);

        widget1 = new QWidget(LeakDialogQtUi);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(310, 11, 162, 96));
        verticalLayout_2 = new QVBoxLayout(widget1);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        mallocCheckBox = new QCheckBox(widget1);
        mallocCheckBox->setObjectName(QStringLiteral("mallocCheckBox"));

        verticalLayout_2->addWidget(mallocCheckBox);

        mallocIterationsLineEdit = new QLineEdit(widget1);
        mallocIterationsLineEdit->setObjectName(QStringLiteral("mallocIterationsLineEdit"));
        mallocIterationsLineEdit->setMaxLength(999999999);
        mallocIterationsLineEdit->setClearButtonEnabled(true);

        verticalLayout_2->addWidget(mallocIterationsLineEdit);

        mallocBytesPerLineEdit = new QLineEdit(widget1);
        mallocBytesPerLineEdit->setObjectName(QStringLiteral("mallocBytesPerLineEdit"));
        mallocBytesPerLineEdit->setMaxLength(999999999);
        mallocBytesPerLineEdit->setClearButtonEnabled(true);

        verticalLayout_2->addWidget(mallocBytesPerLineEdit);

        widget2 = new QWidget(LeakDialogQtUi);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(490, 11, 154, 96));
        verticalLayout_3 = new QVBoxLayout(widget2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        callocCheckBox = new QCheckBox(widget2);
        callocCheckBox->setObjectName(QStringLiteral("callocCheckBox"));

        verticalLayout_3->addWidget(callocCheckBox);

        callocIterationsLineEdit = new QLineEdit(widget2);
        callocIterationsLineEdit->setObjectName(QStringLiteral("callocIterationsLineEdit"));
        callocIterationsLineEdit->setMaxLength(999999999);
        callocIterationsLineEdit->setClearButtonEnabled(true);

        verticalLayout_3->addWidget(callocIterationsLineEdit);

        callocBytesPerLineEdit = new QLineEdit(widget2);
        callocBytesPerLineEdit->setObjectName(QStringLiteral("callocBytesPerLineEdit"));
        callocBytesPerLineEdit->setMaxLength(999999999);
        callocBytesPerLineEdit->setClearButtonEnabled(true);

        verticalLayout_3->addWidget(callocBytesPerLineEdit);


        retranslateUi(LeakDialogQtUi);

        QMetaObject::connectSlotsByName(LeakDialogQtUi);
    } // setupUi

    void retranslateUi(QDialog *LeakDialogQtUi)
    {
        LeakDialogQtUi->setWindowTitle(QApplication::translate("LeakDialogQtUi", "Resource Leak Testing~~~~~~", nullptr));
        Testslabel->setText(QApplication::translate("LeakDialogQtUi", "TESTS:~~~~", nullptr));
        IterationsLabel->setText(QApplication::translate("LeakDialogQtUi", "ITERATIONS:~~~~", nullptr));
        BytesPerLabel->setText(QApplication::translate("LeakDialogQtUi", "BYTES PER:~~~~", nullptr));
        RunTestsButton->setText(QApplication::translate("LeakDialogQtUi", "Run Tests~~~~", nullptr));
        CloseButton->setText(QApplication::translate("LeakDialogQtUi", "Close~~~~", nullptr));
        newOperatorCheckBox->setText(QApplication::translate("LeakDialogQtUi", "new operator~~~~", nullptr));
        newOperIterationsLineEdit->setInputMask(QString());
        newOperIterationsLineEdit->setPlaceholderText(QString());
        mallocCheckBox->setText(QApplication::translate("LeakDialogQtUi", "malloc Function~~~~", nullptr));
        callocCheckBox->setText(QApplication::translate("LeakDialogQtUi", "calloc function~~~~", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LeakDialogQtUi: public Ui_LeakDialogQtUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEAKDIALOGQTUI_H
