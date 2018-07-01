#ifndef SORTRESULTSDIALOG_H
#define SORTRESULTSDIALOG_H

#include <QDialog>

#include "../SortingShared/SortTest.h"

namespace Ui {
class SortResultsDialog;
}

class SortResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SortResultsDialog(QWidget *parent, std::vector<CSortTest>& sortTests);
    ~SortResultsDialog();

public slots:
    void handleClosePushbutton();

private:
    Ui::SortResultsDialog *ui;
};

#endif // SORTRESULTSDIALOG_H
