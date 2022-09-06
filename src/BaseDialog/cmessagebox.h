#ifndef CMESSAGEBOX_H
#define CMESSAGEBOX_H

#include <QWidget>
#include "cbasewidget.h"
namespace Ui {
class CMessageBox;
}

class CMessageBox : public CBaseWidget
{
    Q_OBJECT

public:
    explicit CMessageBox(QWidget *parent = nullptr);
    ~CMessageBox();

private:
    Ui::CMessageBox *ui;
};

#endif // CMESSAGEBOX_H
