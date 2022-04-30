#include "dlg_gamefeild.h"
#include "ui_dlg_gamefeild.h"

DLG_GameFeild::DLG_GameFeild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_GameFeild),
    m_pParent(parent)
{
    ui->setupUi(this);
}

DLG_GameFeild::~DLG_GameFeild()
{
    delete ui;
}

void DLG_GameFeild::closeEvent(QCloseEvent* e)
{
    QDialog::closeEvent(e);
    m_pParent->close();
}
