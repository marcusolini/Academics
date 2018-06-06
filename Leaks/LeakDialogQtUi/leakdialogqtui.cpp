//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "leakdialogqtui.h"
#include "ui_leakdialogqtui.h"

#include "progressdialog.h"

#include <QMessageBox>

LeakDialogQtUi::LeakDialogQtUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LeakDialogQtUi)
{
    ui->setupUi(this);

    ui->NewOperIterationsLineEdit->setDisabled(true);
    ui->NewOperBytesPerLineEdit->setDisabled(true);
    ui->RunTestsButton->setDisabled(true);
    ui->CloseButton->setEnabled(true);

    //ui->NewOperIterationsLineEdit->setInputMask("D");
    //ui->NewOperIterationsLineEdit->setInputMask("999999999999999");
    //ui->NewOperIterationsLineEdit->setValidator( new QIntValidator(0, 999999999999999, this));

    connect(ui->CloseButton, SIGNAL (released()), this, SLOT(handleCloseButton()));
    connect(ui->newOperatorCheckBox, SIGNAL (stateChanged(int)), this, SLOT(handleNewOperatorCheckBox(int)));
    connect(ui->RunTestsButton, SIGNAL (released()), this, SLOT(handleRunTestsButton()));
}

LeakDialogQtUi::~LeakDialogQtUi()
{
    delete ui;
}

void LeakDialogQtUi::handleCloseButton()
{
    this->close();
}

void LeakDialogQtUi::handleRunTestsButton()
{
    // Collect Settings
    QString sIterations;
    QString sBytesPer;
    int nIterations = 0;
    int nBytesPer = 0;
    bool bIntConversion = false;

    bool bRunTests = false;

    if ( ui->newOperatorCheckBox->isChecked() )
    {
        sIterations = ui->NewOperIterationsLineEdit->text();
        nIterations = sIterations.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nIterations = 0;
        }

        sBytesPer = ui->NewOperBytesPerLineEdit->text();
        nBytesPer = sBytesPer.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nBytesPer = 0;
        }

        if ( (0 <nIterations) && (0 < nBytesPer) )
        {
            bRunTests = true;

            CResourceLeakTest resourceLeakTest(ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR, nIterations, nBytesPer);
            gTests.push_back(resourceLeakTest);
        }
        else
        {
            bRunTests = false;
            QMessageBox msgBox;
            msgBox.setText("Invalid Input");
            msgBox.setInformativeText("Review the input values and resubmit.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

    if ( true == bRunTests)
    {
        ProgressDialog w(this);
        w.show();
        w.exec();

        gTests.clear();
    }
}

void LeakDialogQtUi::handleNewOperatorCheckBox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->NewOperIterationsLineEdit->setEnabled(true);
        ui->NewOperBytesPerLineEdit->setEnabled(true);
        ui->RunTestsButton->setEnabled(true);
        break;
    case Qt::Unchecked:
        ui->NewOperIterationsLineEdit->setDisabled(true);
        ui->NewOperBytesPerLineEdit->setDisabled(true);
        ui->RunTestsButton->setDisabled(true);
        break;
    }
}
