//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include <QMessageBox>

#include <thread>
#include <sstream>

#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "leakdialogqtui.h"

#include "LeakLib.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    m_bThreadContinue = true;
    m_bThreadPause = false;

    // Marquee Progress Bar
    ui->RunTestsProgressBar->setMinimum(0);
    ui->RunTestsProgressBar->setMaximum(0);

    connect(ui->StopPushButton, SIGNAL (released()), this, SLOT(handleStopButton()));
    connect(ui->PausePushButton, SIGNAL (released()), this, SLOT(handlePauseButton()));
    connect(this, SIGNAL (closeSignal()), this, SLOT(handleCloseSignal()));

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parent);

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
        std::thread progressThread = std::thread(ProgressDialog::ProgressThreadFunction, this, &iTest);
        progressThread.detach();
        ui->RunTestsLabel->setText(tr("Running Leak Tests..."));
    }
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::handleStopButton()
{
    bool bThreadAlreadyPaused = m_bThreadPause;
    if ( false == bThreadAlreadyPaused)
    {
        m_bThreadPause = true;
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(100);
        ui->RunTestsProgressBar->setValue(0);
    }

    QMessageBox msgBox;
    msgBox.setText(tr("Stop"));
    msgBox.setInformativeText(tr("Stop Resource Leak Tests?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int nRet = msgBox.exec();

    if ( QMessageBox::Yes == nRet)
    {
        m_bThreadContinue = false;
        //this->close();
    }
    else
    {
        if ( false == bThreadAlreadyPaused)
        {
            m_bThreadPause = false;
            ui->RunTestsProgressBar->setMinimum(0);
            ui->RunTestsProgressBar->setMaximum(0);
        }
    }

}

void ProgressDialog::handlePauseButton()
{
    QString sText = ui->PausePushButton->text();

    if (sText.toUpper().contains(tr("PAUSE")))
    {
        m_bThreadPause = true;
        ui->RunTestsLabel->setText(tr("Paused Leak Tests"));
        ui->PausePushButton->setText(tr("Resume"));
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(100);
        ui->RunTestsProgressBar->setValue(0);
    }
    else
    {
        m_bThreadPause = false;
        ui->RunTestsLabel->setText(tr("Running Leak Tests..."));
        ui->PausePushButton->setText(tr("Pause"));
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(0);
    }
}

void ProgressDialog::handleCloseSignal()
{
    // Message any failures before closing
    QMessageBox msgBox;
    msgBox.setText(tr("Failure"));
    msgBox.setIcon(QMessageBox::Critical);

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parentWidget());

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
        if ( CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED == iTest.GetState())
        {
            switch (iTest.GetResourceAllocationType())
            {
            case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
                msgBox.setInformativeText(tr("new operator failure."));
                msgBox.exec();
            break;
            case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
                msgBox.setInformativeText(tr("malloc function failure."));
                msgBox.exec();
            break;
            case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
                msgBox.setInformativeText(tr("calloc function failure."));
                msgBox.exec();
                break;
            case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
                msgBox.setInformativeText(tr("handle function."));
                msgBox.exec();
                break;
            default:
                msgBox.setInformativeText(tr("Invalid allocation type."));
                msgBox.exec();
            break;
            }
        }
    }

    this->close();
}

void ProgressDialog::Complete()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    bool bAllCompleted = true;

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parentWidget());

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
        if ( false == iTest.IsComplete())
        {
            bAllCompleted = false;
            break;
        }
    }

    if (true == bAllCompleted)
    {
        // Using emit to signal originating thread.  Windows execption occurs for any UI interactions.
        emit closeSignal();
    }
}

void ProgressDialog::ProgressThreadFunction(ProgressDialog* pProgressDialog, CResourceLeakTest* iTest)
{
    int nStatus = 0;
    std::wstringstream ss;
    uint64_t id = 0;

    iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::STARTED);

    ss << std::this_thread::get_id();
    id = std::stoull(ss.str());

    iTest->SetThreadId(id);

    std::this_thread::yield();

    for (std::size_t nIteration = 0;
         (nIteration < iTest->GetInterations()) && (pProgressDialog->m_bThreadContinue) && (0 == nStatus);
         nIteration++)
    {
        iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING);

         switch (iTest->GetResourceAllocationType())
         {
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
              nStatus = CLeakLib::LeakNewMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
              }
              break;
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
              nStatus = CLeakLib::LeakMallocMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
              }
              break;
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
              nStatus = CLeakLib::LeakCallocMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
              }
              break;
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
              nStatus = CLeakLib::LeakHandle(1);
              if (0 != nStatus)
              {
                  iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
              }
              break;
         default:
              iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
              break;
         }

         while (pProgressDialog->m_bThreadPause && pProgressDialog->m_bThreadContinue)
         {
              iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::PAUSED);
              std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });
         }
    }

    if ( false == pProgressDialog->m_bThreadContinue)
    {
        iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::INTERRUPTED);
    }
    else
    {
        if ( 0 == nStatus)
        {
            iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::SUCCESS);
        }
        else
        {
            iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
        }
    }

    std::lock_guard<std::recursive_mutex> lock(pProgressDialog->m_mutex);

    iTest->SetIsComplete();
    pProgressDialog->Complete();
}
