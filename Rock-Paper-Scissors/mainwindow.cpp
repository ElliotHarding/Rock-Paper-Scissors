#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "collisioncombobox.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>
#include <QThread>
#include <QLayout>

namespace Constants {

const int CenterPlayFeild = 100;

}

namespace StartSettings
{
const GameObjectSpawnSettings InitialSpawnSettingsRow1 = {"Red", QPoint(0, 0), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow2 = {"Green", QPoint(0, 200), 5};
const GameObjectSpawnSettings InitialSpawnSettingsRow3 = {"Blue", QPoint(200, 200), 5};

const QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> CollisionResults = {
    {QPair<GameObjectType, GameObjectType>("Red", "Red"), "Red"},
    {QPair<GameObjectType, GameObjectType>("Red", "Green"), "Green"},
    {QPair<GameObjectType, GameObjectType>("Red", "Blue"), "Red"},
    {QPair<GameObjectType, GameObjectType>("Green", "Red"), "Green"},
    {QPair<GameObjectType, GameObjectType>("Green", "Green"), "Green"},
    {QPair<GameObjectType, GameObjectType>("Green", "Blue"), "Blue"},
    {QPair<GameObjectType, GameObjectType>("Blue", "Red"), "Red"},
    {QPair<GameObjectType, GameObjectType>("Blue", "Green"), "Blue"},
    {QPair<GameObjectType, GameObjectType>("Blue", "Blue"), "Blue"}
};

const bool LoopGame = false;//Auto restart game
const int AutoRestartDelay = 10;//Seconds between auto restarted game
const int MoveUpdateFrequency = 10;//How often gameobjects positions are updated (ms)
const int MoveRandomDirectionPercentageChance = 95;//Percentage chance a game object moves in a random direction versus heading towards center
const int CenterPushRange = 10;//Once within x blocks of center, tend to move game object away from center
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///MainWindow
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget_gameObjectSettings->setDragDropMode(QAbstractItemView::DragDropMode::NoDragDrop);

    m_pDlgGameFeild = new DLG_GameFeild(this);
    m_pDlgGameFeild->show();

    QGridLayout* layout = new QGridLayout(ui->wdg_collisionTable);
    ui->wdg_collisionTable->setLayout(layout);

    setDefaultSettings();
    reset();

    m_pUpdateGameObjectsTimer = new QTimer(this);
    connect(m_pUpdateGameObjectsTimer, SIGNAL(timeout()), this, SLOT(onUpdateGameObjects()));
    m_pUpdateGameObjectsTimer->start(ui->sb_updateFrequency->value());
}

MainWindow::~MainWindow()
{
    for(GameObject* go : m_gameObjects)
    {
        delete go;
        go = nullptr;
    }
    m_gameObjects.clear();

    delete ui;
}

void MainWindow::setDefaultSettings()
{
    //Remove existing spawn setting widgets
    int row = ui->listWidget_gameObjectSettings->count();
    while(row > 0)
    {
        QListWidgetItem* item = ui->listWidget_gameObjectSettings->item(--row);
        ui->listWidget_gameObjectSettings->removeItemWidget(item);
        delete item;
    }

    //Add default spawn setting widgets
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow1);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow2);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow3);

    ui->cb_loopGame->setChecked(StartSettings::LoopGame);
    ui->sb_secondsBetweenLoops->setValue(StartSettings::AutoRestartDelay);
    ui->sb_updateFrequency->setValue(StartSettings::MoveUpdateFrequency);
    ui->sb_moveRandomPercentage->setValue(StartSettings::MoveRandomDirectionPercentageChance);
    ui->sb_centerPushRange->setValue(StartSettings::CenterPushRange);

    //Set default collision results
    m_collisionResults = StartSettings::CollisionResults;
    updateCollisionTableWidgets();
}

void MainWindow::reset()
{
    int count = 0;
    const int rows = ui->listWidget_gameObjectSettings->count();
    for(int row = 0; row < rows; row++)
    {
        QListWidgetItem* item = ui->listWidget_gameObjectSettings->item(row);
        WDG_GameObjectSettingsRow* rowWidget = dynamic_cast<WDG_GameObjectSettingsRow*>(ui->listWidget_gameObjectSettings->itemWidget(item));
        GameObjectSpawnSettings spawnSettings = rowWidget->getSettings();

        for(int i = 0; i < spawnSettings.count; i++)
        {
            if(count < m_gameObjects.count())
            {
                m_gameObjects[count]->reset(spawnSettings.type, spawnSettings.position);
            }
            else
            {
                m_gameObjects.push_back(new GameObject(m_pDlgGameFeild, spawnSettings.type, spawnSettings.position));
                layout()->addWidget(m_gameObjects[count]);
            }
            count++;
        }
    }

    while(count < m_gameObjects.count())
    {
        delete m_gameObjects[count];
        m_gameObjects.removeAt(count);
    }
}

void MainWindow::addGameObjectSettingsRow(GameObjectSpawnSettings spawnSettings)
{
    QListWidgetItem* item = new QListWidgetItem();
    WDG_GameObjectSettingsRow* itemWidget = new WDG_GameObjectSettingsRow(item, spawnSettings);
    item->setSizeHint(QSize(itemWidget->geometry().width(), itemWidget->geometry().height()));
    ui->listWidget_gameObjectSettings->addItem(item);
    ui->listWidget_gameObjectSettings->setItemWidget(item, itemWidget);

    connect(itemWidget, SIGNAL(onDelete(QListWidgetItem*)), this, SLOT(onSpawnSettingsDelete(QListWidgetItem*)));
    connect(itemWidget, SIGNAL(onUpdateType(GameObjectType)), this, SLOT(onSpawnSettingsTypeChanged(GameObjectType)));

    onSpawnSettingsTypeChanged(spawnSettings.type);

    item = nullptr;
    itemWidget = nullptr;
}

void MainWindow::updateCollisionTableWidgets()
{  
    qDeleteAll(ui->wdg_collisionTable->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
    qDeleteAll(ui->wdg_collisionTable->layout()->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));

    //To be filled with unique game object types
    QList<GameObjectType> types;

    //Layout for collision table widget
    int layoutRow = 1; //Inc for every new row & coloumn
    int layoutCol = 1;

    //Loop through all game object settings widgets
    // - For each unique game object type create a table index of that type for
    //   both column and row
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
            dynamic_cast<QGridLayout*>(ui->wdg_collisionTable->layout())->addWidget(newRowTypeLabel, layoutRow++, 0);

            QLabel* newColTypeLbl = new QLabel(type);
            dynamic_cast<QGridLayout*>(ui->wdg_collisionTable->layout())->addWidget(newColTypeLbl, 0, layoutCol++);
        }
    }

    //Loop through all the combinations of game object types
    // - Create a combobox for each combination
    // - Fill combobox with list of all game object types
    // - If result is already saved use that as current text for combobox.
    //   Otherwise set to type1
    for(int type1 = 0; type1 < types.count(); type1++)
    {
        for(int type2 = 0; type2 < types.count(); type2++)
        {
            const QPair<GameObjectType, GameObjectType> typePair(types[type1], types[type2]);
            if(m_collisionResults.find(typePair) == m_collisionResults.end())
            {
                m_collisionResults.insert(typePair, types[type1] == types[type2] ? types[type1] : types[0]);
            }

            CollisionCombobox* resultsCombo = new CollisionCombobox(typePair);
            for(GameObjectType& got : types)
            {
                resultsCombo->addItem(got);
            }
            resultsCombo->setCurrentText(m_collisionResults[typePair]);

            connect(resultsCombo, SIGNAL(onChanged(QPair<GameObjectType, GameObjectType>, GameObjectType)), this, SLOT(onCollisionResultChanged(QPair<GameObjectType, GameObjectType>, GameObjectType)));

            dynamic_cast<QGridLayout*>(ui->wdg_collisionTable->layout())->addWidget(resultsCombo, type1+1, type2+1);
        }
    }
}

void MainWindow::onUpdateGameObjects()
{
    if(m_gameObjects.count() == 0)
    {
        m_pUpdateGameObjectsTimer->stop();
        return;
    }

    const int percentageRandomDirection = ui->sb_moveRandomPercentage->value() / 4;

    //Update positions
    for(GameObject* go : m_gameObjects)
    {
        int randomValue = QRandomGenerator::global()->generateDouble() * 100;
        if(randomValue < percentageRandomDirection)
        {
            if(go->geometry().right() < geometry().right())
            {
                go->setGeometry(go->geometry().translated(1, 0));
            }
        }
        else if(randomValue < percentageRandomDirection * 2)
        {
            if(go->geometry().left() > 0)
            {
                go->setGeometry(go->geometry().translated(-1, 0));
            }
        }
        else if(randomValue < percentageRandomDirection * 3)
        {
            if(go->geometry().bottom() < geometry().bottom())
            {
                go->setGeometry(go->geometry().translated(0, 1));
            }
        }
        else if(randomValue < percentageRandomDirection * 4)
        {
            if(go->geometry().top() > 0)
            {
                go->setGeometry(go->geometry().translated(0, -1));
            }
        }
        else
        {
            const int xToCenter = Constants::CenterPlayFeild - go->geometry().x();
            const int yToCenter = Constants::CenterPlayFeild - go->geometry().y();

            const int pushFromCenterSize = ui->sb_centerPushRange->value();

            //Move towards center, unless close to it, in which case move away from center
            if(xToCenter > pushFromCenterSize || xToCenter < -pushFromCenterSize ||
               yToCenter > pushFromCenterSize || yToCenter < -pushFromCenterSize)
            {
                go->setGeometry(go->geometry().translated(xToCenter > 0 ? 1 : -1, yToCenter > 0 ? 1 : -1));
            }
            else
            {
                go->setGeometry(go->geometry().translated(xToCenter > 0 ? -1 : 1, yToCenter > 0 ? -1 : 1));
            }

        }
    }

    //Check collisions - update types - check if all the same
    const GameObjectType firstGOT = m_gameObjects[0]->getType();
    bool allSame = true;
    for(GameObject* go1 : m_gameObjects)
    {
        for(GameObject* go2 : m_gameObjects)
        {
            go1->checkCollided(go2, m_collisionResults);
        }

        if(allSame && go1->getType() != firstGOT)
        {
            allSame = false;
        }
    }

    if(allSame)
    {
        //todo update winner

        m_pUpdateGameObjectsTimer->stop();

        if(ui->cb_loopGame->isChecked())
        {
            //QThread::msleep(ui->sb_secondsBetweenLoops->value() * 1000);
            reset();
            m_pUpdateGameObjectsTimer->start(ui->sb_updateFrequency->value());
        }
    }
}

void MainWindow::on_btn_start_clicked()
{
    m_pUpdateGameObjectsTimer->start(ui->sb_updateFrequency->value());
}

void MainWindow::on_btn_stop_clicked()
{
    m_pUpdateGameObjectsTimer->stop();
}

void MainWindow::on_btn_reset_clicked()
{
    reset();
}

void MainWindow::on_btn_addGameObjectSettings_clicked()
{
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow1);
}

void MainWindow::on_btn_defaultSettings_clicked()
{
    setDefaultSettings();
}

void MainWindow::on_sb_updateFrequency_valueChanged(int frequencyMs)
{
    if(m_pUpdateGameObjectsTimer->isActive())
    {
        m_pUpdateGameObjectsTimer->stop();
    }
    m_pUpdateGameObjectsTimer->start(frequencyMs);
}

void MainWindow::onSpawnSettingsDelete(QListWidgetItem* pListWidgetItem)
{
    ui->listWidget_gameObjectSettings->removeItemWidget(pListWidgetItem);
    delete pListWidgetItem;

    updateCollisionTableWidgets();
}

void MainWindow::onSpawnSettingsTypeChanged(GameObjectType)
{
    updateCollisionTableWidgets();
}

void MainWindow::onCollisionResultChanged(QPair<GameObjectType, GameObjectType> typePair, GameObjectType goTypeResult)
{
    m_collisionResults[typePair] = goTypeResult;
}



