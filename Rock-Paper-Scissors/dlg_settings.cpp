#include "dlg_settings.h"
#include "ui_dlg_settings.h"

namespace StartSettings
{
const GameObjectSpawnSettings InitialSpawnSettingsRow1 = {"Red", QPoint(0, 0), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow2 = {"Green", QPoint(0, 200), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow3 = {"Blue", QPoint(200, 200), 5};

const GameObjectSpawnSettings SpawnSettingsRow = {"Red", QPoint(100, 100), 5};

const bool LoopGame = false;//Auto restart game
const int AutoRestartDelay = 10;//Seconds between auto restarted game
const int MoveUpdateFrequency = 10;//How often gameobjects positions are updated (ms)
const int MoveRandomDirectionPercentageChance = 95;//Percentage chance a game object moves in a random direction versus heading towards center
const int CenterPushRange = 10;//Once within x blocks of center, tend to move game object away from center
}

DLG_Settings::DLG_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DLG_Settings)
{
    ui->setupUi(this);
    ui->listWidget_gameObjectSettings->setDragDropMode(QAbstractItemView::DragDropMode::NoDragDrop);

    setDefaultSettings();
}

DLG_Settings::~DLG_Settings()
{
    delete ui;
}

void DLG_Settings::setDefaultSettings()
{
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow1);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow2);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow3);

    ui->cb_loopGame->setChecked(StartSettings::LoopGame);
    ui->sb_secondsBetweenLoops->setValue(StartSettings::AutoRestartDelay);
    ui->sb_updateFrequency->setValue(StartSettings::MoveUpdateFrequency);
    ui->sb_moveRandomPercentage->setValue(StartSettings::MoveRandomDirectionPercentageChance);
    ui->sb_centerPushRange->setValue(StartSettings::CenterPushRange);
}

//Auto restart game
bool DLG_Settings::loopGame()
{
    return ui->cb_loopGame->isChecked();
}

//Seconds between auto restarted game
int DLG_Settings::msBetweenLoops() const
{
    return ui->sb_secondsBetweenLoops->value() * 1000;
}

QList<GameObjectSpawnSettings> DLG_Settings::spawnSettings()
{
    QList<GameObjectSpawnSettings> gameObjectSpawnSettings;
    const int rows = ui->listWidget_gameObjectSettings->count();
    for(int row = 0; row < rows; row++)
    {
        QListWidgetItem* item = ui->listWidget_gameObjectSettings->item(row);
        WDG_GameObjectSettingsRow* rowWidget = dynamic_cast<WDG_GameObjectSettingsRow*>(ui->listWidget_gameObjectSettings->itemWidget(item));
        gameObjectSpawnSettings.push_back(rowWidget->getSettings());
    }
    return gameObjectSpawnSettings;
}

//How often gameobjects positions are updated
int DLG_Settings::moveUpdateFrequencyMs() const
{
    return ui->sb_updateFrequency->value();
}

//Percentage chance a game object moves in a random direction versus heading towards center
int DLG_Settings::moveRandomDirectionPercentage() const
{
    return ui->sb_moveRandomPercentage->value();
}

//Once within x blocks of center, tend to move game object away from center
int DLG_Settings::centerPushRange() const
{
    return ui->sb_centerPushRange->value();
}

void DLG_Settings::on_btn_addGameObjectSettings_clicked()
{
    addGameObjectSettingsRow(StartSettings::SpawnSettingsRow);
}

void DLG_Settings::addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings)
{
    QListWidgetItem* item = new QListWidgetItem();
    WDG_GameObjectSettingsRow* itemWidget = new WDG_GameObjectSettingsRow(item, spawnSettings);
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

void DLG_Settings::on_btn_start_clicked()
{
    emit onStart();
}

void DLG_Settings::on_btn_stop_clicked()
{
    emit onStop();
}

void DLG_Settings::on_sb_updateFrequency_valueChanged(int value)
{
    emit onUpdateMoveFrequency(value);
}

void DLG_Settings::on_btn_reset_clicked()
{
    emit onReset();
}
