#include "dlg_gamefeild.h"
#include "ui_dlg_gamefeild.h"

#include <QGridLayout>

DLG_GameFeild::DLG_GameFeild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_GameFeild)
{
    ui->setupUi(this);

    QGridLayout* layout = new QGridLayout(this);
    setLayout(layout);
}

DLG_GameFeild::~DLG_GameFeild()
{
    delete ui;
}
