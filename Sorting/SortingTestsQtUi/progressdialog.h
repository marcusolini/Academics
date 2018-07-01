#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

#include <mutex>

#include "../SortingShared/SortTest.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent, std::vector<CSortTest>& sortTests);
    ~ProgressDialog();

    std::recursive_mutex sortTestsMutex;

public slots:
    void handleCompleteSignal();

private:
    Ui::ProgressDialog *ui;

    static long SortTestsWatchDogThread(ProgressDialog* pProgressDialog, std::vector<CSortTest>* pSortTests);

signals:
    void completeSignal();
};

#endif // PROGRESSDIALOG_H
