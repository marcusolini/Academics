#include "progressdialog.h"
#include "ui_progressdialog.h"

#include "sortingdialogqtui.h"

#include <QMessageBox>

#include <thread>

//#include "../SortingShared/SortingLib.h"
#include "../SortingShared/SortTest.h"
#include "../SortingShared/SortThreadFunction.h"

ProgressDialog::ProgressDialog(QWidget *parent, std::vector<CSortTest>& sortTests) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    // Marquee Progress Bar
    ui->runSortsprogressBar->setMinimum(0);
    ui->runSortsprogressBar->setMaximum(0);

    connect(this, SIGNAL (completeSignal()), this, SLOT(handleCompleteSignal()));

    std::lock_guard<std::recursive_mutex> lock(sortTestsMutex);
    std::thread watchDogThread = std::thread(SortTestsWatchDogThread, this, &sortTests);
    watchDogThread.detach();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}


void ProgressDialog::handleCompleteSignal()
{
    this->close();
}

/*static*/ long ProgressDialog::SortTestsWatchDogThread(ProgressDialog* pProgressDialog, std::vector<CSortTest>* pSortTests)
{
    std::vector<std::thread> sortTestThreads;

    long nStatus = 0;

    try
    {
        if (nullptr == pProgressDialog) throw EINVAL;
        if (nullptr == pSortTests) throw EINVAL;

        {std::lock_guard<std::recursive_mutex> lock(pProgressDialog->sortTestsMutex);
        for (auto& iTest : (*pSortTests))
        {
             sortTestThreads.push_back( std::thread(SortThreadFunction, &iTest) );
        }
        } //std::lock_guard<std::recursive_mutex> lock(pProgressDialog->sortTestsMutex);

        for (auto& iTestThread : sortTestThreads)
        {
             iTestThread.join();
        }
    }
    catch (long& check_catch_lresult)
    {
        nStatus = check_catch_lresult;
    }

    emit pProgressDialog->completeSignal();

    return nStatus;

}
