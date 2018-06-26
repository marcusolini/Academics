//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "sortingdialogqtui.h"
#include "ui_sortingdialogqtui.h"

#include <QIntValidator>
#include <QMessageBox>

sortingdialog::sortingdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sortingdialog)
{
    ui->setupUi(this);

    ui->SortArrayLinedit->setEnabled(true);
    ui->SortArrayLinedit->setValidator( new QIntValidator(1, 999999999, this));

    ui->QuickSortCheckbox->setDisabled(true);
    ui->MergeSortCheckbox->setDisabled(true);
    ui->BubbleSortCheckbox->setDisabled(true);

    ui->RunSortTestsPushbutton->setDisabled(true);
    ui->CloseSortTestsPushbutton->setEnabled(true);

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
}
