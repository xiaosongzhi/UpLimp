#include "cbasewidget.h"
#include "cmessagebox.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMessageBox w;
    w.show();
    return a.exec();
}

