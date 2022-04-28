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

GameObjectSpawnSettings WDG_GameObjectSettingsRow::getSettings()
{
    GameObjectSpawnSettings spawnSettings;
    spawnSettings.type = ui->cb_type->currentText();
    spawnSettings.count = ui->sb_spawnCount->value();
    spawnSettings.position = QPoint(ui->sb_spawnPosX->value(), ui->sb_spawnPosY->value());
    return spawnSettings;
}

void WDG_GameObjectSettingsRow::on_btn_delete_clicked()
{
    emit onDelete(m_pListWidgetItem);
}
