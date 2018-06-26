#include "sortingdialogqtui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sortingdialog w;
    w.show();

    return a.exec();
}
