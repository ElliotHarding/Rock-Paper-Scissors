#include "dlg_settings.h"
#include "ui_dlg_settings.h"
#include <QGridLayout>
#include <QComboBox>

namespace StartSettings
{
const GameObjectSpawnSettings InitialSpawnSettingsRow1 = {"Red", QPoint(0, 0), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow2 = {"Green", QPoint(0, 200), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow3 = {"Blue", QPoint(200, 200), 5};

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
    updateCollisionTable();
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

void DLG_Settings::updateCollisionTable()
{
    QList<GameObjectType> types;

    int layoutRow = 1;
    int layoutCol = 1;
    QGridLayout* layout = new QGridLayout(ui->wdg_collisionTable);

    for(int settingsWidgetRow = 0; settingsWidgetRow < ui->listWidget_gameObjectSettings->count(); settingsWidgetRow++)
    {
        QListWidgetItem* item = ui->listWidget_gameObjectSettings->item(settingsWidgetRow);
        WDG_GameObjectSettingsRow* rowWidget = dynamic_cast<WDG_GameObjectSettingsRow*>(ui->listWidget_gameObjectSettings->itemWidget(item));

        //Do only for unique types
        const GameObjectType type = rowWidget->getType();
        if(!types.contains(type))
        {
            types.push_back(type);

            QLabel* newRowTypeLabel = new QLabel(type);
            layout->addWidget(newRowTypeLabel, layoutRow++, 0);

            QLabel* newColTypeLbl = new QLabel(type);
            layout->addWidget(newColTypeLbl, 0, layoutCol++);
        }
    }

    for(int type1 = 0; type1 < types.count(); type1++)
    {
        for(int type2 = 0; type2 < types.count(); type2++)
        {
            const QPair<GameObjectType, GameObjectType> typePair(types[type1], types[type2]);
            if(m_collisionResults.find(typePair) == m_collisionResults.end())
            {
                m_collisionResults[typePair] = types[type1];
            }

            QComboBox* resultsCombo = new QComboBox();
            resultsCombo->addItem(m_collisionResults[typePair]);
            if(type1 != type2)
            {
                resultsCombo->addItem(m_collisionResults[typePair] == types[type1] ? types[type2] : types[type1]);
            }

            layout->addWidget(resultsCombo, type1+1, type2+1);
        }
    }

    ui->wdg_collisionTable->setLayout(layout);
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

void DLG_Settings::closeEvent(QCloseEvent* e)
{
    emit onClose();
    QDialog::closeEvent(e);
}

void DLG_Settings::on_btn_addGameObjectSettings_clicked()
{
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow1);
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
