//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "sortingdialogqtui.h"
#include "ui_sortingdialogqtui.h"
#include "progressdialog.h"
#include "sortresultsdialog.h"

#include <QIntValidator>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QFile>

#include <thread>

#include "../../Error_Checks/ERROR_CHECKS.H"
#include "../SortingShared/SortTest.h"

sortingdialog::sortingdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sortingdialog)
{
    ui->setupUi(this);

    ui->SortArrayLinedit->setEnabled(true);
    //ui->SortArrayLinedit->setValidator( new QIntValidator(1, 999999999, this));

    ui->QuickSortCheckbox->setDisabled(true);
    ui->MergeSortCheckbox->setDisabled(true);
    ui->BubbleSortCheckbox->setDisabled(true);

    ui->RunSortTestsPushbutton->setDisabled(true);
    ui->CloseSortTestsPushbutton->setEnabled(true);

    connect(ui->SortArrayLinedit, SIGNAL (textChanged(const QString&)), this, SLOT(handleSortArrayLineditTextChanged(const QString&)));
    connect(ui->OpenFilePushButton, SIGNAL (released()), this, SLOT(handleOpenFilePushbutton()));

    connect(ui->QuickSortCheckbox, SIGNAL (stateChanged(int)), this, SLOT(handleQuickSortCheckbox(int)));
    connect(ui->MergeSortCheckbox, SIGNAL (stateChanged(int)), this, SLOT(handleMergeSortCheckbox(int)));
    connect(ui->BubbleSortCheckbox, SIGNAL (stateChanged(int)), this, SLOT(handleBubbleSortCheckbox(int)));

    connect(ui->CloseSortTestsPushbutton, SIGNAL (released()), this, SLOT(handleCloseSortTestsPushbutton()));
    connect(ui->RunSortTestsPushbutton, SIGNAL (released()), this, SLOT(handleRunSortTestsPushbutton()));
}

sortingdialog::~sortingdialog()
{
    delete ui;
}



void sortingdialog::handleSortArrayLineditTextChanged(const QString& text)
{
    //long nStatus = 0;

    std::vector<int> vArray;

    try
    {
       CHECK_SUCCEEDED_LOG_THROW( (CSortTest::ValidateAndConvertFromStringToVector(text.toStdWString(), vArray)) );
    }
    catch (long& check_catch_lresult)
    {
        //nStatus = check_catch_lresult;

        QMessageBox msgBox;
        msgBox.setText(tr("Invalid Input"));
        msgBox.setInformativeText(tr("Please enter integer data only and clear any non-integer data."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    if (vArray.empty())
    {
        ui->QuickSortCheckbox->setDisabled(true);
        ui->MergeSortCheckbox->setDisabled(true);
        ui->BubbleSortCheckbox->setDisabled(true);
        ui->RunSortTestsPushbutton->setDisabled(true);
    }
    else
    {
        ui->QuickSortCheckbox->setEnabled(true);
        ui->MergeSortCheckbox->setEnabled(true);
        ui->BubbleSortCheckbox->setEnabled(true);

        if ( (false == ui->QuickSortCheckbox->isChecked()) &&
             (false == ui->MergeSortCheckbox->isChecked()) &&
             (false == ui->BubbleSortCheckbox->isChecked())
           )
        {
            ui->RunSortTestsPushbutton->setDisabled(true);
        }
        else
        {
            ui->RunSortTestsPushbutton->setEnabled(true);
        }
    }

    //return nStatus;
}


void sortingdialog::handleOpenFilePushbutton()
{
    QString fileNameFilter;
    QStringList fileNames;

    fileNameFilter = tr("Text files (*.txt)");

    QFileDialog OpenFileDialog(this);
    OpenFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    OpenFileDialog.setNameFilter(fileNameFilter);

    OpenFileDialog.exec();

    // TODO: Add Error Handling

    fileNames = OpenFileDialog.selectedFiles();

    QFile fFile(fileNames[0]);

    fFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QByteArray line = fFile.readLine();

    ui->SortArrayLinedit->setText(line.data());

}


void sortingdialog::handleCloseSortTestsPushbutton()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Close"));
    msgBox.setInformativeText(tr("Close Sort Test?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int nRet = msgBox.exec();

    if ( QMessageBox::Yes == nRet)
    {
        this->close();
    }
}

void sortingdialog::handleQuickSortCheckbox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->RunSortTestsPushbutton->setEnabled(true);
        break;
    case Qt::Unchecked:
    default:
        if ( (false == ui->QuickSortCheckbox->isChecked()) &&
             (false == ui->MergeSortCheckbox->isChecked()) &&
             (false == ui->BubbleSortCheckbox->isChecked())
           )
        {
            ui->RunSortTestsPushbutton->setDisabled(true);
        }
        break;
    }
}

void sortingdialog::handleMergeSortCheckbox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->RunSortTestsPushbutton->setEnabled(true);
        break;
    case Qt::Unchecked:
        if ( (false == ui->QuickSortCheckbox->isChecked()) &&
             (false == ui->MergeSortCheckbox->isChecked()) &&
             (false == ui->BubbleSortCheckbox->isChecked())
           )
        {
            ui->RunSortTestsPushbutton->setDisabled(true);
        }
        break;
    }
}

void sortingdialog::handleBubbleSortCheckbox(int state)
{
    switch ( state )
    {
    case Qt::Checked:
    case Qt::PartiallyChecked:
        ui->RunSortTestsPushbutton->setEnabled(true);
        break;
    case Qt::Unchecked:
        if ( (false == ui->QuickSortCheckbox->isChecked()) &&
             (false == ui->MergeSortCheckbox->isChecked()) &&
             (false == ui->BubbleSortCheckbox->isChecked())
           )
        {
            ui->RunSortTestsPushbutton->setDisabled(true);
        }
        break;
    }
}


void sortingdialog::handleRunSortTestsPushbutton()
{
    //long nStatus = 0;
    std::vector<int> vArray;

    try
    {
        QString sText = ui->SortArrayLinedit->text();

        CHECK_SUCCEEDED_LOG_THROW( CSortTest::ValidateAndConvertFromStringToVector(sText.toStdWString(), vArray) );

        if ( true == ui->QuickSortCheckbox->isChecked())
        {
            CSortTest sortTest( CSortTest::ESORT_TYPE::QUICK_SORT, vArray);
            sortTests.push_back(sortTest);
        }

        if ( true == ui->MergeSortCheckbox->isChecked())
        {
            CSortTest sortTest( CSortTest::ESORT_TYPE::MERGE_SORT, vArray);
            sortTests.push_back(sortTest);
        }

        if ( true == ui->BubbleSortCheckbox->isChecked())
        {
            CSortTest sortTest( CSortTest::ESORT_TYPE::BUBBLE_SORT, vArray);
            sortTests.push_back(sortTest);
        }

        if ( sortTests.size() )
        {
            ProgressDialog wProgressDialog(this, sortTests);
            wProgressDialog.show();
            wProgressDialog.exec();

            SortResultsDialog wSortResultsDialog(this, sortTests);
            wSortResultsDialog.show();
            wSortResultsDialog.exec();

        }
        else
        {
            CHECK_SUCCEEDED_LOG_THROW(EINVAL);
        }
    }
    catch (long& check_catch_lresult)
    {
        //nStatus = check_catch_lresult;

        QMessageBox msgBox;
        msgBox.setText(tr("Invalid Input"));
        msgBox.setInformativeText(tr("Please enter integer data only and clear any non-integer data."));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }

    sortTests.clear();

    //return nStatus;
}
