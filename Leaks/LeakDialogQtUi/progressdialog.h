//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

#include <atomic>
#include <mutex>

#include "resourceleaktest.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

    static void ProgressThreadFunction(ProgressDialog* pProgressDialog, CResourceLeakTest* iTest);

public slots:
    void handleStopButton();
    void handlePauseButton();

private:
    Ui::ProgressDialog *ui;

    std::atomic<bool> m_bThreadContinue;
    std::atomic<bool> m_bThreadPause;

    std::recursive_mutex m_mutex;

    void Complete();

signals:
};

#endif // PROGRESSDIALOG_H
