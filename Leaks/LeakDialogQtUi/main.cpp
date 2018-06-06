#include "leakdialogqtui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LeakDialogQtUi w;
    w.show();

    return a.exec();
}
