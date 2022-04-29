#include "dlg_gamefeild.h"
#include "ui_dlg_gamefeild.h"

DLG_GameFeild::DLG_GameFeild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_GameFeild)
{
    ui->setupUi(this);
}

DLG_GameFeild::~DLG_GameFeild()
{
    delete ui;
}
