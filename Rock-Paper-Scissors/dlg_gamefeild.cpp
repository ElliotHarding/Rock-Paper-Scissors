#include "dlg_gamefeild.h"
#include "ui_dlg_gamefeild.h"

#include <QHBoxLayout>

DLG_GameFeild::DLG_GameFeild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_GameFeild)
{
    ui->setupUi(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
}

DLG_GameFeild::~DLG_GameFeild()
{
    delete ui;
}
