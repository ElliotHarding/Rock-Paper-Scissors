#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPainter>
#include <QRandomGenerator>

namespace Constants {
    const float UpdateGameObjectsFrequency = 1000;

    const QMap<QPair<GameObjectType, GameObjectType>, GameObjectType> CollisionResults = {
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_ROCK), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_PAPER), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_ROCK, GO_SCISSORS), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_ROCK), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_PAPER), GO_PAPER},
        {QPair<GameObjectType, GameObjectType>(GO_PAPER, GO_SCISSORS), GO_SCISSORS},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_ROCK), GO_ROCK},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_PAPER), GO_SCISSORS},
        {QPair<GameObjectType, GameObjectType>(GO_SCISSORS, GO_SCISSORS), GO_SCISSORS}
    };
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 25, 25));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 25, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 25));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 45, 5));
    m_gameObjects.push_back(new GameObject(this, GO_ROCK, 5, 45));

    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 105, 105));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 125, 125));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 105, 125));
    m_gameObjects.push_back(new GameObject(this, GO_PAPER, 125, 105));

    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 5, 105));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 5, 125));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 25, 105));
    m_gameObjects.push_back(new GameObject(this, GO_SCISSORS, 25, 125));

    m_pUpdateGameObjectsTimer = new QTimer(this);
    connect(m_pUpdateGameObjectsTimer, SIGNAL(timeout()), this, SLOT(onUpdateGameObjects()));
    m_pUpdateGameObjectsTimer->start(Constants::UpdateGameObjectsFrequency);
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

void MainWindow::onUpdateGameObjects()
{
    //Update positions
    for(GameObject* go : m_gameObjects)
    {
        int randomValue = QRandomGenerator::global()->generateDouble() * 4;
        if(randomValue == 0)
        {
            go->setGeometry(go->geometry().translated(1, 0));
        }
        else if(randomValue == 1)
        {
            go->setGeometry(go->geometry().translated(-1, 0));
        }
        else if(randomValue == 2)
        {
            go->setGeometry(go->geometry().translated(0, 1));
        }
        else if(randomValue == 3)
        {
            go->setGeometry(go->geometry().translated(0, -1));
        }
        else
        {
            qDebug() << "MainWindow::onUpdateGameObjects - Invalid random number generated";
        }
    }

    //Check collisions - update types
    for(GameObject* go1 : m_gameObjects)
    {
        for(GameObject* go2 : m_gameObjects)
        {
            go1->checkCollided(go2);
        }
    }

    update();
}

QColor getTypeColor(GameObjectType type)
{
    switch (type)
    {
    case GO_PAPER:
        return Qt::red;
    case GO_ROCK:
        return Qt::blue;
    case GO_SCISSORS:
        return Qt::green;
    default:
        qDebug() << "getTypeColor - unknown type";
    }
}

GameObject::GameObject(QWidget *parent, GameObjectType goType, const int& xPos, const int& yPos) : QWidget(parent),
    m_type(goType)
{
    m_color = getTypeColor(goType);
    setGeometry(xPos, yPos, 10, 10);//Todo constify
}

void GameObject::setType(GameObjectType type)
{
    m_type = type;
    m_color = getTypeColor(m_type);
}

GameObjectType GameObject::getType()
{
    return m_type;
}

void GameObject::checkCollided(GameObject *other)
{
    if(geometry().intersects(other->geometry()))
    {
        m_type = Constants::CollisionResults[QPair<GameObjectType, GameObjectType>(m_type, other->getType())];
    }
}

void GameObject::paintEvent(QPaintEvent *paintEvent)
{
    QPainter painter(this);
    painter.fillRect(geometry(), m_color);
}
