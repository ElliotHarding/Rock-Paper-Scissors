#include "wdg_gameobjectsettingsrow.h"
#include "ui_wdg_gameobjectsettingsrow.h"

WDG_GameObjectSettingsRow::WDG_GameObjectSettingsRow(QListWidgetItem* pListWidgetItem, GameObjectSpawnSettings spawnSettings) :
    ui(new Ui::WDG_GameObjectSettingsRow),
    m_pListWidgetItem(pListWidgetItem)
{
    ui->setupUi(this);
    ui->cb_type->setCurrentText(spawnSettings.type);
    ui->sb_spawnPosX->setValue(spawnSettings.position.x());
    ui->sb_spawnPosY->setValue(spawnSettings.position.y());
    ui->sb_spawnCount->setValue(spawnSettings.count);
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

GameObjectType WDG_GameObjectSettingsRow::getType()
{
    return ui->cb_type->currentText();
}

void WDG_GameObjectSettingsRow::on_btn_delete_clicked()
{
    emit onDelete(m_pListWidgetItem);
}
