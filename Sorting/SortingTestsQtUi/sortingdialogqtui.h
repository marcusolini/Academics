//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef SORTINGDIALOG_H
#define SORTINGDIALOG_H

#include <QDialog>

namespace Ui {
class sortingdialog;
}

class sortingdialog : public QDialog
{
    Q_OBJECT

public:
    explicit sortingdialog(QWidget *parent = 0);
    ~sortingdialog();    


public slots:
    void handleSortArrayLineditTextChanged(const QString& text);
    void handleQuickSortCheckbox(int state);
    void handleMergeSortCheckbox(int state);
    void handleBubbleSortCheckbox(int state);
    void handleRunSortTestsPushbutton();
    void handleCloseSortTestsPushbutton();

private:
    Ui::sortingdialog *ui;



};

#endif // SORTINGDIALOG_H
