#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>
#include <QThread>
#include <QLayout>
#include <QComboBox>

namespace Constants {

const int GameObjectSize = 10;

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

    setDefaultSettings();
    updateCollisionTable();
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
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow1);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow2);
    addGameObjectSettingsRow(StartSettings::InitialSpawnSettingsRow3);

    ui->cb_loopGame->setChecked(StartSettings::LoopGame);
    ui->sb_secondsBetweenLoops->setValue(StartSettings::AutoRestartDelay);
    ui->sb_updateFrequency->setValue(StartSettings::MoveUpdateFrequency);
    ui->sb_moveRandomPercentage->setValue(StartSettings::MoveRandomDirectionPercentageChance);
    ui->sb_centerPushRange->setValue(StartSettings::CenterPushRange);

    m_collisionResults = StartSettings::CollisionResults;
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
                m_gameObjects.push_back(new GameObject(this, spawnSettings.type, spawnSettings.position));
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

    connect(itemWidget, SIGNAL(onDelete(QListWidgetItem*)), this, SLOT(onDelete(QListWidgetItem*)));

    item = nullptr;
    itemWidget = nullptr;
}

void MainWindow::updateCollisionTable()
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
            const int xToCenter = geometry().center().x() - geometry().left() - go->geometry().x();
            const int yToCenter = geometry().center().y() - geometry().top() - go->geometry().y();

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
            QThread::msleep(ui->sb_secondsBetweenLoops->value() * 1000);
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

void MainWindow::on_sb_updateFrequency_valueChanged(int frequencyMs)
{
    if(m_pUpdateGameObjectsTimer->isActive())
    {
        m_pUpdateGameObjectsTimer->stop();
    }
    m_pUpdateGameObjectsTimer->start(frequencyMs);
}

void MainWindow::onDelete(QListWidgetItem* pListWidgetItem)
{
    ui->listWidget_gameObjectSettings->removeItemWidget(pListWidgetItem);
    delete pListWidgetItem;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///GameObject
///
GameObject::GameObject(QWidget *parent, GameObjectType goType, const QPoint& spawnPoint) : QWidget(parent)
{
    reset(goType, spawnPoint);
}

void GameObject::reset(GameObjectType goType, const QPoint& spawnPoint)
{
    setType(goType);
    setGeometry(spawnPoint.x(), spawnPoint.y(), Constants::GameObjectSize, Constants::GameObjectSize);
}

void GameObject::setType(GameObjectType type)
{
    if(m_type != type)
    {
        m_type = type;
        m_color = QColor(type);
        repaint();
    }
}

GameObjectType GameObject::getType()
{
    return m_type;
}

void GameObject::checkCollided(GameObject *other, QMap<QPair<GameObjectType, GameObjectType>, GameObjectType>& collisionResults)
{
    if(geometry().intersects(other->geometry()))
    {
        setType(collisionResults[QPair<GameObjectType, GameObjectType>(m_type, other->getType())]);
    }
}

void GameObject::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,Constants::GameObjectSize,Constants::GameObjectSize), m_color);
}
