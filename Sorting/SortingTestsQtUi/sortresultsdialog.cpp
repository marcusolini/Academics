#include "sortresultsdialog.h"
#include "ui_sortresultsdialog.h"

#include <QMessageBox>

#include <chrono>

SortResultsDialog::SortResultsDialog(QWidget *parent, std::vector<CSortTest>& sortTests) :
    QDialog(parent),
    ui(new Ui::SortResultsDialog)
{
    ui->setupUi(this);

    connect(ui->ClosePushButton, SIGNAL (released()), this, SLOT(handleClosePushbutton()));

    QString sTempQString;

    sTempQString = QString::fromWCharArray(CSortTest::ConvertSortStateToString(CSortTest::ESTATE_TYPE::NOT_RUN).c_str());
    ui->QuickSortStatusLineEdit->setText(sTempQString);
    ui->MergeSortStatusLineEdit->setText(sTempQString);
    ui->BubbleSortStatusLineEdit->setText(sTempQString);

    for (auto& iTest : sortTests)
    {
         switch (iTest.GetSortType())
         {
         case CSortTest::ESORT_TYPE::QUICK_SORT:
         {
            sTempQString = QString::fromWCharArray(CSortTest::ConvertSortStateToString(iTest.GetState()).c_str());
            ui->QuickSortStatusLineEdit->setText(sTempQString);

            ui->QuickSortNumberOfSortsLineEdit->setText( std::to_string(iTest.GetNumberOfSorts()).c_str() );

            auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
            ui->QuickSortSortDurationLineEdit->setText(std::to_string(nTotalMicroseconds.count()).c_str());
            ui->QuickSortUnitsLineEdit->setText(tr("microseconds") );

            sTempQString = QString::fromWCharArray(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str());

            ui->QuickSortSortArrayLineEdit->setText(sTempQString);
            break;
         }
         case CSortTest::ESORT_TYPE::MERGE_SORT:
         {
             sTempQString = QString::fromWCharArray(CSortTest::ConvertSortStateToString(iTest.GetState()).c_str());
             ui->MergeSortStatusLineEdit->setText(sTempQString);

             ui->MergeSortNumberOfSortsLineEdit->setText( std::to_string(iTest.GetNumberOfSorts()).c_str() );

             auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
             ui->MergeSortSortDurationLineEdit->setText(std::to_string(nTotalMicroseconds.count()).c_str());
             ui->MergeSortUnitsLineEdit->setText(tr("microseconds") );

             sTempQString = QString::fromWCharArray(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str());

             ui->MergeSortSortArrayLineEdit->setText(sTempQString);
             break;
         }
         case CSortTest::ESORT_TYPE::BUBBLE_SORT:
         {
             sTempQString = QString::fromWCharArray(CSortTest::ConvertSortStateToString(iTest.GetState()).c_str());
             ui->BubbleSortStatusLineEdit->setText(sTempQString);

             ui->BubbleSortNumberOfSortsLineEdit->setText( std::to_string(iTest.GetNumberOfSorts()).c_str() );

             auto nTotalMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(iTest.GetDuration());
             ui->BubbleSortSortDurationLineEdit->setText(std::to_string(nTotalMicroseconds.count()).c_str());
             ui->BubbleSortUnitsLineEdit->setText(tr("microseconds") );

             sTempQString = QString::fromWCharArray(CSortTest::ConvertVectorToString(iTest.GetSortedArray()).c_str());

             ui->BubbleSortSortArrayLineEdit->setText(sTempQString);
             break;
         }
         default:
             break;
         }

    }
}

SortResultsDialog::~SortResultsDialog()
{
    delete ui;
}

void SortResultsDialog::handleClosePushbutton()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Close"));
    msgBox.setInformativeText(tr("Close Sort Results?"));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int nRet = msgBox.exec();

    if ( QMessageBox::Yes == nRet)
    {
        this->close();
    }
}

