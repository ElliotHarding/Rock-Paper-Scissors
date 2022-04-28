#include "wdg_gameobjectsettingsrow.h"
#include "ui_wdg_gameobjectsettingsrow.h"

WDG_GameObjectSettingsRow::WDG_GameObjectSettingsRow(QListWidgetItem* pListWidgetItem) :
    ui(new Ui::WDG_GameObjectSettingsRow),
    m_pListWidgetItem(pListWidgetItem)
{
    ui->setupUi(this);
}

WDG_GameObjectSettingsRow::~WDG_GameObjectSettingsRow()
{
    delete ui;
}

void WDG_GameObjectSettingsRow::on_btn_delete_clicked()
{
    emit onDelete(m_pListWidgetItem);
}
