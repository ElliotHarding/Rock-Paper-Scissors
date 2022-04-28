#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>
#include <QThread>
#include <QLayout>

namespace Constants {

const int GameObjectSize = 10;

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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///MainWindow
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pDlgSettings = new DLG_Settings(this);
    m_pDlgSettings->show();

    connect(m_pDlgSettings, SIGNAL(onStart()), this, SLOT(onStart()));
    connect(m_pDlgSettings, SIGNAL(onStop()), this, SLOT(onStop()));
    connect(m_pDlgSettings, SIGNAL(onReset()), this, SLOT(onReset()));
    connect(m_pDlgSettings, SIGNAL(onUpdateMoveFrequency(int)), this, SLOT(onUpdateMoveFrequency(int)));

    const QList<GameObjectSpawnSettings> spawnSettings = m_pDlgSettings->spawnSettings();
    for(const GameObjectSpawnSettings typeSpawnSettings : spawnSettings)
    {
        for(int i = 0; i < typeSpawnSettings.count; i++)
        {
            m_gameObjects.push_back(new GameObject(this, typeSpawnSettings.type, typeSpawnSettings.position));
        }
    }

    m_pUpdateGameObjectsTimer = new QTimer(this);
    connect(m_pUpdateGameObjectsTimer, SIGNAL(timeout()), this, SLOT(onUpdateGameObjects()));
    m_pUpdateGameObjectsTimer->start(m_pDlgSettings->moveUpdateFrequencyMs());
}

MainWindow::~MainWindow()
{
    for(GameObject* go : m_gameObjects)
    {
        delete go;
        go = nullptr;
    }
    m_gameObjects.clear();

    //It gets deleted via QT
    m_pDlgSettings = nullptr;

    delete ui;
}

void MainWindow::reset()
{
    int count = 0;
    const QList<GameObjectSpawnSettings> spawnSettings = m_pDlgSettings->spawnSettings();
    for(const GameObjectSpawnSettings typeSpawnSettings : spawnSettings)
    {
        for(int i = 0; i < typeSpawnSettings.count; i++)
        {
            if(count < m_gameObjects.count())
            {
                m_gameObjects[count]->reset(typeSpawnSettings.type, typeSpawnSettings.position);
            }
            else
            {
                m_gameObjects.push_back(new GameObject(this, typeSpawnSettings.type, typeSpawnSettings.position));
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

void MainWindow::onUpdateGameObjects()
{
    if(m_gameObjects.count() == 0)
    {
        m_pUpdateGameObjectsTimer->stop();
        return;
    }

    const int percentageRandomDirection = m_pDlgSettings->moveRandomDirectionPercentage() / 4;

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

            const int pushFromCenterSize = m_pDlgSettings->centerPushRange();

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
            go1->checkCollided(go2);
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

        if(m_pDlgSettings->loopGame())
        {
            QThread::msleep(m_pDlgSettings->msBetweenLoops());
            reset();
            m_pUpdateGameObjectsTimer->start(m_pDlgSettings->moveUpdateFrequencyMs());
        }
    }
}

void MainWindow::onStart()
{
    m_pUpdateGameObjectsTimer->start(m_pDlgSettings->moveUpdateFrequencyMs());
}

void MainWindow::onStop()
{
    m_pUpdateGameObjectsTimer->stop();
}

void MainWindow::onReset()
{
    reset();
}

void MainWindow::onUpdateMoveFrequency(int frequencyMs)
{
    if(m_pUpdateGameObjectsTimer->isActive())
    {
        m_pUpdateGameObjectsTimer->stop();
    }
    m_pUpdateGameObjectsTimer->start(frequencyMs);
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

void GameObject::checkCollided(GameObject *other)
{
    if(geometry().intersects(other->geometry()))
    {
        setType(Constants::CollisionResults[QPair<GameObjectType, GameObjectType>(m_type, other->getType())]);
    }
}

void GameObject::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.fillRect(QRect(0,0,Constants::GameObjectSize,Constants::GameObjectSize), m_color);
}
