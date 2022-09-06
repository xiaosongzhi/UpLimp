#include "cmessagebox.h"
#include "ui_cmessagebox.h"

CMessageBox::CMessageBox(QWidget *parent) :
    CBaseWidget(parent),
    ui(new Ui::CMessageBox)
{
    ui->setupUi(this);
}

CMessageBox::~CMessageBox()
{
    delete ui;
}
