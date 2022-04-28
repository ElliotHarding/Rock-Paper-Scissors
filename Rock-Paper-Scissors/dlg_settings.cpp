#include "dlg_settings.h"
#include "ui_dlg_settings.h"
#include "wdg_gameobjectsettingsrow.h"

DLG_Settings::DLG_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_Settings)
{
    ui->setupUi(this);
    ui->listWidget_gameObjectSettings->setDragDropMode(QAbstractItemView::DragDropMode::NoDragDrop);

    on_btn_addGameObjectSettings_clicked();
}

DLG_Settings::~DLG_Settings()
{
    delete ui;
}

bool DLG_Settings::loopGame()
{
    return ui->cb_loopGame->isChecked();
}

int DLG_Settings::secondsBetweenLoops() const
{
    return ui->sb_secondsBetweenLoops->value();
}

int DLG_Settings::gameObjectSize() const
{
    return ui->sb_gameObjectSize->value();
}

int DLG_Settings::moveUpdateFrequencyMs() const
{
    return ui->sb_updateFrequency->value();
}

int DLG_Settings::moveRandomDirectionPercentage() const
{
    return ui->sb_moveRandomPercentage->value();
}

int DLG_Settings::centerPushRange() const
{
    return ui->sb_centerPushRange->value();
}

void DLG_Settings::on_btn_addGameObjectSettings_clicked()
{
    QListWidgetItem* item = new QListWidgetItem();
    WDG_GameObjectSettingsRow* itemWidget = new WDG_GameObjectSettingsRow(item);
    item->setSizeHint(QSize(itemWidget->geometry().width(), itemWidget->geometry().height()));
    ui->listWidget_gameObjectSettings->addItem(item);
    ui->listWidget_gameObjectSettings->setItemWidget(item, itemWidget);

    connect(itemWidget, SIGNAL(onDelete(QListWidgetItem*)), this, SLOT(onDelete(QListWidgetItem*)));

    item = nullptr;
    itemWidget = nullptr;
}

void DLG_Settings::onDelete(QListWidgetItem *pListWidgetItem)
{
    //Dont ever remove the last layer
    if(ui->listWidget_gameObjectSettings->count() > 1)
    {
        ui->listWidget_gameObjectSettings->removeItemWidget(pListWidgetItem);
        delete pListWidgetItem; //todo - check if the widget that still has ptr to pListWidgetItem is broken or something...
    }
}
