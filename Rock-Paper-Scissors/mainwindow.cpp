#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

namespace Constants {
    const float UpdateGameObjectsFrequency = 1;

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

    m_pUpdateGameObjectsTimer = new QTimer(this);
    connect(m_pUpdateGameObjectsTimer, SIGNAL(timeout()), this, SLOT(onUpdateGameObjects()));
    m_pUpdateGameObjectsTimer->start(Constants::UpdateGameObjectsFrequency);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onUpdateGameObjects()
{
    //Update positions
    for(GameObject* go : m_gameObjects)
    {
        int randomValue = qrand() % 4;
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

GameObject::GameObject(QWidget *parent, GameObjectType goType) : QWidget(parent),
    m_type(goType)
{
}

void GameObject::setType(GameObjectType type)
{
    m_type = type;
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

}
