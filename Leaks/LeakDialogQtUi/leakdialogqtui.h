#ifndef LEAKDIALOGQTUI_H
#define LEAKDIALOGQTUI_H

#include <QDialog>

#include "resourceleaktest.h"

namespace Ui {
class LeakDialogQtUi;
}

class LeakDialogQtUi : public QDialog
{
    Q_OBJECT

public:
    explicit LeakDialogQtUi(QWidget *parent = 0);
    ~LeakDialogQtUi();
    int test = 10;

    std::vector<CResourceLeakTest> gTests;

public slots:
    void handleCloseButton();
    void handleNewOperatorCheckBox(int state);
    void handleRunTestsButton();

private:
    Ui::LeakDialogQtUi *ui;
};

#endif // LEAKDIALOGQTUI_H
