//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "leakdialogqtui.h"
#include "ui_leakdialogqtui.h"

#include "progressdialog.h"

#include <QIntValidator>
#include <QMessageBox>

LeakDialogQtUi::LeakDialogQtUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LeakDialogQtUi)
{
    ui->setupUi(this);

    ui->newOperatorCheckBox->setEnabled(true);
    ui->newOperIterationsLineEdit->setDisabled(true);
    ui->newOperBytesPerLineEdit->setDisabled(true);
    ui->newOperIterationsLineEdit->setValidator( new QIntValidator(1, 999999999, this));
    ui->newOperBytesPerLineEdit->setValidator( new QIntValidator(1, 999999999, this));

    ui->mallocCheckBox->setEnabled(true);
    ui->mallocIterationsLineEdit->setDisabled(true);
    ui->mallocBytesPerLineEdit->setDisabled(true);
    ui->mallocIterationsLineEdit->setValidator( new QIntValidator(1, 999999999, this));
    ui->mallocBytesPerLineEdit->setValidator( new QIntValidator(1, 999999999, this));

    ui->callocCheckBox->setEnabled(true);
    ui->callocIterationsLineEdit->setDisabled(true);
    ui->callocBytesPerLineEdit->setDisabled(true);
    ui->callocIterationsLineEdit->setValidator( new QIntValidator(1, 999999999, this));
    ui->callocBytesPerLineEdit->setValidator( new QIntValidator(1, 999999999, this));

#ifdef _WIN32
    ui->handleCheckBox->setEnabled(true);
#else
    ui->handleCheckBox->setDisabled(true);
#endif // _WIN32
    ui->handleIterationsLineEdit->setDisabled(true);
    ui->handleBytesPerLineEdit->setDisabled(true);
    ui->handleIterationsLineEdit->setValidator( new QIntValidator(1, 999999999, this));
    ui->handleBytesPerLineEdit->setValidator( new QIntValidator(1, 999999999, this));

    ui->RunTestsButton->setDisabled(true);
    ui->CloseButton->setEnabled(true);


    connect(ui->CloseButton, SIGNAL (released()), this, SLOT(handleCloseButton()));
    connect(ui->newOperatorCheckBox, SIGNAL (stateChanged(int)), this, SLOT(handleNewOperatorCheckBox(int)));
    connect(ui->mallocCheckBox, SIGNAL (stateChanged(int)), this, SLOT(handleMallocCheckBox(int)));
    connect(ui->callocCheckBox, SIGNAL (stateChanged(int)), this, SLOT(handleCallocCheckBox(int)));
    connect(ui->handleCheckBox, SIGNAL (stateChanged(int)), this, SLOT(handleHandleCheckBox(int)));
    connect(ui->RunTestsButton, SIGNAL (released()), this, SLOT(handleRunTestsButton()));
}

LeakDialogQtUi::~LeakDialogQtUi()
{
    delete ui;
}

void LeakDialogQtUi::handleCloseButton()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Close"));
    msgBox.setInformativeText(tr("Close Resource Leak Test?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int nRet = msgBox.exec();

    if ( QMessageBox::Yes == nRet)
    {
        this->close();
    }
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
        sIterations = ui->newOperIterationsLineEdit->text();
        nIterations = sIterations.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nIterations = 0;
        }

        sBytesPer = ui->newOperBytesPerLineEdit->text();
        nBytesPer = sBytesPer.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nBytesPer = 0;
        }

        if ( (0 <nIterations) && (0 < nBytesPer) )
        {
            bRunTests = true;

            CResourceLeakTest resourceLeakTest(CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR, nIterations, nBytesPer);
            gTests.push_back(resourceLeakTest);
        }
        else
        {
            bRunTests = false;
            QMessageBox msgBox;
            msgBox.setText(tr("Invalid Input"));
            msgBox.setInformativeText("Review the new operator input values and resubmit.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }


    if ( ui->mallocCheckBox->isChecked() )
    {
        sIterations = ui->mallocIterationsLineEdit->text();
        nIterations = sIterations.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nIterations = 0;
        }

        sBytesPer = ui->mallocBytesPerLineEdit->text();
        nBytesPer = sBytesPer.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nBytesPer = 0;
        }

        if ( (0 <nIterations) && (0 < nBytesPer) )
        {
            bRunTests = true;

            CResourceLeakTest resourceLeakTest(CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION, nIterations, nBytesPer);
            gTests.push_back(resourceLeakTest);
        }
        else
        {
            bRunTests = false;
            QMessageBox msgBox;
            msgBox.setText(tr("Invalid Input"));
            msgBox.setInformativeText(tr("Review the malloc function input values and resubmit."));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }


    if ( ui->callocCheckBox->isChecked() )
    {
        sIterations = ui->callocIterationsLineEdit->text();
        nIterations = sIterations.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nIterations = 0;
        }

        sBytesPer = ui->callocBytesPerLineEdit->text();
        nBytesPer = sBytesPer.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nBytesPer = 0;
        }

        if ( (0 <nIterations) && (0 < nBytesPer) )
        {
            bRunTests = true;

            CResourceLeakTest resourceLeakTest(CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION, nIterations, nBytesPer);
            gTests.push_back(resourceLeakTest);
        }
        else
        {
            bRunTests = false;
            QMessageBox msgBox;
            msgBox.setText(tr("Invalid Input"));
            msgBox.setInformativeText(tr("Review the calloc function input values and resubmit."));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

    if ( ui->handleCheckBox->isChecked() )
    {
        sIterations = ui->handleIterationsLineEdit->text();
        nIterations = sIterations.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nIterations = 0;
        }

        sBytesPer = ui->handleBytesPerLineEdit->text();
        nBytesPer = sBytesPer.toInt(&bIntConversion);
        if (false == bIntConversion)
        {
            nBytesPer = 0;
        }

        if ( (0 <nIterations) && (0 < nBytesPer) )
        {
            bRunTests = true;

            CResourceLeakTest resourceLeakTest(CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION, nIterations, nBytesPer);
            gTests.push_back(resourceLeakTest);
        }
        else
        {
            bRunTests = false;
            QMessageBox msgBox;
            msgBox.setText(tr("Invalid Input"));
            msgBox.setInformativeText(tr("Review the handle function input values and resubmit."));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }

    if ( true == bRunTests)
    {
        ProgressDialog w(this);
        w.show();
        w.exec();

        QMessageBox msgBox;
        msgBox.setText(tr("Tests Complete"));
        msgBox.setInformativeText(tr("Resources will remain leaked until application is closed."));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        gTests.clear();
    }
}

void LeakDialogQtUi::handleNewOperatorCheckBox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->newOperIterationsLineEdit->setEnabled(true);
        ui->newOperBytesPerLineEdit->setEnabled(true);
        ui->RunTestsButton->setEnabled(true);
        break;
    case Qt::Unchecked:
        ui->newOperIterationsLineEdit->setDisabled(true);
        ui->newOperBytesPerLineEdit->setDisabled(true);

        if ( (false == ui->newOperatorCheckBox->isChecked()) &&
             (false == ui->mallocCheckBox->isChecked()) &&
             (false == ui->callocCheckBox->isChecked()) &&
             (false == ui->handleCheckBox->isChecked())
           )
        {
            ui->RunTestsButton->setDisabled(true);
        }
        break;
    }
}

void LeakDialogQtUi::handleMallocCheckBox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->mallocIterationsLineEdit->setEnabled(true);
        ui->mallocBytesPerLineEdit->setEnabled(true);
        ui->RunTestsButton->setEnabled(true);
        break;
    case Qt::Unchecked:
        ui->mallocIterationsLineEdit->setDisabled(true);
        ui->mallocBytesPerLineEdit->setDisabled(true);

        if ( (false == ui->newOperatorCheckBox->isChecked()) &&
             (false == ui->mallocCheckBox->isChecked()) &&
             (false == ui->callocCheckBox->isChecked()) &&
             (false == ui->handleCheckBox->isChecked())
           )
        {
            ui->RunTestsButton->setDisabled(true);
        }
        break;
    }
}

void LeakDialogQtUi::handleCallocCheckBox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->callocIterationsLineEdit->setEnabled(true);
        ui->callocBytesPerLineEdit->setEnabled(true);
        ui->RunTestsButton->setEnabled(true);
        break;
    case Qt::Unchecked:
        ui->callocIterationsLineEdit->setDisabled(true);
        ui->callocBytesPerLineEdit->setDisabled(true);

        if ( (false == ui->newOperatorCheckBox->isChecked()) &&
             (false == ui->mallocCheckBox->isChecked()) &&
             (false == ui->callocCheckBox->isChecked()) &&
             (false == ui->handleCheckBox->isChecked())
           )
        {
            ui->RunTestsButton->setDisabled(true);
        }
        break;
    }
}

void LeakDialogQtUi::handleHandleCheckBox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->handleIterationsLineEdit->setEnabled(true);
        ui->handleBytesPerLineEdit->setEnabled(true);
        ui->RunTestsButton->setEnabled(true);
        break;
    case Qt::Unchecked:
        ui->handleIterationsLineEdit->setDisabled(true);
        ui->handleBytesPerLineEdit->setDisabled(true);

        if ( (false == ui->newOperatorCheckBox->isChecked()) &&
             (false == ui->mallocCheckBox->isChecked()) &&
             (false == ui->callocCheckBox->isChecked()) &&
             (false == ui->handleCheckBox->isChecked())
           )
        {
            ui->RunTestsButton->setDisabled(true);
        }
        break;
    }
}

